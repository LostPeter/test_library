// author: LostPeter
// time:   2022-10-29

#include <iostream>
using namespace std;

#include "zthread/Runnable.h"
#include "zthread/PoolExecutor.h"
#include "zthread/ThreadedExecutor.h"
#include "zthread/ConcurrentExecutor.h"
#include "zthread/SynchronousExecutor.h"
#include "zthread/Mutex.h"
#include "zthread/Guard.h"
using namespace ZThread;

//0> ThreadedExecutor:     为每个任务创建一个线程，过多会导致开销，不保证顺序
//1> PoolExecutor:         线程池，提前创建线程以复用，不保证顺序
//2> ConcurrentExecutor:   固定大小的线程池，按任务加入顺序执行使用同一个线程执行
//3> SynchronousExecutor:  同一时刻只运行一个任务，串行，不会创建管理线程，使用提交任务的线程，此例用main()函数主线程
bool g_isEnable[4] = 
{
    true,  
    true,  
    true,   
    true,   
};

const int g_numThread = 5; 
bool g_isComplete[g_numThread] = 
{
    false, 
    false,
    false,
    false,
    false
};
void ResetFlags()
{
    for (int i = 0; i < g_numThread; i++)
    {
        g_isComplete[i] = false;
    }
}
void SetComplete(int index)
{
    g_isComplete[index] = true;
}
bool IsComplete()
{
    for (int i = 0; i < g_numThread; i++)
    {
        if (!g_isComplete[i])
            return false;
    }
    return true;
}

class Counter : public Runnable
{
public:
    Counter(int id, int num)
        : _id(id)
        , _num(num)
    {

    }

public: 
    static Mutex s_lock;

private:
    int _id;
    int _num;

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
        Thread::sleep(10);

        while (this->_num > 0)
        {
            this->_num --;

            cout << *this;
        }

        Guard<Mutex> lock(Counter::s_lock);
        cout << "Thread complete, id: " << this->_id << ", num: " << this->_num << endl;
        SetComplete(this->_id);
    }
};
Mutex Counter::s_lock;


int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 003 Executor ********** " << endl;
    {
        cout << "***** <1> Test ThreadedExecutor *****" << endl;
        {
            if (g_isEnable[0])
            {
                try
                {
                    ThreadedExecutor executor;
                    for (int i = 0; i < g_numThread; i++)
                    {
                        executor.execute(new Counter(i, 10));
                    }

                    while (!IsComplete())
                    {
                        
                    }
                    ResetFlags();
                }
                catch(Synchronization_Exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
           
        }
        cout << "***** <1> Test ThreadedExecutor *****" << endl;


        cout << "***** <2> Test PoolExecutor *****" << endl;
        {
            if (g_isEnable[1])
            {
                try
                {
                    PoolExecutor executor(g_numThread);
                    for (int i = 0; i < g_numThread; i++)
                    {
                        executor.execute(new Counter(i, 10));
                    }

                    while (!IsComplete())
                    {
                        
                    }
                    ResetFlags();
                }
                catch(Synchronization_Exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
           
        }
        cout << "***** <2> Test PoolExecutor *****" << endl;


        cout << "***** <3> Test ConcurrentExecutor *****" << endl;
        {
            if (g_isEnable[2])
            {
                try
                {
                    ConcurrentExecutor executor;
                    for (int i = 0; i < g_numThread; i++)
                    {
                        executor.execute(new Counter(i, 10));
                    }

                    while (!IsComplete())
                    {
                        
                    }
                    ResetFlags();
                }
                catch(Synchronization_Exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
           
        }
        cout << "***** <3> Test ConcurrentExecutor *****" << endl;


        cout << "***** <4> Test SynchronousExecutor *****" << endl;
        {
            if (g_isEnable[3])
            {
                try
                {
                    SynchronousExecutor executor;
                    for (int i = 0; i < g_numThread; i++)
                    {
                        executor.execute(new Counter(i, 10));
                    }

                    while (!IsComplete())
                    {
                        
                    }
                    ResetFlags();
                }
                catch(Synchronization_Exception& e)
                {
                    std::cerr << e.what() << '\n';
                }
            }
        }
        cout << "***** <4> Test SynchronousExecutor *****" << endl;
    }
    cout << "********** Test zthread 003 Executor ********** " << endl;

    cin.get();
	return 0;
}