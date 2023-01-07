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

#define GLM_FORCE_LEFT_HANDED
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


void printVec2(const std::string& name, const glm::vec2& v)
{
    cout << "[" << v.x << "," << v.y << "," << "]" << " - vec2 - " << name << endl;
}
void printVec3(const std::string& name, const glm::vec3& v)
{
    cout << "[" << v.x << "," << v.y << "," << v.z << "]" << " - vec3 - " << name<< endl;
}
void printVec4(const std::string& name, const glm::vec4& v)
{
    cout << "[" << v.x << "," << v.y << "," << v.z << "," << v.w << "]" << " - vec4 - " << name<< endl;
}

void printQuat(const std::string& name, const glm::quat& q)
{
    cout << "[" << q.x << "," << q.y << "," << q.z << "," << q.w << "]" << " - quat - " << name << endl;
}

void printMat2(const std::string& name, const glm::mat2& mat)
{
    cout << "[" << mat[0][0] << "," << mat[0][1] << ", "
                << mat[1][0] << "," << mat[1][1] << "]"
                << " - mat2 - " << name
         << endl;
}
void printMat3(const std::string& name, const glm::mat3& mat)
{
    cout << "[" << mat[0][0] << "," << mat[0][1] << "," << mat[0][2] << ", "
                << mat[1][0] << "," << mat[1][1] << "," << mat[1][2] << ", " 
                << mat[2][1] << "," << mat[2][1] << "," << mat[2][2] << "]"
                << " - mat3 - " << name
         << endl;
}
void printMat4(const std::string& name, const glm::mat4& mat)
{
    cout << "[" << mat[0][0] << "," << mat[0][1] << "," << mat[0][2] << "," << mat[0][3] << ", "
                << mat[1][0] << "," << mat[1][1] << "," << mat[1][2] << "," << mat[1][3] << ", "
                << mat[2][0] << "," << mat[2][1] << "," << mat[2][2] << "," << mat[2][3] << ", "
                << mat[3][0] << "," << mat[3][1] << "," << mat[3][2] << "," << mat[3][3] << "]"
                << " - mat4 - " << name
         << endl;
}

int main()
{
    cout << "********** Test glm 004 camera ********** " << endl;
    {
        glm::vec3 vCameraPos(0.0f, 2.0f, -1.0f);
        printVec3("Camera Pos", vCameraPos);
        glm::vec3 vCameraLookAt(0.0f, 0.0f, 0.0f);
        printVec3("Camera LookAt", vCameraLookAt);
        glm::vec3 vCameraUp(0.0f, 1.0f, 0.0f);
        printVec3("Camera Up", vCameraUp);

        glm::vec3 vX(1.0f, 0.0f, 0.0f);
        glm::vec3 vY(0.0f, 1.0f, 0.0f);
        glm::vec3 vZ(0.0f, 0.0f, 1.0f);
        glm::vec3 vRight, vUp, vDir;
        glm::vec3 vEulerAngles;

        std::cout << std::endl;

        //row-col matrix
        std::cout << "******************** row-col matrix *********************" << std::endl;
        {
            glm::vec4 v(1.0f, 1.0f, 1.0f, 1.0f);
            std::cout << "Length: " << glm::length(v) << ", Length2: " << glm::length2(v) << std::endl;

            glm::mat4 m1(1.0f, 0.0f, 0.0f, 3.0f,
                         0.0f, 1.0f, 0.0f, 4.0f,
                         0.0f, 0.0f, 1.0f, 5.0f,
                         0.0f, 0.0f, 0.0f, 1.0f);
            printMat4("m1: ", m1);

            glm::mat4 m2(1.0f, 0.0f, 0.0f, 0.0f,
                         0.0f, 1.0f, 0.0f, 0.0f,
                         0.0f, 0.0f, 1.0f, 0.0f,
                         3.0f, 4.0f, 5.0f, 1.0f);
            printMat4("m2: ", m2);

            glm::vec4 v1 = v * m1;
            glm::vec4 v2 = m1 * v;
            glm::vec4 v3 = v * m2;
            glm::vec4 v4 = m2 * v;
            printVec4("v * m1", v1);
            printVec4("m1 * v", v2);
            printVec4("v * m2", v3);
            printVec4("m2 * v", v4);
        }
        std::cout << "******************** row-col matrix *********************" << std::endl;

        std::cout << std::endl;

        //Construct vRight-vUp-vDir
        std::cout << "******************** vRight-vUp-vDir ********************" << std::endl;
        {
            vDir = glm::normalize(vCameraLookAt - vCameraPos);
            printVec3("Dir", vDir);

            vRight = glm::normalize(glm::cross(vCameraUp, vDir));
            printVec3("Right", vRight);

            vUp = glm::cross(vDir, vRight);
            printVec3("Up", vUp);
        }
        std::cout << "******************** vRight-vUp-vDir ********************" << std::endl;

        std::cout << std::endl;

        //Left-Hand glm::lookAtLH
        std::cout << "******************** Left Hand ********************" << std::endl;
        {   
            glm::mat4 matViewLH = glm::lookAtLH(vCameraPos, vCameraLookAt, vCameraUp);
            printMat4("Camera View LH", matViewLH);
            
            glm::mat3 matRotLH(matViewLH);
            matRotLH = glm::transpose(matRotLH);
            printMat3("Rot Matrix3 LH", matRotLH);
            glm::quat qRotLH = glm::quat_cast(matRotLH);
            printQuat("Rot Quat LH", qRotLH);

            vRight = qRotLH * vX;
            printVec3("Right LH", vRight);
            vUp = qRotLH * vY;
            printVec3("Up LH", vUp);
            vDir = qRotLH * vZ;
            printVec3("Dir LH", vDir);

            vEulerAngles = glm::eulerAngles(qRotLH);
            vEulerAngles.x = glm::degrees(vEulerAngles.x);
            vEulerAngles.y = glm::degrees(vEulerAngles.y);
            vEulerAngles.z = glm::degrees(vEulerAngles.z);
            printVec3("EulerAngles LH", vEulerAngles);
        }
        std::cout << "******************** Left Hand ********************" << std::endl;

        std::cout << std::endl;

        //Right-Hand glm::lookAtRH
        std::cout << "******************** Right Hand ********************" << std::endl;
        {
            glm::mat4 matViewRH = glm::lookAtRH(vCameraPos, vCameraLookAt, vCameraUp);
            printMat4("Camera View RH", matViewRH);

            glm::mat3 matRotRH(matViewRH);
            printMat3("Rot Matrix3 RH", matRotRH);
            glm::quat qRotRH = glm::quat_cast(matRotRH);
            printQuat("Rot Quat RH", qRotRH);

            vRight = qRotRH * vX;
            printVec3("Right RH", vRight);
            vUp = qRotRH * vY;
            printVec3("Up RH", vUp);
            vDir = qRotRH * vZ;
            printVec3("Dir RH", vDir);

            vEulerAngles = glm::eulerAngles(qRotRH);
            vEulerAngles.x = glm::degrees(vEulerAngles.x);
            vEulerAngles.y = glm::degrees(vEulerAngles.y);
            vEulerAngles.z = glm::degrees(vEulerAngles.z);
            printVec3("EulerAngles RH", vEulerAngles);
        }
        std::cout << "******************** Right Hand ********************" << std::endl;
    }
    cout << "********** Test glm 004 camera ********** " << endl;

    cin.get();
	return 0;
}