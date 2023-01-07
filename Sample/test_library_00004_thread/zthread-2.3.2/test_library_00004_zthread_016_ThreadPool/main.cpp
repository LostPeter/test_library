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
#include "zthread/Semaphore.h"
#include "zthread/CountingSemaphore.h"
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


class Reads_Semaphore : public Runnable
{
public:
	Reads_Semaphore(CountedPtr<Semaphore>/*CountedPtr<CountingSemaphore>*/& aSemaphore, CountedPtr<Display>& disp, int _id = -1)
        : id(_id)
        , pSemaphore(aSemaphore)
        , pDisplay(disp)
    {

    }

private:
	int	id;
	CountedPtr<Semaphore> pSemaphore;
	//CountedPtr<CountingSemaphore> pSemaphore;
	CountedPtr<Display> pDisplay; 
	
public: 
	virtual void run()
	{
		try
		{
			while (!Thread::interrupted())
			{
				pSemaphore->acquire();
				
				output("Reads_Semaphore::Reading...");
				Thread::sleep(1000);
				
				pSemaphore->release();
			}
		}
		catch (Interrupted_Exception& e)
		{
			cerr << "Reads_Semaphore exception: " << e.what() << endl;
		}
		catch (InvalidOp_Exception& e)
		{
			cerr << "Reads_Semaphore exception: " << e.what() << endl;
			return;
		}
	}

	void output(string str)
	{
		ostringstream os;
		os << *this << " : " << str <<endl;
		pDisplay->Output(os);
	}
	friend ostream& operator<< (ostream& os, Reads_Semaphore& reads)
	{
		return os << "id: " << reads.id << " Count = " << reads.pSemaphore->count();
	}
};


class Reads_CountingSemaphore : public Runnable
{
public:
	Reads_CountingSemaphore(CountedPtr<CountingSemaphore>& aSemaphore, CountedPtr<Display>& disp, int _id = -1)
        : id(_id)
        , pSemaphore(aSemaphore)
        , pDisplay(disp)
    {

    }

private:
	int	id;
	CountedPtr<CountingSemaphore> pSemaphore;
	CountedPtr<Display> pDisplay; 
	
public: 
	virtual void run()
	{
		try
		{
			while (!Thread::interrupted())
			{
				pSemaphore->acquire();
				
				output("Reads_CountingSemaphore::Reading...");
				Thread::sleep(1000);
				
				pSemaphore->release();
			}
		}
		catch (Interrupted_Exception& e)
		{
			cerr << "Reads_CountingSemaphore exception: " << e.what() << endl;
		}
		catch (InvalidOp_Exception& e)
		{
			cerr << "Reads_CountingSemaphore exception: " << e.what() << endl;
			return;
		}
	}

	void output(string str)
	{
		ostringstream os;
		os << *this << " : " << str <<endl;
		pDisplay->Output(os);
	}
	friend ostream& operator<< (ostream& os, Reads_CountingSemaphore& reads)
	{
		return os << "id: " << reads.id << " Count = " << reads.pSemaphore->count();
	}
};


int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 015 Semaphore ********** " << endl;
    {
        try
        {
            //count = 5，InvalidOp_Exception
            CountedPtr<Semaphore> pSemaphore(new Semaphore(5, 2));

            //no InvalidOp_Exception
            CountedPtr<CountingSemaphore>pCountingSemaphore(new CountingSemaphore(5));
            
            CountedPtr<Display> display(new Display);
            ThreadedExecutor executor;
            for (int i = 0; i < 20; i ++)
            {
                //这样测试是为了有在创建线程之前有线程执行，并且能促使信号量增1
                executor.wait(1500);
                
                if (i % 2 == 0)
                    executor.execute(new Reads_Semaphore(pSemaphore, display, i));
                else
                    executor.execute(new Reads_CountingSemaphore(pCountingSemaphore, display, i));
            }
            
            cin.get();
            executor.interrupt();
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 015 Semaphore ********** " << endl;
    
    cin.get();
	return 0;
}