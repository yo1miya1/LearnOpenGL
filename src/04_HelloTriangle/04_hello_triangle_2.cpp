#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

using namespace std;


//回调函数声明
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

//窗口大小变化回调函数
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

//顶点着色器源码
const char* vertexShaderSource =    " #version 330 core\n"
                                    "layout (location = 0) in vec3 aPos;\n"
                                    "void main()\n"
                                    "{\n"
                                    "gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0f);\n"
                                    "gl_PointSize = 5.0f;\n"
                                    "}\n";

//片段着色器源码
const char* fragmentShaderSource = " #version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n"
                                   "}\n";


//main函数
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
    //设置视口大小
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //设置回调函数//需要设置回调函数，不然窗口大小变化时，无法更新视口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    //定义一个float数组，表示空间中的三角形
    float vertices[] = {
         0.5f,  0.5f, 0.0f,  // top right
         0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f
    };
    unsigned int indexes[] = {
        0, 1, 3,  // first Triangle
        1, 2, 3     //第二个三角形
    };

    //创建VBO,VAO
    unsigned int VBO, VAO, EBO;
    //生成VAO
    glGenVertexArrays(1, &VAO);
    //生成VBO
    glGenBuffers(1, &VBO);
    //生成EBO
    glGenBuffers(1, &EBO);

    //绑定VAO
    glBindVertexArray(VAO);

    //绑定VBO缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //填充VBO数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //绑定EBO对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //填充EBO数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

    //设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //解绑VAO
    glBindVertexArray(0);


    //创建顶点和片元着色器
    //声明着色器对象
    unsigned int vertexShader, fragmentShader;
    //创建顶点着色器对象
    vertexShader    = glCreateShader(GL_VERTEX_SHADER);
    //创建片元着色器对象
    fragmentShader  = glCreateShader(GL_FRAGMENT_SHADER);

    //附加着色器源码
    //着色器对象，数量，源码，长度
    glShaderSource  (vertexShader, 1, &vertexShaderSource, NULL);
    //编译vertexShader
    glCompileShader (vertexShader);
    //着色器对象，数量，源码，长度
    glShaderSource  (fragmentShader, 1, &fragmentShaderSource, NULL);
    // 编译vertexShader
    glCompileShader (fragmentShader);
    

    //检查编译结果
    //编译结果是否成功
    int success;
    //编译信息
    char infoLog[512];
    //获取编译结果
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << endl;
    }
    //获取编译结果
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << endl;
    }


    //创建着色器程序
    //声明着色器程序对象
    unsigned int shaderProgram;
    //创建着色器程序对象
    shaderProgram = glCreateProgram();
    //附加着色器
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    //链接着色器程序
    glLinkProgram(shaderProgram);


    //获取链接结果
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << endl;
    }


    //使用着色器程序
    glUseProgram(shaderProgram);
    //删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //线框模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    //渲染循环
    //窗口是否关闭
    while (!glfwWindowShouldClose(window))
    {
        //输入处理
        processInput(window);


        //渲染指令
        //设置清屏背景颜色
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        //清除颜色缓冲
        glClear(GL_COLOR_BUFFER_BIT);

        //绑定VBO
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);

        //DrawCall
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        //解绑VAO
        glBindVertexArray(0);

        //检查并调用事件，交换缓冲
        //交换缓冲
        glfwSwapBuffers(window);
        //检查是否有事件发生，并调用相应的回调函数
        glfwPollEvents();
    }

    //释放资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    //释放GLFW资源
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
