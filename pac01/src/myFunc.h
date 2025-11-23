#pragma once
#ifndef MYSHADERFUNC_H
#define MYSHADERFUNC_H

#include "GLFW/glfw3.h"
#include "tool/shader.h"



// 加载纹理 返回图片数据 
// 1.纹理 2.路径 3.图片是否为RGB
// 启用纹理 glActiveTexture(GL_TEXTURE);  
// 绑定纹理 glBindTexture(GL_TEXTURE_2D, GLuint& texture); 
void loadTexture(GLuint& texture, const char* path, bool isRGB)
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
}



// 平行光
// shader名 方向 颜色
void setDirLight(Shader Shader, DirLight light)
{
    Shader.setVec3("dirLight.direction", light.direction);
    Shader.setVec3("dirLight.ambient", light.ambient);
    Shader.setVec3("dirLight.diffuse", light.diffuse);
    Shader.setVec3("dirLight.specular", light.specular);
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
void ChangeSpotLightColorWithTime(glm::vec3 pointLDiffs[])
{
    pointLDiffs[0] = glm::vec3(sin(glfwGetTime()), 0.0f, 0.0f);
    pointLDiffs[1] = glm::vec3(-sin(glfwGetTime()), sin(glfwGetTime()), 0.0f);
    pointLDiffs[2] = glm::vec3(sin(glfwGetTime()), -0.0f, -sin(glfwGetTime()));
    pointLDiffs[3] = glm::vec3(0.0f, -sin(glfwGetTime()), sin(glfwGetTime()));
}
#endif