#pragma once
// OpenGL 初始化库
#include "glad/glad.h"
#include "GLFW/glfw3.h"
// glm 数学库
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
// 纹理
#define STB_IMAGE_IMPLEMENTATION
#include "tool/stb_image.h"// stb_image
// 几何体
#include "geometry/BoxGeometry.h"
#include "geometry/PlaneGeometry.h"
#include "geometry/SphereGeometry.h"
// Shader
#include "tool/shader.h"
// 模型
#include "tool/mesh.h"
#include "tool/model.h"
// Camera
#include "tool/camera.h"
// 系统库
#include <iostream>
using namespace std;


const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;


// 函数
void processInput(GLFWwindow* window)// 处理按键输入
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)//窗口大小变化回调函数
{
    glViewport(0, 0, width, height);
}
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}
void printAngle(float _angle)
{
    cout << _angle << endl;
}

void printOpenGLInfo(const bool printInfo)
{
    if (printInfo)
    {
        cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
        cout << "GPU: " << glGetString(GL_RENDERER) << endl;
    }
}
glm::mat4 transMat(glm::mat4 _mat,      // 变换矩阵
    glm::vec3 _rotAixs,                 // 旋转轴
    float _angle,                       // 旋转角度
    glm::vec3 _scale,                   // 缩放比例
    glm::vec3 _pos)                     // 位置
{
    _mat = glm::translate(_mat, _pos);
    _mat = glm::rotate(_mat, glm::radians(_angle), _rotAixs);
    _mat = glm::scale(_mat, _scale);
    return _mat;
}
void resetTransMats(glm::mat4 _mat,     // 变换矩阵
    glm::vec3 _rotAixs,                 // 旋转轴
    float _angle,                       // 旋转角度
    glm::vec3 _scale,                   // 缩放比例
    glm::vec3 _pos)                     // 位置
{
    _angle = 0.0f;
    _mat = glm::mat4(1.0f);
    _rotAixs = glm::vec3(0.0f, 0.0f, 1.0f);
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _pos = glm::vec3(0.0f, 0.0f, 0.0f);
}
glm::mat4 Mat4_MVP(float M_angle,      // 模型角度
    glm::vec3 M_Aixs,   // 旋转轴
    glm::vec3 V_pos,    // 视角位置
    float P_angle,      // 视角角度
    float P_width,      // 屏幕宽
    float P_height,     // 屏幕高
    float P_Near,       // 近裁平面
    float P_Far)        // 远裁平面
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, M_angle, M_Aixs);
    view = glm::translate(view, V_pos);
    projection = glm::perspective(P_angle, P_width / P_height, P_Near, P_Far);

    return projection * view * model;
}

