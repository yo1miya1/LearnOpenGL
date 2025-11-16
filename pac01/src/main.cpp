//#include "main.h"

//#pragma once
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




// 初始化shader路径
string Shader::dirName = "";


// 函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void printAngle(float _angle);

glm::mat4 transMat(glm::mat4 _mat, glm::vec3 _rotAixs, float _angle, glm::vec3 _scale, glm::vec3 _pos);
void resetTransMats(glm::mat4 _mat, glm::vec3 _rotAixs, float _angle, glm::vec3 _scale, glm::vec3 _pos);


// 定义
void processInput(GLFWwindow* window)// 处理按键输入
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height)//窗口大小变化回调函数
{
    glViewport(0, 0, width, height);
}
void printAngle(float _angle)
{
    cout << _angle << endl;
}
glm::mat4 transMat(glm::mat4 _mat, glm::vec3 _rotAixs, float _angle, glm::vec3 _scale, glm::vec3 _pos)
{
    _mat = glm::translate(_mat, _pos);
    _mat = glm::rotate(_mat, glm::radians(_angle), _rotAixs);
    _mat = glm::scale(_mat, _scale);
    return _mat;
}
void resetTransMats(glm::mat4 _mat, glm::vec3 _rotAixs, float _angle, glm::vec3 _scale, glm::vec3 _pos)
{
    _angle = 0.0f;
    _mat = glm::mat4(1.0f);
    _rotAixs = glm::vec3(0.0f, 0.0f, 1.0f);
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _pos = glm::vec3(0.0f, 0.0f, 0.0f);
}


int main()
{
    // glfw初始化
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // 创建窗口, 并设置回调函数
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    // Shader
    // 路径中的 " . " 指代工程目录，是 ".vcxproj" 文件所在目录
    Shader ourShader("./src/shader/vertex.glsl", "./src/shader/fragment.glsl");

    PlaneGeometry Plane1;



    


    //声明纹理
    unsigned int tex0, tex1;
    glGenTextures(1, &tex0);
    glBindTexture(GL_TEXTURE_2D, tex0);
    //设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//缩小时使用临近
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大时使用线性
    //图像y轴翻转
    stbi_set_flip_vertically_on_load(true);
    //在渲染循环之前加载纹理
    int width, height, nrChannels;//nrChannles为图像的通道数，比如jpg是RGB三通道，png是RGBA四通道
    //路径， 宽度， 高度， 通道数， 加载标志
    unsigned char* data = stbi_load("./static/texture/container.jpg", &width, &height, &nrChannels, 0);
    //检查并生成纹理
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);}
    else {
        std::cout << "WARNING::TEXTURE0 DON`T LOAD!" << std::endl;}
    //释放图像内存
    stbi_image_free(data);

    //第二个纹理
    glGenTextures(1, &tex1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    //设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//缩小时使用临近
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, /*GL_LINEAR*/GL_NEAREST);//放大时使用线性
    //路径， 宽度， 高度， 通道数， 加载标志
    data = stbi_load("./static/texture/awesomeface.png", &width, &height, &nrChannels, 0);
    //检查并生成纹理
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);}
    else{
        cout << "WARNING::TEXTURE1 DON`T LOAD!" << endl;}
    //释放图像内存
    stbi_image_free(data);
    
    // 传参
    ourShader.use();
    ourShader.setInt("tex0", 0);
    ourShader.setInt("tex1", 1);
    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // 绑定 texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex1);

        // 使用着色器
        ourShader.use();

        glBindVertexArray(Plane1.VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glDrawElements(GL_POINTS, 6, GL_UNSIGNED_INT, 0);

        // 更新窗口
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
    // 手动释放资源
    Plane1.dispose();
    glfwTerminate();// glfw销毁
    return 0;
}