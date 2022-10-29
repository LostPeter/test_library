#include <iostream>
#include <exception>
#ifdef  _WIN32
    #include <Windows.h>
#endif
#include "MainWindow.h"

MainWindow mainWindow;

int main(int argc, char* argv[])
{
	try
	{
#ifndef _DEBUG
		//ShowWindow(GetConsoleWindow(), SW_HIDE);
#endif // _DEBUG

		mainWindow.initVulkan();
		mainWindow.renderLoop();
		if (mainWindow.minimizedWindowNeedClose == false) {
			mainWindow.finishVulkan();
		}
	}
	catch (std::exception& ex)
	{
#ifdef _DEBUG
		std::cout << ex.what();
		return EXIT_FAILURE;
#else
#ifdef  _WIN32
		MessageBoxA(NULL, ex.what(), "Error", MB_OK);
#endif // _WIN32
#endif // _DEBUG
		return EXIT_FAILURE;
	}
	catch (ErrorInfo& errInfo)
	{
		std::string errInfoStr = "MainWindow exception\n"
			+ (std::string)"What: " + errInfo.what + "\n"
			+ (std::string)"File: " + errInfo.file + "\n"
			+ (std::string)"Line: " + errInfo.line + "\n";
#ifdef _DEBUG
		std::cout << errInfoStr;
		return EXIT_FAILURE;
#else
#ifdef  _WIN32
		MessageBoxA(NULL, errInfo.what.c_str(), "Error", MB_OK);
#endif // _WIN32
#endif // _DEBUG
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}