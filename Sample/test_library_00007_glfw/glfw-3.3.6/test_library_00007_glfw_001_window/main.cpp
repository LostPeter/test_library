/****************************************************************************
* test_library - Copyright (C) 2022 by LostPeter
* 
* Author: LostPeter
* Time:   2022-10-29
*
* This code is licensed under the MIT license (MIT) (http://opensource.org/licenses/MIT)
****************************************************************************/

#include <iostream>
using namespace std;

#include "GLFW/glfw3.h"


int main()
{
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "test_library_00007_glfw_001_window", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window !" << endl;
        glfwTerminate();
        return -1;
    }

    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    
    glfwMakeContextCurrent(window);
    return 0;
}