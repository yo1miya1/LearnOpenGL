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



// 初始化shader路径
string Shader::dirName = "";


// 函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void printAngle(float _angle);

glm::mat4 transMat(glm::mat4 _mat,  glm::vec3 _rotAixs, float _angle, glm::vec3 _scale, glm::vec3 _pos);
void resetTransMats(glm::mat4 _mat, glm::vec3 _rotAixs, float _angle, glm::vec3 _scale, glm::vec3 _pos);