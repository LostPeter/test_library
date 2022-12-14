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
#include <algorithm>
#include <string>
using namespace std;

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "rapidxml_print.hpp"
using namespace rapidxml;

#include "DfFileXML.h"


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

bool load_file(const char* szFile, std::vector<char>& content)
{
    std::ifstream file(szFile, std::ios::ate | std::ios::binary);
    if (!file.is_open())
    {
        cout << "*********************** VulkanWin32Platform::LoadFileContent: Read file failed, path: " << szFile << endl;
        return false;
    }

    size_t fileSize = (size_t)file.tellg();
    content.resize(fileSize + 1);
    file.seekg(0);
    file.read(content.data(), fileSize);
    file.close();
    content[fileSize] = 0;
    return true;
}

void test_rapidxml_load()
{
    std::string pathXML = GetAssetsPath() + "Assets/Test/engine_common.config";

    DfFileXML xml;
    if (xml.LoadXML(pathXML))
	{
		cout << "test_rapidxml_load: Load file: [" << pathXML << "] success !" << endl;
    }
	else
	{
		cout << "test_rapidxml_load: Load file: [" << pathXML << "] failed !" << endl;
	}

    DfXMLDocument* pDocument = xml.GetXMLDocument();
    DfXMLElement* pElementRoot = pDocument->GetElementRoot();
    cout << "root: " << pElementRoot->GetName()
         << ", count: " << pElementRoot->GetElementChildrenCount() 
         << endl;

    std::vector<char> content;
    if (!load_file(pathXML.c_str(), content))
    {
        cout << "test_rapidxml_load: load_file: [" << pathXML << "] failed !" << endl;
        return;
    }

    DfFileXML xml2;
    if (xml2.CreateXML(content.data()))
    {
        cout << "test_rapidxml_load: Load file from content: [" << pathXML << "] success !" << endl;
    }
    else
    {
        cout << "test_rapidxml_load: Load file from content: [" << pathXML << "] failed !" << endl;
    }
}


void test_rapidxml_write()
{
	std::string pathXML = GetAssetsPath() + "Assets/Test/xml_out.xml";

    DfFileXML xml; 
    xml.CreateXML();

    DfXMLDocument* pDocument = xml.GetXMLDocument();
    DfXMLElement* pElementDeclaration = pDocument->CreateElementDeclaration();
    DfXMLElement* pElementRoot = pDocument->CreateElementRoot("document");
    DfXMLElement* pElementChild = nullptr;
    DfXMLElement* pElementChild2 = nullptr;
    DfXMLElement* pElementChild3 = nullptr;

    pElementChild = pElementRoot->AddElementChild(new DfXMLElement("item"));
    pElementChild->AddAttribute("name", "name");
    pElementChild->AddAttribute("value", "LostPeter");

    pElementChild = pElementRoot->AddElementChild(new DfXMLElement("item"));
    pElementChild->AddAttribute("name", "address");
    pElementChild->AddAttribute("value", "??????");

    pElementChild = pElementRoot->AddElementChild(new DfXMLElement("item"));
    pElementChild->AddAttribute("name", "age");
    pElementChild->AddAttribute("value", "40");

    pElementChild = new DfXMLElement("item");
    pElementChild->AddAttribute("name", "value1");
    pElementChild = pElementRoot->AddElementChild(pElementChild);
    pElementChild2 = pElementChild->AddElementChild(new DfXMLElement("item"));
    pElementChild2->AddAttribute("name", "1");
    pElementChild2->AddAttribute("value", "23, 43, -2.3, 6.7, 90");
    pElementChild2 = pElementChild->AddElementChild(new DfXMLElement("item"));
    pElementChild2->AddAttribute("name", "2");
    pElementChild2->AddAttribute("value", "-9, -19, 10, 2");
    pElementChild2 = pElementChild->AddElementChild(new DfXMLElement("item"));
    pElementChild2->AddAttribute("name", "3");
    pElementChild2->AddAttribute("value", "-5, -55");
    
    pElementChild = pElementRoot->AddElementChild(new DfXMLElement("item"));
    pElementChild->AddAttribute("name", "value2");
    pElementChild->AddAttribute("value", "13.3, 1.9, 2.10");

    pElementChild = new DfXMLElement("item");
    pElementChild->AddAttribute("name", "bei_jing");
    pElementChild = pElementRoot->AddElementChild(pElementChild);
    pElementChild2 = pElementChild->AddElementChild(new DfXMLElement("item"));
    pElementChild2->AddAttribute("name", "address");
    pElementChild2->AddAttribute("value", "??????");
    pElementChild2 = pElementChild->AddElementChild(new DfXMLElement("item"));
    pElementChild2->AddAttribute("name", "car");
    pElementChild2->AddAttribute("value", "true");
    pElementChild2 = pElementChild->AddElementChild(new DfXMLElement("item"));
    pElementChild2->AddAttribute("name", "dog");
    pElementChild2->AddAttribute("value", "false");

    pElementChild = new DfXMLElement("item");
    pElementChild->AddAttribute("name", "hu_bei");
    pElementChild = pElementRoot->AddElementChild(pElementChild);
    pElementChild2 = pElementChild->AddElementChild(new DfXMLElement("item"));
    pElementChild2->AddAttribute("name", "address");
    pElementChild2->AddAttribute("value", "??????");

    pElementChild2 = new DfXMLElement("item");
    pElementChild2->AddAttribute("name", "value1");
    pElementChild2 = pElementChild->AddElementChild(pElementChild2);
    pElementChild3 = pElementChild2->AddElementChild(new DfXMLElement("item"));
    pElementChild3->AddAttribute("name", "xiang_cheng");
    pElementChild3->AddAttribute("value", "??????");
    pElementChild3 = pElementChild2->AddElementChild(new DfXMLElement("item"));
    pElementChild3->AddAttribute("name", "xiang_yang");
    pElementChild3->AddAttribute("value", "??????");


    if (!xml.SaveXML(pathXML))
    {
        cout << "test_rapidxml_write: Save xml file: [" << pathXML << "] failed !" << endl;
    }
    else
    {
        cout << "test_rapidxml_write: Save xml file: [" << pathXML << "] success !" << endl;
    }
}


int main()
{
    cout << "********** Test rapidxml 002 Wrapper ********** " << endl;
    {
        cout << "***** <1> test_rapidxml_load ****** " << endl;
        {
            test_rapidxml_load();
        }
        cout << "***** <1> test_rapidxml_load ****** " << endl;

        cout << "***** <2> test_rapidxml_write ***** " << endl;
        {
            test_rapidxml_write();
        }
        cout << "***** <2> test_rapidxml_write ***** " << endl;
    }
    cout << "********** Test rapidxml 002 Wrapper ********** " << endl;

    cin.get();
	return 0;
}