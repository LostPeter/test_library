// author: LostPeter
// time:   2022-10-29

#include <iostream>
#include <fstream>
using namespace std;


#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "zthread/Exceptions.h"
#include "zthread/PoolExecutor.h"
#include "zthread/Priority.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"
using namespace ZThread;


const double pi = 3.141592653589793;
const double e = 2.718281828459;

class Counter : public Runnable
{
public:
    Counter(int id, int num)
        : _id(id)
        , _num(num)
        , d(0)
    {

    }
    ~Counter()
    {

    }

public: 
    static Mutex s_lock;

private:
    int _id;
    int _num;
    volatile double d;

public:  
    friend ostream& operator << (ostream& os, const Counter& counter)
	{
        Guard<Mutex> lock(Counter::s_lock);

		return os << "##### id: " << counter._id
                  << ", num: " << counter._num
                  << ", priority: " << Thread().getPriority() 
                  << endl;
	}

    virtual void run()
    {
        while (true)
        {
            for (int i = 0; i < 10000; i++)
			{
				d = d + (pi + e) /double(i);
			}
			cout << *this;

			if (--this->_num == 0)
                return;
        }
        
    }
};
Mutex Counter::s_lock;


int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 004 Priority ********** " << endl;
    {
        try
        {
            Thread tHigh(new Counter(0, 5));
            tHigh.setPriority(Priority::High);

            for (int i = 0; i < 5; i++)
            {
                Thread tLow(new Counter((i + 1), 5));
                tLow.setPriority(Priority::Low);
            }

            time_t start = time(NULL);
            time_t diff = time(NULL) - start;
            while (diff < 20)
            {
                diff = time(NULL) - start;

                //cout << "Time: " << diff << endl;
            }
            cout << "Time finish: " << diff << endl;
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 004 Priority ********** " << endl;

    cin.get();
	return 0;
}