// author: LostPeter
// time:   2022-10-29

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


class Jammer : public Runnable
{
public:
    Jammer()
        : condition(mutex)
        , isGotButteredToast(false)
        , jammed(0)
    {

    }

private:
    Mutex mutex;
    Condition condition;
    bool isGotButteredToast;

    int jammed;

public:
    void moreButteredToastReady()
    {
        Guard<Mutex> lock(this->mutex);

        this->isGotButteredToast = true;
        this->condition.signal();    
    }

    virtual void run()
    {
        try
        {
            while (!Thread::interrupted())
            {
                {
                    Guard<Mutex> lock(this->mutex);
                    while (this->isGotButteredToast == false)
                    {
                        this->condition.wait();
                    }
                    ++ this->jammed;
                }

                cout << "Putting jam on toast: " << this->jammed << endl;

                {
                    Guard<Mutex> lock(this->mutex);

                    this->isGotButteredToast = false;
                }
            }
        }
        catch(Interrupted_Exception& e)
        {
            std::cerr << "Jammer exception: " << e.what() << '\n';
        }
    }
};

class Butterer : public Runnable
{
public:
    Butterer(CountedPtr<Jammer>& j)
        : condition(mutex)
        , jammer(j)
        , isGotToast(false)
        , buttered(0)
    {
        
    }

private:
    Mutex mutex;
    Condition condition;
    CountedPtr<Jammer> jammer;
    bool isGotToast;
    int buttered;

public:
    void moreToastReady()
    {
        Guard<Mutex> lock(this->mutex);

        this->isGotToast = true;
        this->condition.signal();    
    }

    virtual void run()
    {
        try
        {
            while (!Thread::interrupted())
            {
                {
                    Guard<Mutex> lock(this->mutex);
                    while (this->isGotToast == false)
                    {
                        this->condition.wait();
                    }
                    ++ this->buttered;
                }

                cout << "Buttering toast: " << this->buttered << endl;
                this->jammer->moreButteredToastReady();

                {
                    Guard<Mutex> lock(this->mutex);

                    this->isGotToast = false;
                }
            }
        }
        catch(Interrupted_Exception& e)
        {
            std::cerr << "Butterer exception: " << e.what() << '\n';
        }
    }
};

class Toast : public Runnable
{
public:
    Toast(CountedPtr<Butterer>& b)
        : butterer(b)
        , toasted(0)
    {
        
    }

private:
    CountedPtr<Butterer> butterer;
    int toasted;

public:
    virtual void run()
    {
        try
        {
            while (!Thread::interrupted())
            {
                Thread::sleep(1000);

                cout << "A new Toast: " << ++ this->toasted <<endl;
				
				this->butterer->moreToastReady();
            }
        }
        catch(Interrupted_Exception& e)
        {
            std::cerr << "Toast exception: " << e.what() << '\n';
        }
    }
};


int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 011 ProducerConsumer ********** " << endl;
    {
        try
        {
            CountedPtr<Jammer> jammer(new Jammer());
            CountedPtr<Butterer> butterer(new Butterer(jammer));
            CountedPtr<Toast> toast(new Toast(butterer));

            ThreadedExecutor executor;
            executor.execute(toast);
            executor.execute(jammer);
            executor.execute(butterer);
            cin.get();
            executor.interrupt();
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << "Main exception: " << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 011 ProducerConsumer ********** " << endl;

    cin.get();
	return 0;
}