// author: LostPeter
// time:   2022-10-29

#include <iostream>
using namespace std;

#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "zthread/Exceptions.h"
using namespace ZThread;

class Counter : public Runnable
{
public:
    Counter(int id)
        : _id(id)
    {

    }


private:
    int _id;
    int _num;

public:
    virtual void run()
    {
        Thread::sleep(1000);

        this->_num = 1;
        while (this->_num <= 50)
        {
            cout << "id: " << this->_id << ", num: " << this->_num << endl;
            this->_num ++;
        }
    }

};

int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 001 Runnable ********** " << endl;
    {
        try
        {
            Thread t(new Counter(1));
            t.wait();
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 001 Runnable ********** " << endl;

    cin.get();
	return 0;
}