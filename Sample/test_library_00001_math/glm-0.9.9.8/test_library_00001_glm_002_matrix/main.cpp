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
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtx/norm.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"


void printVec3(const glm::vec3& v)
{
    cout << "vex3: [" << v.x << "," << v.y << "," << v.z << "]" << endl;
}
void printVec4(const glm::vec4& v)
{
    cout << "vec4: [" << v.x << "," << v.y << "," << v.z << "," << v.w << "]" << endl;
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
    cout << "********** Test glm 002 matrix ********** " << endl;
    {
        cout << "***** Test glm::mat2 *****" << endl;
        {
            glm::mat2 mat2(1.0f);
            cout << "mat2: size: [" << sizeof(glm::mat2) << "]"
                 << ", element: [" << sizeof(glm::mat2)/sizeof(glm::f32) << "], ";
            printMat2(mat2);
            mat2[0][0] = 2; mat2[0][1] = 3;
            mat2[1][0] = 4; mat2[1][1] = 5;  
            printMat2(mat2);
        }
        cout << "***** Test glm::mat2 *****" << endl;

        cout << endl;

        cout << "***** Test glm::mat2 *****" << endl;
        {
            cout << "***** Test glm::mat2 *****" << endl;
            glm::mat3 mat3(1.0f);
            cout << "mat3: size: [" << sizeof(glm::mat3) << "]"
                 << ", element: [" << sizeof(glm::mat3)/sizeof(glm::f32) << "], ";
            printMat3(mat3);
        }

        cout << endl;

        cout << "***** Test glm::mat4 *****" << endl;
        {
            glm::mat4 mat4(1.0f);
            cout << "mat4: size: [" << sizeof(glm::mat4) << "]"
                 << ", element: [" << sizeof(glm::mat4)/sizeof(glm::f32) << "], ";
            printMat4(mat4);

            glm::vec4 v(1.0f, 1.0f, 1.0f, 1.0f);
            cout << "length: " << glm::length(v) << ", length2: " << glm::length2(v) << endl;

            glm::mat4 m1(1.0f, 0.0f, 0.0f, 3.0f,
                         0.0f, 1.0f, 0.0f, 4.0f,
                         0.0f, 0.0f, 1.0f, 5.0f,
                         0.0f, 0.0f, 0.0f, 1.0f);
            printMat4(m1);

            glm::mat4 m2(1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         3.0f, 4.0f, 5.0f, 1.0f);
            printMat4(m2);
            glm::mat4 m2_inv = glm::inverse(m2);
            printMat4(m2_inv);

            glm::vec4 v1 = v * m1;
            glm::vec4 v2 = m1 * v;
            glm::vec4 v3 = v * m2;
            glm::vec4 v4 = m2 * v;
            printVec4(v1);
            printVec4(v2);
            printVec4(v3);
            printVec4(v4);

            glm::vec3 vTrans(1, 2, 3);
            glm::mat4 m3 = glm::translate(vTrans);
            printMat4(m3);

            glm::mat4 matrix4View_LookAt20 = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f),
                                                         glm::vec3(0.0f, 0.0f, 0.0f),
                                                         glm::vec3(0.0f, 1.0f, 0.0f));
            printMat4(matrix4View_LookAt20);

            glm::mat4 matrix4View_LookAt20Minus = glm::lookAt(glm::vec3(0.0f, 0.0f, -20.0f),
                                                              glm::vec3(0.0f, 0.0f, 0.0f),
                                                              glm::vec3(0.0f, 1.0f, 0.0f));  
            printMat4(matrix4View_LookAt20Minus);


            glm::mat4 mat4RotY45 = glm::eulerAngleY(glm::radians(45.0f));
            printMat4(mat4RotY45);

            glm::vec4 pos(1, 0, 0, 0);
            printVec4(pos);
            glm::vec4 posTrans = mat4RotY45 * pos;
            printVec4(posTrans);

            glm::quat qRot = glm::toQuat(mat4RotY45);
            printQua(qRot);
            printVec4(pos);
            posTrans = qRot * pos;
            printVec4(posTrans);

        }
        cout << "***** Test glm::mat4 *****" << endl;

        cout << endl;

        cout << "***** Test glm::function *****" << endl;
        {
            float fFov = glm::radians(45.0f);
            float fAspect = 1.33f;
            float fNear = 0.1f;
            float fFar = 1000.0f;
            
            glm::mat4 mat4PerspectiveRH = glm::perspectiveRH(fFov,
                                                             fAspect,
                                                             fNear, 
                                                             fFar);
            printMat4(mat4PerspectiveRH);                                          

            glm::mat4 mat4PerspectiveLH = glm::perspectiveLH(fFov,
                                                             fAspect,
                                                             fNear, 
                                                             fFar);
            printMat4(mat4PerspectiveLH);            
        }
        cout << "***** Test glm::function *****" << endl;

        cout << endl;

        cout << "***** Test glm::rotate *****" << endl;
        {
            //glm::vec3 vRight(1, 0, 0);
            glm::vec3 vRight(0.985109f, 0, -0.17192f);
			//glm::vec3 vUp(0, 0.707, 0.707);
            glm::vec3 vUp(0, 1, 0);
			glm::vec3 vDir = glm::cross(vRight, vUp);

            printVec3(vRight);
            printVec3(vUp);
            printVec3(vDir);
            glm::mat4 mat4Rot(vRight.x, vRight.y, vRight.z, 0,
                                 vUp.x,    vUp.y,    vUp.z, 0,
                                vDir.x,   vDir.y,   vDir.z, 0,
                                     0,        0,        0, 1);
            printMat4(mat4Rot);   

            glm::mat4 matTrans(1,  0,   0,  0,
                               0,  1,   0,  0,
                               0,  0,   1,  0,
                               0,  0, -20, 1);  
            printMat4(matTrans);   
            
            glm::mat4 matT = matTrans * mat4Rot;
            printMat4(matT);   

            glm::vec4 v0(0,0,0,1);
            v0 = matT * v0;
            printVec4(v0);

            

        }
        cout << "***** Test glm::rotate *****" << endl;

        cout << endl;

        cout << "***** Test glm::aabb *****" << endl;
        {
            glm::vec4 vMin(-100, -100, -100, 1);
            glm::vec4 vMax(100, 100, 100, 1);

            glm::mat4 matTrans(0.01f,     0,     0,     0,
                                   0, 0.01f,     0,     0,
                                   0,     0, 0.01f,     0,
                                  -3,     0,     0,     1);
            printMat4(matTrans);  

            glm::vec4 cur = vMin;
            glm::vec4 v1 = matTrans * cur;
            printVec4(v1);

            cur.z = vMax.z;
            glm::vec4 v2 = matTrans * cur;
            printVec4(v2);

            cur.y = vMax.y;
            glm::vec4 v3 = matTrans * cur;
            printVec4(v3);

            cur.z = vMin.z;
            glm::vec4 v4 = matTrans * cur;
            printVec4(v4);

            cur.x = vMax.x;
            glm::vec4 v5 = matTrans * cur;
            printVec4(v5);

            cur.z = vMax.z;
            glm::vec4 v6 = matTrans * cur;
            printVec4(v6);

            cur.y = vMin.y;
            glm::vec4 v7 = matTrans * cur;
            printVec4(v7);

            cur.z = vMin.z;
            glm::vec4 v8 = matTrans * cur;
            printVec4(v8);
        }
        cout << "***** Test glm::aabb *****" << endl;
    }
    cout << "********** Test glm 002 matrix ********** " << endl;

    cin.get();
	return 0;
}