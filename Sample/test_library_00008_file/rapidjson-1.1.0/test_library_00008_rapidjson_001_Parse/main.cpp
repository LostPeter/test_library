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

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/prettywriter.h"
#include "rapidjson/stringbuffer.h"
using namespace rapidjson;


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

int test_rapidjson_parse()
{
    std::string pathJson = GetAssetsPath() + "Assets/Test/json_in.json";
	std::ifstream in(pathJson);
	if (!in.is_open()) 
    {
		fprintf(stderr, "test_rapidjson_parse: Fail to read json file: %s\n", pathJson.c_str());
		return -1;
	}

	std::string json_content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
	in.close();

	rapidjson::Document dom;
	if (!dom.Parse(json_content.c_str()).HasParseError()) 
    {
		if (dom.HasMember("name") && dom["name"].IsString()) 
        {
			fprintf(stdout, "name: %s\n", dom["name"].GetString());
		}

		if (dom.HasMember("address") && dom["address"].IsString()) 
        {
			fprintf(stdout, "address: %s\n", utf8_to_gbk(dom["address"].GetString()));
		}

		if (dom.HasMember("age") && dom["age"].IsInt()) 
        {
			fprintf(stdout, "age: %d\n", dom["age"].GetInt());
		}

		const char* tmp = "xxx";
		if (!dom.HasMember(tmp)) 
        {
			fprintf(stdout, "Warning: it has no member: %s\n", tmp);
		}

		if (dom.HasMember("value1") && dom["value1"].IsArray()) 
        {
			const rapidjson::Value& arr = dom["value1"];
			for (int i = 0; i < arr.Size(); ++i) 
            {
				const rapidjson::Value& tmp = arr[i];
				
				fprintf(stdout, "value1:\ni = %d:", i);
				for (int j = 0; j < tmp.Size(); ++j) 
                {
					if (tmp[j].IsInt())
						fprintf(stdout, "%d, ", tmp[j].GetInt());
					if (tmp[j].IsFloat())
						fprintf(stdout, "%.1f, ", tmp[j].GetFloat());
				}
				fprintf(stdout, "\n");
			}
		}

		if (dom.HasMember("value2") && dom["value2"].IsArray()) 
        {
			const rapidjson::Value& arr = dom["value2"];

			fprintf(stdout, "value2: ");
			for (int i = 0; i < arr.Size(); ++i) 
            {
				fprintf(stdout, "%.2f, ", arr[i].GetFloat());
			}
			fprintf(stdout, "\n");
		}

		if (dom.HasMember("bei_jing") && dom["bei_jing"].IsObject()) 
        {
			const rapidjson::Value& obj = dom["bei_jing"];

			if (obj.HasMember("address") && obj["address"].IsString()) 
            {
				fprintf(stdout, "address: %s\n", utf8_to_gbk(obj["address"].GetString()));
			}

			if (obj.HasMember("car") && obj["car"].IsBool()) 
            {
				fprintf(stdout, "car: %d\n", obj["car"].GetBool());
			}

			if (obj.HasMember("cat") && obj["cat"].IsBool()) 
            {
				fprintf(stdout, "cat: %d\n", obj["cat"].GetBool());
			}
		}

		if (dom.HasMember("hu_bei") && dom["hu_bei"].IsObject()) 
        {
			const rapidjson::Value& obj = dom["hu_bei"];

			if (obj.HasMember("address") && obj["address"].IsString()) 
            {
				fprintf(stdout, "address: %s\n", utf8_to_gbk(obj["address"].GetString()));
			}

			if (obj.HasMember("value1") && obj["value1"].IsArray()) 
            {
				const rapidjson::Value& arr = obj["value1"];

				if (arr[0].HasMember("xiang_cheng") && arr[0]["xiang_cheng"].IsString()) 
                {
					fprintf(stdout, "xiang_cheng: %s\n", utf8_to_gbk(arr[0]["xiang_cheng"].GetString()));
				}

				if (arr[0].HasMember("tai_an") && arr[0]["tai_an"].IsString()) 
                {
					fprintf(stdout, "tai_an: %s\n", utf8_to_gbk(arr[0]["tai_an"].GetString()));
				}
			}
		}
	} 
    else 
    {
		fprintf(stderr, "test_rapidjson_parse: Fail to parse json file: %s\n", pathJson.c_str());
		return -1;
	}

	// iter json
	for (rapidjson::Value::ConstMemberIterator iter = dom.MemberBegin(); iter != dom.MemberEnd(); ++iter) 
    {
		fprintf(stdout, "iter json name: %s, type: %d\n", iter->name.GetString(), iter->value.GetType());
	}

	return 0;
}


int test_rapidjson_write()
{
	rapidjson::StringBuffer buf;
	//rapidjson::Writer<rapidjson::StringBuffer> writer(buf);
	rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buf); // it can word wrap

	writer.StartObject();

	writer.Key("name"); writer.String("LostPeter");
	writer.Key("address"); writer.String("北京");
	writer.Key("age"); writer.Int(40);

	writer.Key("value1");
	writer.StartArray();
	writer.StartArray();

	writer.StartObject();

	writer.Key("name"); writer.String("LostPeter");
	writer.Key("address"); writer.String("北京");
	writer.Key("age"); writer.Int(40);

	writer.Key("value1");
	writer.StartArray();
	writer.StartArray();
	writer.Double(23); writer.Double(43); writer.Double(-2.3); writer.Double(6.7); writer.Double(90);
	writer.EndArray();

	writer.StartArray();
	writer.Int(-9); writer.Int(-19); writer.Int(10); writer.Int(2);
	writer.EndArray();

	writer.StartArray();
	writer.Int(-5); writer.Int(-55);
	writer.EndArray();
	writer.EndArray();

	writer.Key("value2");
	writer.StartArray();
	writer.Double(13.3); writer.Double(1.9); writer.Double(2.10);
	writer.EndArray();

	writer.Key("bei_jing");
	writer.StartObject();
	writer.Key("address"); writer.String("海淀");
	writer.Key("car"); writer.Bool(true);
	writer.Key("dog"); writer.Bool(false);
	writer.EndObject();

	writer.Key("hu_bei");
	writer.StartObject();
	writer.Key("address"); writer.String("襄阳");
	writer.Key("value1");
	writer.StartArray();
	writer.Key("xiang_cheng"); writer.String("隆中");
	writer.Key("xiang_yang"); writer.String("汉江");
	writer.EndArray();
	writer.EndObject();

	writer.EndObject();

	const char* json_content = buf.GetString();
	fprintf(stdout, "json content: %s\n", json_content);


    std::string pathJson = GetAssetsPath() + "Assets/Test/json_out.json";
	std::ifstream in(pathJson);

	std::ofstream outfile;
	outfile.open(pathJson);
	if (!outfile.is_open()) 
    {
		fprintf(stderr, "test_rapidjson_write: Fail to open file to write: %s\n", pathJson.c_str());
		return -1;
	}

	outfile << json_content << std::endl;
	outfile.close();

	return 0;
}


int main()
{
    cout << "********** Test rapidjson 001 Parse ********** " << endl;
    {
        cout << "***** <1> test_rapidjson_parse ***** " << endl;
        {
            test_rapidjson_parse();
        }
        cout << "***** <1> test_rapidjson_parse ***** " << endl;

        cout << "***** <2> test_rapidjson_write ***** " << endl;
        {
            test_rapidjson_write();
        }
        cout << "***** <2> test_rapidjson_write ***** " << endl;
    }
    cout << "********** Test rapidjson 001 Parse ********** " << endl;

    cin.get();
	return 0;
}