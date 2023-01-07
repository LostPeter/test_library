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

#include "glm/glm.hpp"


void printVec2(const glm::vec2& v)
{
    cout << "vec2: [" << v.x << "," << v.y << "," << "]" << endl;
}
void printVec3(const glm::vec3& v)
{
    cout << "vec3: [" << v.x << "," << v.y << "," << v.z << "]" << endl;
}
void printVec4(const glm::vec4& v)
{
    cout << "vec4: [" << v.x << "," << v.y << "," << v.z << "," << v.w << "]" << endl;
}

int main()
{
    cout << "********** Test glm 001 vec ********** " << endl;
    {
        glm::vec2 v2(0.0f, 0.0f);
        cout << "vec2: size: [" << sizeof(glm::vec2) << "]"
             << ", element: [" << sizeof(glm::vec2)/sizeof(float) << "]";
        printVec2(v2);

        glm::vec3 v3(1.0f, 1.0f, 1.0f);
        cout << "vec3: size: [" << sizeof(glm::vec3) << "]"
             << ", element: [" << sizeof(glm::vec3)/sizeof(float) << "]";
        printVec3(v3);

        glm::vec4 v4(2.0f, 2.0f, 2.0f, 2.0f);
        cout << "vec4: size: [" << sizeof(glm::vec4) << "]"
             << ", element: [" << sizeof(glm::vec4)/sizeof(float) << "]";
        printVec4(v4);
    }
    cout << "********** Test glm 001 vec ********** " << endl;

    cin.get();
	return 0;
}