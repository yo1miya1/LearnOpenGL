#pragma once
#include "glad/glad.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

void printAngle(float _angle)
{
    std::cout << _angle << std::endl;
}
void printOpenGLInfo()
{
    if (printInfo)
    {
        std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
        std::cout << "GPU: " << glGetString(GL_RENDERER) << std::endl;
    }
}



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