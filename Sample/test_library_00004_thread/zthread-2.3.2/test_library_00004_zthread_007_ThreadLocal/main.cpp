// author: LostPeter
// time:   2022-10-29

#include <iostream>
#include <fstream>
using namespace std;

#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "zthread/ThreadLocal.h"
#include "zthread/ThreadedExecutor.h"
#include "zthread/Exceptions.h"
#include "zthread/Cancelable.h"
#include "zthread/CountedPtr.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"
using namespace ZThread;

class ThreadVariables : public Cancelable
{
public:
    ThreadVariables()
        : canceled(false)
    {
        this->tValue.set(0);
    }

private:
    ThreadLocal<int> tValue;
    bool canceled;
    Mutex mutex;

public:
    int get()
    {
        return this->tValue.get();
    }
    void increment()
    {
        this->tValue.set(this->tValue.get() + 1);
    }

    virtual void cancel()
    {
        Guard<Mutex> lock(this->mutex);
        this->canceled = true;
    }

    virtual bool isCanceled()
    {
        Guard<Mutex> lock(this->mutex);
        return this->canceled;
    }
};

class Accessor : public Runnable
{
public:
    Accessor(const CountedPtr<ThreadVariables>& tl, int id)
        : tValue(tl)
        , _id(id)
    {

    }

public:
    static Mutex s_Mutex; 

private:
    int _id;
    CountedPtr<ThreadVariables> tValue;

public:
    friend ostream& operator <<(ostream& os, Accessor& a)
	{
		Guard<Mutex> lock(Accessor::s_Mutex);
        
		return os << "###### id: " << a._id << ", value: " << a.tValue->get() << endl;
	}

    virtual void run()
    {
        while (!this->tValue->isCanceled())
		{
			this->tValue->increment();

			cout << *this;
		}
    }

};
Mutex Accessor::s_Mutex;


int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 007 ThreadLocal ********** " << endl;
    {
        try
        {
            CountedPtr<ThreadVariables> tValue(new ThreadVariables);

            ThreadedExecutor executor;
            for (int i = 0; i < 5; i++)
            {
                executor.execute(new Accessor(tValue, i));
            }

            cin.get();
            tValue->cancel();
        }   
        catch(Synchronization_Exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 007 ThreadLocal ********** " << endl;

    cin.get();
	return 0;
}