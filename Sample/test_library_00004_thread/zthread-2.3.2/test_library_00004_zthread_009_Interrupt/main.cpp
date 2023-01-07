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
#include "zthread/Exceptions.h"
#include "zthread/Cancelable.h"
#include "zthread/CountedPtr.h"
#include "zthread/Mutex.h"
#include "zthread/FastMutex.h"
#include "zthread/Guard.h"
using namespace ZThread;

bool g_isComplete[2] = 
{
    false,
    false,
};
void setIsComplete(int index)
{
    g_isComplete[index] = true;
}
bool IsComplete(int index)
{
    return g_isComplete[index];
}

const double PI = 3.1415926;
const double e = 2.7182818;
class NeedCleanup
{
public:
	NeedCleanup(int idn = 0) : id(idn)
	{
		cout << " NeedCleanup : " << id << endl;
	}

	~NeedCleanup()
	{
		cout << " ~NeedCleanup : " << id << endl;
	}
private:
	int id;
};

class Blocked : public  Runnable
{
public:
	Blocked() 
        : d(0.0)
    {

    }

private:
	volatile double d;


public:	
	virtual void run()
	{
		try
		{
			while (!Thread::interrupted())
			{
				NeedCleanup(1);
				cout << "Blocked::run(): Sleep !" <<endl;

				Thread::sleep(5000);
			}
			cout << "Blocked::run(): Exit via while() test !" <<endl;
		}
		catch (Interrupted_Exception& e)
		{
			cout << "Blocked::run(): Exit via Interrupted_Exception test !" <<endl;
			cerr << e.what() <<endl;
		}

        setIsComplete(0);
	}

};

class NonBlocked : public  Runnable
{
public:
	NonBlocked() 
        : d(0.0)
    {

    }

private:
	volatile double d;

public:	
	virtual void run()
	{
		try
		{
			while (!Thread::interrupted())
			{
				NeedCleanup(2);
				cout <<"NonBlocked::run: Calculating !" <<endl;
				for (int i= 0; i <100000; i++)
				{
					d = d + (PI + e) / (double)i;
				}
			}
			cout << "NonBlocked::run: Exit via while() test !" <<endl;
		}
		catch (Interrupted_Exception& e)
		{
			cout << "NonBlocked::run: Exit via Interrupted_Exception test !" <<endl;
			cerr << e.what() <<endl;
		}

        setIsComplete(1);
	}

};

int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 009 Interrupt ********** " << endl;
    {
        cout << "***** <1> Test Blocked Interrupt *****" << endl;
        {
            Thread t(new Blocked);	
		    Thread::sleep(2000);

		    t.interrupt();		
        }
        cout << "***** <1> Test Blocked Interrupt *****" << endl;

        while (!IsComplete(0))
        {

        }

        cout << "***** <2> Test NonBlocked Interrupt *****" << endl;
        {
            Thread t(new NonBlocked);	
		    Thread::sleep(5000);

		    t.interrupt();		
        }
        cout << "***** <2> Test NonBlocked Interrupt *****" << endl;

        while (!IsComplete(1))
        {

        }
    }
    cout << "********** Test zthread 009 Interrupt ********** " << endl;

    cin.get();
	return 0;
}