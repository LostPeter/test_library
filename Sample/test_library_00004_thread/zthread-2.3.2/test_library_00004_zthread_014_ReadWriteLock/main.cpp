/****************************************************************************
* test_library - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-29
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include <iostream>
#include <sstream>
#include <time.h>
#include <vector>
using namespace std;

#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "zthread/ThreadedExecutor.h"
#include "zthread/Exceptions.h"
#include "zthread/CountedPtr.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"
#include "zthread/Condition.h"
#include "zthread/ReadWriteLock.h"
#include "zthread/FairReadWriteLock.h"
#include "zthread/BiasedReadWriteLock.h"
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


class ReadThread : public Runnable
{
public:
	ReadThread(CountedPtr<ReadWriteLock>& _lock, CountedPtr<Display>& disp, int _id = -1) 
        : lock(_lock)
        , display(disp)
        , id(_id)
    {

    }
	
private:
	CountedPtr<ReadWriteLock> lock;
	CountedPtr<Display>	display;
	int id;

public:    
	virtual void run()
	{
		try
		{
			while (!Thread::interrupted())
			{
				this->lock->getReadLock().acquire();
				
				output("ReadThread::Reading...");
				Thread::sleep(100);
				
				this->lock->getReadLock().release();
				
				Thread::sleep(500);
			}
		}
		catch (Interrupted_Exception& e)
		{
			output(e.what());
		}
	}
	
	void output(string str)
	{
		ostringstream os;
		os << *this << " : " << str <<endl;
		this->display->Output(os);
	}
	friend ostream& operator<< (ostream& os, const ReadThread& readThread)
	{
		return os << readThread.id;
	}
};


class WriteThread : public Runnable
{
public:
	WriteThread(CountedPtr<ReadWriteLock>& _lock, CountedPtr<Display>& disp, int _id = -1) 
        : lock(_lock)
        , display(disp)
        , id(_id)
    {

    }
	
private:
	CountedPtr<ReadWriteLock> lock;
	CountedPtr<Display>	display;
	int id;

public:
	virtual void run()
	{
		try
		{
			while (!Thread::interrupted())
			{
				this->lock->getWriteLock().acquire();
				
				output("WriteThread::Wirtting...");
				Thread::sleep(100);
				
				this->lock->getWriteLock().release();
				
				Thread::sleep(500);
			}
		}
		catch (Interrupted_Exception& e)
		{
			output(e.what());
		}
	}
	
	void output(string str)
	{
		ostringstream os;
		os << *this << " : " << str <<endl;
		this->display->Output(os);
	}
	friend ostream& operator<< (ostream& os, const WriteThread& writeThread)
	{
		return os << writeThread.id;
	}
};

int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 014 ReadWriteLock ********** " << endl;
    {
        try
        {
            //FairReadWriteLock FIFO lock
            CountedPtr<ReadWriteLock> ReadWrite(new FairReadWriteLock());
            
            //BiasedReadWriteLock Write priority lock
            //CountedPtr<ReadWriteLock> ReadWrite( new BiasedReadWriteLock());
                
            CountedPtr<Display> disp(new Display());
            
            ThreadedExecutor executor;
            for (int i = 0; i < 5; i++)
            {
                executor.execute(new ReadThread(ReadWrite, disp, i));
            }
            
            for (int i = 0; i < 5; i++)
            {
                executor.execute(new WriteThread(ReadWrite, disp, i));
            }
            
            cin.get();
            executor.interrupt();
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 014 ReadWriteLock ********** " << endl;

    cin.get();
	return 0;
}