#pragma once
// OpenGL 初始化库
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "tool/shader.h"// Shader
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
// Camera
#include "tool/camera.h"
// 系统库
#include <iostream>
using namespace std;



// OpenGL Settings
float SCR_WIDTH = 1920.0f;
float SCR_HEIGHT = 1080.0f;
string Shader::dirName = "";
bool Shader::lookShaderPath = true;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

float deltaTime = 0.001f;
float lastFrame = 0.0f;
float fov = 55.0f;

glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = camera.GetViewMatrix();
glm::mat4 projection = glm::mat4(1.0f);

glm::vec3 lightPosition(1.0f, 1.0f, -2.0f);
glm::vec3 lightColor = glm::vec3(1.0f);
glm::vec4 lightVector = glm::vec4(1.0f, 1.0f, 1.0f, 0.0f);

// 物体
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};
// 点光源
glm::vec3 pointLPosis[4] = {
glm::vec3(0.7f,  0.2f,  2.0f),
glm::vec3(2.3f, -3.3f, -4.0f),
glm::vec3(-4.0f,  2.0f, -12.0f),
glm::vec3(0.0f,  0.0f, -3.0f)
};
glm::vec3 pointLAmbis[4] = { glm::vec3(0.05f) , glm::vec3(0.05f) ,glm::vec3(0.05f) ,glm::vec3(0.05f) };
glm::vec3 pointLDiffs[4] = {
glm::vec3(1.0f,1.0f,1.0f),
glm::vec3(0.0f,0.0f,1.0f),
glm::vec3(0.0f,1.0f,0.0f),
glm::vec3(1.0f,0.0f,0.0f)
};
glm::vec3 pointLSpecs[4] = { glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f) };
float pointLc[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float pointLl[4] = { 0.09f, 0.09f, 0.09f, 0.09f };
float pointLq[4] = { 0.032f, 0.032f, 0.032f, 0.032f };

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
// 加载纹理 返回图片数据 1.纹理 2.路径 3.图片是否为RGB
unsigned char* loadTexture(GLuint& texture, const char* path, bool isRGB)
{
    int width, height, nrChannels;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
    if (data)
    {
        if (isRGB)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        if (!isRGB)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    return data;
}
// 平行光
// shader名 方向 颜色
void setDirLight(Shader Shader, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular)
{
    Shader.setVec3("dirLight.direction", direction);
    Shader.setVec3("dirLight.ambient", ambient);
    Shader.setVec3("dirLight.diffuse", diffuse);
    Shader.setVec3("dirLight.specular", specular);
}
// 点光源
// shader名 光源数量 位置 颜色 衰减1.0/0.09/0.032
void setPointLights(Shader Shader, unsigned int length, glm::vec3 Positions[], glm::vec3 ambient[], glm::vec3 diffuse[], glm::vec3 specular[], float constant[], float linear[], float quadratic[])
{
    for (unsigned int i = 0; i < length; i++)
    {
        Shader.setVec3("pointLights[" + std::to_string(i) + "].position", Positions[i]);
        Shader.setVec3("pointLights[" + std::to_string(i) + "].ambient", ambient[i]);
        Shader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", diffuse[i]);
        Shader.setVec3("pointLights[" + std::to_string(i) + "].specular", specular[i]);
        Shader.setFloat("pointLights[" + std::to_string(i) + "].constant", constant[i]);
        Shader.setFloat("pointLights[" + std::to_string(i) + "].linear", linear[i]);
        Shader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", quadratic[i]);
    }
}
// 聚光 
// shader名 位置 方向 颜色 内外圆锥 衰减1.0 0.09 0.032
void setSpotLight(Shader Shader, glm::vec3 position, glm::vec3 direction, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float cutOff, float outerCutOff, float constant, float linear, float quadratic)
{
    float inRa = glm::cos(glm::radians(cutOff));
    float outRa = glm::cos(glm::radians(outerCutOff));
    // SpotLight
    Shader.setVec3("spotLight.position", position);
    Shader.setVec3("spotLight.direction", direction);
    Shader.setFloat("spotLight.cutOff", inRa);
    Shader.setFloat("spotLight.outerCutOff", outRa);
    Shader.setFloat("spotLight.constant", constant);
    Shader.setFloat("spotLight.linear", linear);
    Shader.setFloat("spotLight.quadratic", quadratic);
    Shader.setVec3("spotLight.ambient", ambient);
    Shader.setVec3("spotLight.diffuse", diffuse);
    Shader.setVec3("spotLight.specular", specular);
}
// 聚光颜色随时间变化
void ChangeSpotLightColorWithTime()
{
    pointLDiffs[0] = glm::vec3(sin(glfwGetTime()), 0.0f, 0.0f);
    pointLDiffs[1] = glm::vec3(-sin(glfwGetTime()), sin(glfwGetTime()), 0.0f);
    pointLDiffs[2] = glm::vec3(sin(glfwGetTime()), -0.0f, -sin(glfwGetTime()));
    pointLDiffs[3] = glm::vec3(0.0f, -sin(glfwGetTime()), sin(glfwGetTime()));
}