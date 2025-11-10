//2025.11.10 修改注释内容 与 加载纹理对象
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "tool/shader.h"//加载着色器所需的头文件
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "tool/stb_image.h"//加载纹理所需的头文件

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

// 设置路径
std::string Shader::dirName = "";

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
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }



    // Shader
    // 路径中的 " . " 指代工程目录，是 ".vcxproj" 文件所在目录
    Shader ourShader("./src/shader/vertex.glsl", "./src/shader/fragment.glsl");



    // 顶点数据
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };


    // VBO, VAO, EBO的创建、绑定
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // 绑定VAO
    glBindVertexArray(VAO);
    // 绑定VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 绑定EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);


    // 设置顶点位置属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 设置顶点颜色属性指针
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 顶点纹理坐标属性指针
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);







    //声明纹理
    unsigned int texture0, texture1;
    glGenTextures(1, &texture0);
    glBindTexture(GL_TEXTURE_2D, texture0);
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
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "WARNING::TEXTURE0 DON`T LOAD!" << std::endl;
    }
    //释放图像内存
    stbi_image_free(data);

    //第二个纹理
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    //设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//缩小时使用临近
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, /*GL_LINEAR*/GL_NEAREST);//放大时使用线性
    //路径， 宽度， 高度， 通道数， 加载标志
    data = stbi_load("./static/texture/awesomeface.png", &width, &height, &nrChannels, 0);
    //检查并生成纹理
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "WARNING::TEXTURE1 DON`T LOAD!" << std::endl;
    }
    //释放图像内存
    stbi_image_free(data);

    ourShader.use();
    ourShader.setInt("tex0", 0);
    ourShader.setInt("tex1", 1);






    //渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);
        
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture1);
        
        // render contaniner 
        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //更新窗口
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // 释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // glfw销毁
    glfwTerminate();
    return 0;
}



//处理按键输入
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

//窗口大小变化回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}