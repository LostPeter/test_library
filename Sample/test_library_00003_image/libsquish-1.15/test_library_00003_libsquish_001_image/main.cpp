// author: LostPeter
// time:   2022-10-29

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
#include <algorithm>
using namespace std;

#include "squish.h"


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


int main()
{
    cout << "********** Test libsquish 001 image ********** " << endl;
    {
        squish::u8 pixels[16*4];  // 16 pixels of input
        squish::u8 block[8];      // 8 bytes of output

        /* write some pixel data */

        // compress the 4x4 block using DXT1 compression
        squish::Compress(pixels, block, squish::kDxt1);
        
        // decompress the 4x4 block using DXT1 compression
        squish::Decompress(pixels, block, squish::kDxt1);
    }
    cout << "********** Test libsquish 001 image ********** " << endl;
    
    cin.get();
    return 0;
}