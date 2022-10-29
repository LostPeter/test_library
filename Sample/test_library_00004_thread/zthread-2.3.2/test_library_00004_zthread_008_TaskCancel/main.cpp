// author: LostPeter
// time:   2022-10-29

#include <iostream>
#include <sstream>
#include <time.h>
#include <vector>
using namespace std;

#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "zthread/ThreadedExecutor.h"
#include "zthread/Exceptions.h"
#include "zthread/Cancelable.h"
#include "zthread/CountedPtr.h"
#include "zthread/Mutex.h"
#include "zthread/FastMutex.h"
#include "zthread/Guard.h"
using namespace ZThread;

class Display
{
public:
    Display()
    {

    }

private:
    Mutex mutex;

public:
    void Output(ostringstream& os)
    {
        Guard<Mutex> lock(this->mutex);
        cout << os.str();
    }
};

class Counter : public Cancelable
{
public:
    Counter(int id)
        : _id(id)
        , _num(0)
        , _pause(false)
        , _canceled(false)
    {

    }

private:
    int _id;
    int _num;
    bool _pause;
    bool _canceled;
    FastMutex mutex;

public:
    int value()
    {
        Guard<FastMutex> lock(this->mutex);
        return this->_num;
    }

    int increment()
    {
        Guard<FastMutex> lock(this->mutex);

        int temp = this->_num;
        if (rand() % 2 == 0)
        {
            Thread::yield();
        }
        return this->_num = ++ temp;
    }

    void pause()
    {
        Guard<FastMutex> lock(this->mutex);
        this->_pause = true;
    }
    bool isPaused()
    {
        Guard<FastMutex> lock(this->mutex);
        return this->_pause;
    }

    virtual void cancel()
    {
        Guard<FastMutex> lock(this->mutex);
        this->_canceled = true;
    }
    virtual bool isCanceled()
    {
        Guard<FastMutex> lock(this->mutex);
        return this->_canceled;
    }
};


class Entrance : public Runnable
{
public:
    Entrance(int _id,
            CountedPtr<Counter>& c,
            CountedPtr<Display>& d)
        : id(_id)
        , pCounter(c)
        , pDisplay(d)
        , num(0)
        , waittingForCancel(false)
    {

    }

private:
    CountedPtr<Counter> pCounter;
    CountedPtr<Display> pDisplay;

    int id;
    int num;

    bool waittingForCancel;

public:
    int GetValue()
	{
		while (this->pCounter->isPaused() && !this->waittingForCancel)	//确保当前计数器和总计数器都已经退出
			Thread::sleep(100);
		
		return this->num;
	}

    friend ostream& operator<< (ostream& os, const Entrance& e)
	{
		return os << "Entrance: id: "<< e.id << ", num: " << e.num;
	}

    virtual void run()
    {
        try 
        {
            while (!this->pCounter->isPaused())
            {
                this->num ++;
                {
                    ostringstream os;
                    os << *this << ", Total: " << this->pCounter->increment() << endl;
                    this->pDisplay->Output(os);
                }
                Thread::sleep(100);
            }
            
            this->waittingForCancel = true;
            while (!this->pCounter->isCanceled())
            {
                Thread::sleep(100);
            }
            
            ostringstream os;
            os << "Terminating: " << *this << endl;
            this->pDisplay->Output(os);
        }
        catch (Synchronization_Exception* e)
		{
			cerr << "Exception: "<< e->what() <<endl;	
		}
    }
};

int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 008 TaskCancel ********** " << endl;
    {
        srand(time(0));

        CountedPtr<Counter> pCounter(new Counter(0));
        vector<Entrance*> v;

        CountedPtr<Display> pDisplay(new Display());

        try
        {
            ThreadedExecutor excutor;

            for (int i = 0; i < 5; i++)
            {
                Entrance* pTask = new Entrance(i, pCounter, pDisplay);
                excutor.execute(pTask);
                v.push_back(pTask);
            }

            cin.get();
            pCounter->pause();

            int nSum = 0;
            vector<Entrance*>::iterator it = v.begin();
            while (it != v.end())
            {
                nSum += (*it)->GetValue();
                ++it;
            }

            ostringstream os;
            os << "Counter paused !" << endl
               << "Total: " << pCounter->value() << endl 
               << "Sum of Entrance: " << nSum << endl;
            pDisplay->Output(os);

            pCounter->cancel();
            cin.get();
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 008 TaskCancel ********** " << endl;

    cin.get();
	return 0;
}