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


class Chopstick
{
public:
	Chopstick() 
        : condition(mutex)
        , taken(false)
    {

    }
	
private:
	Mutex mutex;
	Condition condition;
	bool taken;

public: 
	void Take()
	{
		Guard<Mutex> lock(this->mutex);
		
		while (this->taken == true)
			this->condition.wait();
		this->taken = true;
	}
	
	void Drop()
	{
		Guard<Mutex> lock(this->mutex);
		
		this->taken = false;
		this->condition.signal(); 
	}
};


class Philosopher : public Runnable
{
public:
	Philosopher(Chopstick& l, Chopstick& r, int _id, int ponder, CountedPtr<Display>& disp)
        : left(l)
        , right(r)
        , id(_id)
        , ponderFactor(ponder)
        , display(disp)
	{
	
	}
	
private:
	Chopstick& left;
	Chopstick& right;
	
	int id;
	int ponderFactor;
	
	CountedPtr<Display> display;

public: 
	virtual void run()
	{
		try
		{
			while (!Thread::interrupted())
			{
				output("Philosopher::Think !");
				Thread::sleep(randSleepTime());
				
				output("Philosopher::Grabbing right !");
				this->right.Take();
				
				output("Philosopher::Grabbing left !");
				this->left.Take();
				
				output("Philosopher::Eatting !");
				Thread::sleep(randSleepTime());
				
				this->right.Drop();
				this->left.Drop();
			}
		}
		catch (Interrupted_Exception& e)
		{
			cerr << "Philosopher exception: " << e.what() <<endl;
		}
	}
	friend ostream& operator <<(ostream& os, const Philosopher& p)
	{
		return os << ", Philosopher: " << p.id;
	}
	
private:
	int randSleepTime()
	{
		if (this->ponderFactor == 0)	
            return 0;
		
		return rand() / (RAND_MAX / this->ponderFactor) * 250;
	}
	
	void output(string str)
	{
		ostringstream os;
		os << *this << " : " << str <<endl;
		
		this->display->Output(os);
	}
};


int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 013 DeadLock ********** " << endl;
    {
        srand((unsigned int)time(0));
        const int count = 5;
        int pender =  0;

        try
        {
            CountedPtr<Display> disp(new Display);
            ThreadedExecutor executor;
            
            Chopstick chopstick[count];
            for (int i = 0; i < count; i ++)
            {
                //executor.execute(new Philosopher(chopstick[i], chopstick[(i + 1) % count], i, pender, disp));
                
                if (i < count - 1)
                    executor.execute(new Philosopher(chopstick[i], chopstick[(i + 1)], i, pender, disp));
                else
                    executor.execute(new Philosopher(chopstick[0], chopstick[i], i, pender, disp));
            }
            cin.get();
            executor.interrupt();
            executor.wait();
            
            cin.get();
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << "Main: " << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 013 DeadLock ********** " << endl;

    cin.get();
	return 0;
}