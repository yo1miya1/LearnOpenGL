#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <tool/shader.h>
#include <tool/camera.h>
#include <geometry/BoxGeometry.h>
#include <geometry/PlaneGeometry.h>
#include <geometry/SphereGeometry.h>

#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>

using namespace std;

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;

// camera
// camera value
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// delta time
float deltaTime = 0.001f;
float lastTime = 0.0f;
float fov = 55.0f;

// 鼠标上一帧的位置
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

Camera camera(glm::vec3(0.0, 1.0, 6.0));

glm::vec4 clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
// MVP
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = camera.GetViewMatrix();
glm::mat4 projection = glm::mat4(1.0f);

glm::vec3 lightColor;

// 函数
// // 窗口变动监听
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
// 键盘输入监听
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    // 相机按键控制
    // 相机移动
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }
}
// 鼠标移动监听
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
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
// 加载纹理贴图
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    // 图像y轴翻转
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
// 打印OpenGL信息
void printOpenGLInfo(const bool printInfo)
{
    if (printInfo)
    {
        cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
        cout << "GPU: " << glGetString(GL_RENDERER) << endl;
    }
}

void SetPointLights(Shader& sceneShader, glm::vec3  pointLightPositions[4], glm::vec3  pointLightColors[4])
{
    for (unsigned int i = 0; i < 4; i++)
    {

        // 设置点光源属性
        sceneShader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
        sceneShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.01f, 0.01f, 0.01f);
        sceneShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", pointLightColors[i]);
        sceneShader.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);

        // // 设置衰减
        sceneShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
        sceneShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
        sceneShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
    }
}

// 修改光源颜色
void ChangeLightColor()
{
    lightColor.x = sin(glfwGetTime() * 2.0f);
    lightColor.y = sin(glfwGetTime() * 0.7f);
    lightColor.z = sin(glfwGetTime() * 1.3f);
}

// 绘制地板
void DrawFloorPlane(Shader& sceneShader, PlaneGeometry& planeGeometry)
{
    glStencilMask(0x00);

    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

    sceneShader.setFloat("uvScale", 4.0f);
    sceneShader.setMat4("model", model);

    glBindVertexArray(planeGeometry.VAO);
    glDrawElements(GL_TRIANGLES, planeGeometry.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// 绘制窗户
void DrawTranslucencyWindow(PlaneGeometry& GlassGeometry, unsigned int windowMap, std::vector<glm::vec3>& windowsPosWS, glm::mat4& model, Shader& sceneShader)
{
    glBindVertexArray(GlassGeometry.VAO);
    glBindTexture(GL_TEXTURE_2D, windowMap);
    // 对透明物体动态排序
    map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < windowsPosWS.size(); i++)
    {
        float distance = glm::length(camera.Position - windowsPosWS[i]);
        sorted[distance] = windowsPosWS[i];
    }
    for (std::map<float, glm::vec3>::reverse_iterator iterator = sorted.rbegin(); iterator != sorted.rend(); iterator++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, iterator->second);
        sceneShader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, GlassGeometry.indices.size(), GL_UNSIGNED_INT, 0);
    }
}

// 绘制灯光物体
void DrawLightings(Shader& lightObjectShader, glm::mat4& view, glm::mat4& projection, glm::mat4& model, glm::vec3& lightPos, SphereGeometry& sphereGeometry, glm::vec3  pointLightPositions[4], glm::vec3  pointLightColors[4])
{
    // ************************************************************
    lightObjectShader.use();
    lightObjectShader.setMat4("view", view);
    lightObjectShader.setMat4("projection", projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);

    lightObjectShader.setMat4("model", model);
    lightObjectShader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glBindVertexArray(sphereGeometry.VAO);
    glDrawElements(GL_TRIANGLES, sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0);

    for (unsigned int i = 0; i < 4; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);

        lightObjectShader.setMat4("model", model);
        lightObjectShader.setVec3("lightColor", pointLightColors[i]);

        glBindVertexArray(sphereGeometry.VAO);
        glDrawElements(GL_TRIANGLES, sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0);
    }
    // ************************************************************
}

// 绘制砖块
void DrawBaseBlockBox(unsigned int brickMap, glm::mat4& model, Shader& sceneShader, BoxGeometry& boxGeometry)
{
    //-------------------------------------------------------------
    // 正常绘制对象写入缓冲区
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xff);

    glBindTexture(GL_TEXTURE_2D, brickMap);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0, 1.0, -1.0));
    model = glm::scale(model, glm::vec3(2.0, 2.0, 2.0));

    sceneShader.setFloat("uvScale", 1.0f);
    sceneShader.setMat4("model", model);

    glBindVertexArray(boxGeometry.VAO);
    glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0, 0.5, 2.0));
    sceneShader.setMat4("model", model);

    glBindVertexArray(boxGeometry.VAO);
    glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);

}

// 画模板描边
void DrawStencilOutline(Shader& sceneShader, glm::mat4& model, BoxGeometry& boxGeometry)
{
    // 绘制放大的版本，然后禁用模板写入
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    sceneShader.setFloat("stencil", 1.0);
    float scale = 1.03;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0, 1.0, -1.0));
    model = glm::scale(model, glm::vec3(2.0 * scale, 2.0 * scale, 2.0 * scale));

    sceneShader.setFloat("uvScale", 1.0f);
    sceneShader.setMat4("model", model);

    glBindVertexArray(boxGeometry.VAO);
    glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0, 0.5, 2.0));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    sceneShader.setMat4("model", model);

    glBindVertexArray(boxGeometry.VAO);
    glDrawElements(GL_TRIANGLES, boxGeometry.indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glStencilMask(0xff);
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glEnable(GL_DEPTH_TEST);
    sceneShader.setFloat("stencil", 0.0);
    //-------------------------------------------------------------
}

// 清屏
void Init(Shader& sceneShader, float& factor, float& currentFrame)
{
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastTime;
    lastTime = currentFrame;

    sceneShader.use();

    factor = glfwGetTime();
    sceneShader.setFloat("factor", -factor * 0.3);

    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);//ClearColor
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//清除缓冲
}
