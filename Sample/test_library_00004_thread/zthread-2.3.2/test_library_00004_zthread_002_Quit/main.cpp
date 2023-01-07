/****************************************************************************
* test_library - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-29
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

//Platform
#define PLATFORM_IOS				1
#define PLATFORM_ANDROID			2
#define PLATFORM_WIN32				3
#define PLATFORM_MAC				4

#if defined(__APPLE_CC__)
    #undef  PLATFORM
	#if __ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__ >= 60000 || __IPHONE_OS_VERSION_MIN_REQUIRED >= 60000
		#define PLATFORM			PLATFORM_IOS
	#else
		#define PLATFORM			PLATFORM_MAC
	#endif
#elif defined(__ANDROID__)
	#undef  PLATFORM
	#define PLATFORM				PLATFORM_ANDROID
#elif defined(WIN32) 
    #undef  PLATFORM
	#define PLATFORM				PLATFORM_WIN32
	#pragma warning (disable:4127)  
#else
	#pragma error "UnKnown platform! Abort! Abort!"
#endif

#if PLATFORM == PLATFORM_WIN32
    #undef min
	#undef max
	#if defined(__MINGW32__)
		#include <unistd.h>
	#endif
	#include <io.h>
	#include <process.h>
    #include <Windows.h>
    #include <shlwapi.h>
#elif PLATFORM == PLATFORM_ANDROID

#elif PLATFORM == PLATFORM_MAC
    #include "ocutil.h"
#endif

#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
using namespace std;

#include "zthread/Runnable.h"
#include "zthread/Thread.h"
#include "zthread/Exceptions.h"
#include "zthread/Time.h"
using namespace ZThread;


#if PLATFORM == PLATFORM_WIN32
    char* Unicode2Utf8(wchar_t* unicodeStr) {
        int cStrLen = WideCharToMultiByte(CP_UTF8, 0, unicodeStr, -1, NULL, 0, NULL, NULL);
        char* cStr = (char*)malloc(sizeof(char) * (cStrLen + 1));
        WideCharToMultiByte(CP_UTF8, 0, unicodeStr, -1, cStr, cStrLen + 1, NULL, NULL);
        *(cStr + cStrLen) = '\0';
        return cStr;
    }
#endif
std::string GetAssetsPath()
{
    std::string path;

    #if PLATFORM == PLATFORM_WIN32
        wchar_t szBuf[512];
        ::GetModuleFileNameW(NULL, szBuf, 512);
        ::PathRemoveFileSpecW(szBuf);
        char* utf8 = Unicode2Utf8(szBuf);

        path.append(utf8);
        free(utf8); 

        std::replace(path.begin(), path.end(), '\\', '/');

    #elif PLATFORM == PLATFORM_MAC
        path = OCUtil_GetPathExecute();
        
    #endif
        std::cout << "Path exe: " << path << std::endl; 
        if (path[path.size() - 1] == '.')
        {
            path = path.substr(0, path.size() - 2);
        }
        size_t index = path.find_last_of('/');
        path = path.substr(0, index);
        path += "/";

    return path;
}

class DisplayTask : public Runnable
{
public:
    DisplayTask(const string& file)
        : isQuit(false)
    {
        this->in.open(file.c_str());
    }
    ~DisplayTask()
    {
        this->in.close();
    }

private:
    ifstream    in;
    string      line;
    bool        isQuit;

public:
    virtual void run()
    {
        Thread::sleep(10);   

        while (getline(in, this->line) && !this->isQuit)
        {
            cout << this->line << endl;
            Thread::sleep(1000);   
        }
        cout << "DisplayTask:run() is done !" << endl;
    }

    void quit()
    {
        this->isQuit = true;
        cout << "DisplayTask:quit() is quit !" << endl;
    }
};


int main()
{
    #ifndef NDEBUG
        Thread::s_isDebugPrint = false;
    #endif

    cout << "********** Test zthread 002 Quit ********** " << endl;
    {
        try
        {
            string pathFile = GetAssetsPath() + "Assets/Test/ReadMe.txt";
            DisplayTask* pTask = new DisplayTask(pathFile);
            Thread t(pTask);
            //cin.get();

            time_t start = time(NULL);
            time_t diff = time(NULL) - start;
            while (diff < 5)
            {
                diff = time(NULL) - start;

                //cout << "Time: " << diff << endl;
            }
            cout << "Time finish: " << diff << endl;
            pTask->quit();
        }
        catch(Synchronization_Exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    cout << "********** Test zthread 002 Quit ********** " << endl;

    cin.get();
	return 0;
}