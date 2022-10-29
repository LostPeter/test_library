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
#include <fstream>
#include <algorithm>
#include <string>
using namespace std;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
extern "C" {
    #include "lua.h"
    #include "lualib.h"
    #include "lauxlib.h"
}

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

inline char* utf8_to_gbk(const char* utf8)
{
#ifdef _MSC_VER
	const int maxlen = 256;
	char gbk[maxlen];
	wchar_t unicode_str[maxlen];
	int outlen = MultiByteToWideChar(CP_UTF8, 0, utf8, strlen(utf8), unicode_str, maxlen);
	outlen = WideCharToMultiByte(CP_ACP, 0, unicode_str, outlen, gbk, maxlen, NULL, NULL);
	gbk[outlen] = '\0';
	return gbk;
#else // linux
	return const_cast<char*>(utf8);
#endif
}



int SayHello(lua_State* L)
{
    cout << "Hello World !" << endl;
    return 0;
}

int CalculateSin(lua_State* L)
{
    double angle = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(angle));
    return 1;
}

int CalculateSinCos(lua_State* L)
{
    double angle = luaL_checknumber(L, 1);
    lua_pushnumber(L, sin(angle));
    lua_pushnumber(L, cos(angle));
    return 2;
}


int main()
{
    lua_State* L = luaL_newstate();
    luaL_openlibs(L);

    cout << "********** Test lua 002 C ********** " << endl;
    {
        lua_pushcfunction(L, SayHello);
        lua_setglobal(L, "c_SayHello");

        lua_pushcfunction(L, CalculateSin);
        lua_setglobal(L, "c_CalculateSin");

        lua_pushcfunction(L, CalculateSinCos);
        lua_setglobal(L, "c_CalculateSinCos");

        int error = luaL_loadfile(L, "../Assets/Lua/test_0002.lua") | lua_pcall(L, 0, 0, 0);
        if (error != 0)
        {
            cout << "Assets/Lua/test_0002.lua run error !" << endl;
        }
    }
    cout << "********** Test lua 002 C ********** " << endl;

    lua_close(L);
    cin.get();
	return 0;
}