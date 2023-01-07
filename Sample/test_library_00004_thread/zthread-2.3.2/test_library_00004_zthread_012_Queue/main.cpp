/****************************************************************************
* test_library - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-29
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include <iostream>
#include <string>
#include <deque>
using namespace std;

#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "zthread/ThreadedExecutor.h"
#include "zthread/PoolExecutor.h"
#include "zthread/Exceptions.h"
#include "zthread/CountedPtr.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"
#include "zthread/Condition.h"
using namespace ZThread;


template<class T>
class TQueue
{
public:
    TQueue()
        : condition(mutex)
    {

    }

private:
    Mutex mutex;
    Condition condition;
    deque<T> data;

public:
    void put(T item)
    {
        Guard<Mutex> lock(this->mutex);
        this->data.push_back(item);
        this->condition.signal();
    }

    T get()
    {
        Guard<Mutex> lock(this->mutex);
        while (this->data.empty())
        {
            this->condition.wait();
        }
        
        T ret = this->data.front();
        this->data.pop_front();
        return ret;
    }
};


class Car
{
public:
    Car() 
        : id(-1) 
        , engine(false)
        , driveTrain(false)
        , wheels(false) 
    {

    } 

    Car(int _id)
        : id(_id)
        , engine(false)
        , driveTrain(false)
        , wheels(false)
    {

    }

private:
    int id;
    bool engine;
    bool driveTrain;
    bool wheels;  

public:
    int getId() 
    { 
        return this->id; 
    }

    bool engineInstalled() 
    { 
        return this->engine; 
    } 
	void addEngine() 
    { 
        this->engine = true; 
    }  
	
    bool driveTrainInstalled() 
    { 
        return this->driveTrain; 
    } 
	void addDriveTrain() 
    { 
        this->driveTrain = true; 
    }  
	
    bool wheelsInstalled() 
    { 
        return this->wheels; 
    }  
	void addWheels() 
    { 
        this->wheels = true; 
    }  
	
	friend ostream& operator<<(ostream& os, const Car& c)
	{
		return os << "Car: id: " << c.id << "- [ "
			      << "engine: " << c.engine
			      << ", driveTrain: " << c.driveTrain
			      << ", wheels: " << c.wheels << " ]";
	}
};
typedef CountedPtr< TQueue<Car> > CarQueue;

class ChassisBuilder : public Runnable
{
public:  
	ChassisBuilder(CarQueue& cq) 
        : carQueue(cq)
        , counter(0) 
    {

    }  

private:
	CarQueue carQueue; 
	int	counter;

public:
	virtual void run()
	{
		try 
		{
			while (!Thread::interrupted()) 
			{
				Thread::sleep(100); 
				// Make chassis:  
				Car c(counter++);
				cout << c << endl;  
				// Insert into queue
				carQueue->put(c);
			}
		} 
		catch(Interrupted_Exception& e) 
        { 
            std::cerr << "ChassisBuilder exception: " << e.what() << '\n';
        }
		cout << "ChassisBuilder off !" << endl;  
	}  
};  


class Cradle
{
public:  
	Cradle()  
        : workCondition(workLock)
        , readyCondition(readyLock)
        , occupied(false)
        , engineBotHired(true)
        , wheelBotHired(true)
        , driveTrainBotHired(true)
	{  
        
	} 

private:
	Car c;
	Mutex workLock;
    Mutex readyLock;
	Condition workCondition;
    Condition readyCondition;

    bool occupied;

	bool engineBotHired;
    bool wheelBotHired;
    bool driveTrainBotHired;

public: 
	void insertCar(Car chassis) 
	{
		this->c = chassis;
		this->occupied = true;
	}

	Car getCar() 
	{
		if (!this->occupied)
		{  
			cerr << "Cradle::getCar: No Car in Cradle for getCar() !" << endl;  
			return Car();
		} 
		this->occupied = false; 
		return c;  
	}  

	Car* operator->() 
    { 
        return &this->c; 
    }

	void offerEngineBotServices()
	{
		Guard<Mutex> g(this->workLock);
		while (this->engineBotHired)  
			this->workCondition.wait();

		this->engineBotHired = true;
	}

	void offerWheelBotServices() 
	{  
		Guard<Mutex> g(this->workLock);
		while(this->wheelBotHired)  
			this->workCondition.wait();

		this->wheelBotHired = true;
	}  

	void offerDriveTrainBotServices()
	{  
		Guard<Mutex> g(this->workLock);  
		while(this->driveTrainBotHired)  
			this->workCondition.wait();  

		this->driveTrainBotHired = true; 
	}  

	void startWork()
	{  
		Guard<Mutex> g(this->workLock);  
		this->engineBotHired = false;  
		this->wheelBotHired	= false;  
		this->driveTrainBotHired = false;  
		this->workCondition.broadcast(); //3 signal()
		//workCondition.signal(); 
		//workCondition.signal(); 
		//workCondition.signal(); 
	} 

	void taskFinished() 
	{  
		Guard<Mutex> g(this->readyLock); 

		this->readyCondition.signal();
	}  

	void waitUntilWorkFinished()
	{  
		Guard<Mutex> g(this->readyLock);
		while(!(this->c.engineInstalled() && this->c.driveTrainInstalled() && this->c.wheelsInstalled()))  
			this->readyCondition.wait();
	}
};
typedef CountedPtr<Cradle> CradlePtr;  


class Director : public Runnable
{  
public:
	Director(CarQueue& cq, CarQueue& fq, CradlePtr cr)  
		: chassisQueue(cq)
        , finishingQueue(fq)
        , cradle(cr) 
    {

    }  

private:        
	CarQueue chassisQueue;
    CarQueue finishingQueue;
	CradlePtr cradle;

public:
	virtual void run() 
	{
		try
		{
			while (!Thread::interrupted())
			{
				// Blocks until chassis is available:  
				this->cradle->insertCar(this->chassisQueue->get());
				// Notify robots car is ready for work
				this->cradle->startWork();
				// Wait until work completes  
				this->cradle->waitUntilWorkFinished();
				// Put car into queue for further work  
				this->finishingQueue->put(this->cradle->getCar()); 
			}  
		}
		catch(Interrupted_Exception& e) 
        { 
            std::cerr << "Director exception: " << e.what() << '\n';
        }
		cout << "Director off !" << endl;  
	}  
};

class EngineRobot : public Runnable
{  
public:  
	EngineRobot(CradlePtr cr) 
        : cradle(cr) 
    {

    }  

private:     
	CradlePtr cradle;

public:
	virtual void run() 
	{  
		try
		{  
			while (!Thread::interrupted())
			{
				// Blocks until job is offered/accepted:  
				this->cradle->offerEngineBotServices();
				cout << "EngineRobot: Installing engine !" << endl; 
				(*this->cradle)->addEngine();
				this->cradle->taskFinished();  
			}  
		} 
		catch(Interrupted_Exception& e) 
        { 
            std::cerr << "EngineRobot exception: " << e.what() << '\n';
        } 
		cout << "EngineRobot off !" << endl;
	}
};


class DriveTrainRobot : public Runnable
{  
public:  
	DriveTrainRobot(CradlePtr cr) 
        : cradle(cr) 
    {

    }

private:         
	CradlePtr cradle;  

public:
	virtual void run() 
	{  
		try 
		{  
			while (!Thread::interrupted())
			{  
				// Blocks until job is offered/accepted:  
				this->cradle->offerDriveTrainBotServices();
				cout << "DriveTrainRobot: Installing DriveTrain !" << endl;
				(*this->cradle.operator ->())->addDriveTrain(); 
				this->cradle->taskFinished();  
			}  
		} 
		catch(Interrupted_Exception& e) 
        {
            std::cerr << "DriveTrainRobot exception: " << e.what() << '\n';
        } 
		cout << "DriveTrainRobot off !" << endl;  
	}  
};  


class WheelRobot : public Runnable
{  
public:  
	WheelRobot(CradlePtr cr) 
        : cradle(cr) 
    {

    }  

private:         
	CradlePtr cradle;  

public:
	virtual void run() 
	{  
		try
		{  
			while (!Thread::interrupted()) 
			{  
				// Blocks until job is offered/accepted:  
				this->cradle->offerWheelBotServices();  
				cout << "WheelRobot: Installing Wheels !" << endl;
				(*this->cradle)->addWheels();  
				this->cradle->taskFinished();  
			}  
		} 
		catch(Interrupted_Exception& e) 
        { 
            std::cerr << "WheelRobot exception: " << e.what() << '\n';
        }  
		cout << "WheelRobot off !" << endl;  
	}
};


class Reporter : public Runnable
{  
public:  
	Reporter(CarQueue& cq) 
        : carQueue(cq) 
    {

    }  

private:  
	CarQueue carQueue;  

public:
	virtual void run() 
	{
		try 
		{  
			while (!Thread::interrupted()) 
			{  
				cout << carQueue->get() << endl;  
			}  
		} 
		catch(Interrupted_Exception& e) 
        { 
            std::cerr << "Reporter exception: " << e.what() << '\n';
        }  
		cout << "Reporter off !" << endl;  
	}  
};


int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 012 Queue ********** " << endl;
    {
        try
        {
            CarQueue chassisQueue(new TQueue<Car>);
            CarQueue finishingQueue(new TQueue<Car>); 
            CradlePtr cradle(new Cradle);  
            
            ThreadedExecutor assemblyLine;  
            assemblyLine.execute(new EngineRobot(cradle));  
            assemblyLine.execute(new DriveTrainRobot(cradle));  
            assemblyLine.execute(new WheelRobot(cradle));  
            assemblyLine.execute(new Director(chassisQueue, finishingQueue, cradle));
            assemblyLine.execute(new Reporter(finishingQueue));  
            // Start everything running by producing chassis:  
            assemblyLine.execute(new ChassisBuilder(chassisQueue));  

            cin.get();  
            assemblyLine.interrupt();  
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << "Main: " << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 012 Queue ********** " << endl;

    cin.get();
	return 0;
}