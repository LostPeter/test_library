/****************************************************************************
* test_library - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-29
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include <iostream>
using namespace std;

#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "zthread/ThreadedExecutor.h"
#include "zthread/Exceptions.h"
#include "zthread/CountedPtr.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"
#include "zthread/Condition.h"
using namespace ZThread;

class Car
{
public:
    Car()
        : condition(mutex)
        , isWaxon(false)
    {

    }

private:
    Mutex mutex;
    Condition condition;
    bool isWaxon;

public:
    void Waxed()
    {
        Guard<Mutex> lock(this->mutex);
        this->isWaxon = true;
        this->condition.signal();
    }

    void Buffed()
    {
        Guard<Mutex> lock(this->mutex);
        this->isWaxon = false;
        this->condition.signal();
    }

    void WaitForWaxing()
    {
        Guard<Mutex> lock(this->mutex);
        while (this->isWaxon == false)
        {
            this->condition.wait();
        }
    }

    void WaitForBuffing()
    {
        Guard<Mutex> lock(this->mutex);
        while (this->isWaxon == true)
        {
            this->condition.wait();
        }
    }
};

class WaxOn : public Runnable
{
public:
    WaxOn(CountedPtr<Car>& car)
        : pCar(car)
    {

    }

private:
    CountedPtr<Car> pCar;

public:
    virtual void run()
    {
        try
        {
            while (!Thread::interrupted())
            {
                cout << "Wax on !" << endl;
                Thread::sleep(200);
                pCar->Waxed();
                pCar->WaitForBuffing();
            }
            
        }
        catch(Interrupted_Exception& e)
        {
            std::cerr << "Car wax on exception: " << e.what() << '\n';
        }
    }
};

class WaxOff : public Runnable
{
public:
    WaxOff(CountedPtr<Car>& car)
        : pCar(car)
    {
        
    }

private:
    CountedPtr<Car> pCar;

public:
    virtual void run()
    {
        try
        {
            while (!Thread::interrupted())
            {
                pCar->WaitForWaxing();
                Thread::sleep(200);
                cout << "Wax off !" << endl;
                pCar->Buffed();
            }
        }
        catch(Interrupted_Exception& e)
        {
            std::cerr << "Car wax off exception: " << e.what() << '\n';
        }
    }
};


int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 010 Condition ********** " << endl;
    {
        try
        {
            CountedPtr<Car> car(new Car());
            ThreadedExecutor excutor;
            excutor.execute(new WaxOff(car));
            excutor.execute(new WaxOn(car));
            cin.get();
            excutor.interrupt();
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << "Main exception: " << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 010 Condition ********** " << endl;
    
    cin.get();
	return 0;
}