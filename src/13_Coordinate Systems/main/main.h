#pragma once
// OpenGL 初始化库
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "tool/shader.h"// Shader
// glm 数学库
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// 系统库
#include <iostream>
using namespace std;
// 纹理
#define STB_IMAGE_IMPLEMENTATION
#include "tool/stb_image.h"// stb_image

#include "geometry/PlaneGeometry.h"
#include "geometry/BoxGeometry.h"


float screenWidth = 800.0f;
float screenHeight = 600.0f;


// 初始化shader路径
string Shader::dirName = "";
bool Shader::lookShaderPath = false;

// 函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void printAngle(float _angle);

glm::mat4 transMat( glm::mat4 _mat,  // 变换矩阵
                    glm::vec3 _rotAixs, // 旋转轴
                    float _angle, // 旋转角度
                    glm::vec3 _scale, // 缩放比例
                    glm::vec3 _pos);// 位置
void resetTransMats(glm::mat4 _mat, // 变换矩阵
                    glm::vec3 _rotAixs, // 旋转轴
                    float _angle, // 旋转角度
                    glm::vec3 _scale, // 缩放比例
                    glm::vec3 _pos);// 位置

glm::mat4 Mat4_MVP(float M_angle,// 模型角度
    glm::vec3 M_Aixs,// 旋转轴
    glm::vec3 V_pos,// 视角位置
    float P_angle,// 视角角度
    float P_width,// 屏幕宽
    float P_height,// 屏幕高
    float P_Near,// 近裁平面
    float P_Far);// 远裁平面