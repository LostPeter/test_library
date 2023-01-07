/****************************************************************************
* test_library - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-29
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include <iostream>
#include <fstream>
using namespace std;

#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "zthread/Exceptions.h"
#include "zthread/CountedPtr.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"
using namespace ZThread;

class Counter
{
public:
    Counter(int id)
        : _id(id)
        , _num(0)
    {

    }

private:
    int _id;
    int _num;

    Mutex mutex;

public:
    void increment(int tID)
    {
        this->_num ++;

        Guard<Mutex> lock(this->mutex);
        cout << "id: " << this->_id << ", num: " << this->_num << ", tid: " << tID << endl;
    }
};

class Incrementer : public Runnable
{
public:
    Incrementer(const CountedPtr<Counter>& c, int id)
        : counter(c)
        , _id(id)
    {

    }

private:
    CountedPtr<Counter> counter;
    int _id;

public: 
    virtual void run() 
    {
        for (int i = 100; i > 0; i--)
        {
            this->counter->increment(this->_id);
        }
    }
};

int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 005 CountedPtr ********** " << endl;
    {
        CountedPtr<Counter> counter(new Counter(0));
        try
        {
            Thread t0(new Incrementer(counter, 0));
            Thread t1(new Incrementer(counter, 1));

            t0.wait();
            t1.wait();
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 005 CountedPtr ********** " << endl;

    cin.get();
	return 0;
}