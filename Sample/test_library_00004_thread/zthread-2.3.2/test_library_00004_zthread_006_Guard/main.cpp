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
#include "zthread/Cancelable.h"
#include "zthread/CountedPtr.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"
using namespace ZThread;


class Generator : public Cancelable
{
public:
    Generator()
        : canceled(false)
    {

    }

public:
    virtual int nextValue() = 0;

    virtual void cancel()
    {
        this->canceled = true;
    }

    virtual bool isCanceled()
    {
        return this->canceled;
    }

private:
    bool canceled;
};

class EvenGenerator : public Generator
{
public: 
    EvenGenerator()
        : currentEvenValue(0)
    {

    }
    ~EvenGenerator()
    {
        cout << "~EvenGenerator()" << endl;
    }

private:
    unsigned int currentEvenValue;
    Mutex mutex;

public:
    virtual int nextValue()
    {
        Guard<Mutex> lock(this->mutex);
        this->currentEvenValue ++;
        Thread::yield();
        this->currentEvenValue ++;

        return this->currentEvenValue;
    }
};

class EvenChecker : public Runnable
{
public:
    EvenChecker(const CountedPtr<Generator>& g, int id)
        : generator(g)
        , _id(id)
    {

    }
    ~EvenChecker()
    {
        cout << "~EvenChecker(), id: " << this->_id <<endl;
    }

public: 
    static Mutex s_Mutex;

private:
    CountedPtr<Generator> generator;
    int _id;

public:
    virtual void run()
    {
        while (!this->generator->isCanceled())
        {
            int val = this->generator->nextValue();
            if (val % 2 != 0)
            {
                {
                    Guard<Mutex> lock(EvenChecker::s_Mutex);
                    cout << "id: " << this->_id << ", val: " << val << " is not even !" <<endl;
                }

                this->generator->cancel();
            }
            else 
            {
                {
                    Guard<Mutex> lock(EvenChecker::s_Mutex);
                    cout << "id: " << this->_id << ", val: " << val << " is even !" <<endl;
                }

                Thread::sleep(200);
            }
        }
    }

public:
    template<typename GenType>
    static void test(int n = 5)
    {
        try
        {
            ThreadedExecutor executor;
            CountedPtr<Generator> generator(new GenType);
            for (int i = 0; i < n; i++)
			{
				executor.execute(new EvenChecker(generator, i));
			}
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << e.what() << endl;
        }
    }
};
Mutex EvenChecker::s_Mutex;


int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 006 Guard ********** " << endl;
    {
        try
        {
            EvenChecker::test<EvenGenerator>();
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 006 Guard ********** " << endl;

    cin.get();
	return 0;
}