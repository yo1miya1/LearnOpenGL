#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
using namespace std;

//回调函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

int main()
{
    //实例化窗口
    glfwInit();
    //设置OpenGL版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    //设置OpenGL profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //创建窗口
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        //释放资源
        glfwTerminate();
        return -1;
    }
    //设置窗口上下文
    glfwMakeContextCurrent(window);

    //加载GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }

    //渲染循环
    while (!glfwWindowShouldClose(window))//窗口是否关闭
    {
        //输入处理
        processInput(window);//处理输入

        //渲染指令
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//设置清屏背景颜色
        glClear(GL_COLOR_BUFFER_BIT);//清除颜色缓冲

        glfwPollEvents();//检查是否有事件发生，并调用相应的回调函数
    }

    //释放资源
    glfwTerminate();
    return 0;
}

//输入处理
void processInput(GLFWwindow* window)
{
    //处理输入 按键为 esc 退出程序
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        //glfw设置窗口应该退出
        glfwSetWindowShouldClose(window, true);//退出程序，true
}

//窗口大小变化回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    //设置视口大小
    glViewport(0, 0, width, height);
}