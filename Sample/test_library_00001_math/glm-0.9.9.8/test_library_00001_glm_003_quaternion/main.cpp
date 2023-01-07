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

#define GLM_FORCE_RADIANS
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"


void printVec3(const glm::vec3& v)
{
    cout << "vex3: [" << v.x << "," << v.y << "," << v.z << "]" << endl;
}
void printVec4(const glm::vec4& v)
{
    cout << "vex4: [" << v.x << "," << v.y << "," << v.z << "," << v.w << "]" << endl;
}

void printQua(const glm::quat& q)
{
    cout << "quat: [" << q.x << "," << q.y << "," << q.z << "," << q.w << "]" << endl;
}

void printMat2(const glm::mat2& mat)
{
    cout << "mat2: [" << mat[0][0] << "," << mat[0][1] << ", " 
                      << mat[1][0] << "," << mat[1][1] << "]"
                      << endl;
}
void printMat3(const glm::mat3& mat)
{
    cout << "mat3: [" << mat[0][0] << "," << mat[0][1] << "," << mat[0][2] << ", "  
                      << mat[1][0] << "," << mat[1][1] << "," << mat[1][2] << ", " 
                      << mat[2][1] << "," << mat[2][1] << "," << mat[2][2] << "]"
                      << endl;
}
void printMat4(const glm::mat4& mat)
{
    cout << "mat4: [" << mat[0][0] << "," << mat[0][1] << "," << mat[0][2] << "," << mat[0][3] << ", " 
                      << mat[1][0] << "," << mat[1][1] << "," << mat[1][2] << "," << mat[1][3] << ", " 
                      << mat[2][0] << "," << mat[2][1] << "," << mat[2][2] << "," << mat[2][3] << ", "
                      << mat[3][0] << "," << mat[3][1] << "," << mat[3][2] << "," << mat[3][3] << "]"
                      << endl;
}

int main()
{
    cout << "********** Test glm 003 quaterinion ********** " << endl;
    {
        cout << "***** Test glm::quat *****" << endl;
        {
            glm::quat q = glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, 0.0f))); 
            cout << "quat: size: [" << sizeof(glm::quat) << "]"
                 << ", element: [" << sizeof(glm::quat)/sizeof(float) << "], ";
            printQua(q);
        }
        cout << "***** Test glm::quat *****" << endl;

        cout << endl;

        cout << "***** Test quat rot *****" << endl;
        {
            glm::mat4 mat4(1.0f);
            glm::quat qRotZ = glm::quat(glm::radians(glm::vec3(0.0f, 0.0f, 90.0f))); 
            glm::mat4 matTrans = glm::mat4_cast(qRotZ) * mat4;

            printMat4(mat4);
            printQua(qRotZ);
            glm::quat qRotZ_inv = glm::inverse(qRotZ);
            printMat4(matTrans);

            glm::vec4 x(1.0f, 0.0f, 0.0f, 0.0f);
            cout << "x: ";
            printVec4(x);

            glm::vec4 trans = matTrans * x;
            cout << "trans: ";
            printVec4(trans);

            float fMin = -FLT_MAX;
            float fMax =  FLT_MAX;

            glm::vec3 vMin(fMin, fMin, fMin);
            glm::vec3 vMax(fMax, fMax, fMax);
            if (vMin.x == fMin && vMin.y == fMin && vMin.z == fMin)
                cout << "vec3 min is min Infinite !" << endl;
            else 
                cout << "vec3 min is finite !" << endl;

            
            if (vMax.x == fMax && vMax.y == fMax && vMax.z == fMax)
                cout << "vec3 max is max Infinite !" << endl;
            else 
                cout << "vec3 max is finite !" << endl;

            
        }
        cout << "***** Test quat rot *****" << endl;
    }
    cout << "********** Test glm 003 quaterinion ********** " << endl;

    cin.get();
	return 0;
}