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
const char* vertexShaderSource = " #version 330 core\n"
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
"FragColor = vec4(0.1f, 0.5f, 0.2f, 1.0f);\n"
"}\n";

const char* fragmentShaderSource2 = " #version 330 core\n"
"out vec4 FragColor;\n"
"uniform vec4 ourColor;\n"
"void main()\n"
"{\n"
"FragColor = ourColor;\n"
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
        return -1;}
    //设置窗口上下文
    glfwMakeContextCurrent(window);

    //加载GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;}
    //设置视口大小
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    glEnable(GL_PROGRAM_POINT_SIZE);

    //设置回调函数//需要设置回调函数，不然窗口大小变化时，无法更新视口大小
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

    //定义一个float数组，表示空间中的三角形
    float vertices[] = {-0.5f,  0.5f, 0.0f,
                         0.5f,  0.5f, 0.0f,
                         0.5f, -0.5f, 0.0f};

    float vertices2[] = { -0.5f,  0.5f, 0.0f,
                          -0.5f, -0.5f, 0.0f,
                           0.5f, -0.5f, 0.0f };
    unsigned int indexes[] = {0, 1, 2};

    //创建VBO,VAO
    unsigned int VBO, VAO, EBO, VAO2, VBO2, EBO2;
    //生成VAO
    glGenVertexArrays(1, &VAO);
    //生成VBO
    glGenBuffers(1, &VBO);
    //生成EBO
    glGenBuffers(1, &EBO);
    //生成VAO
    glGenVertexArrays(1, &VAO2);
    //生成VBO
    glGenBuffers(1, &VBO2);
    //生成EBO
    glGenBuffers(1, &EBO2);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //解绑VAO
    glBindVertexArray(0);

    //绑定VAO
    glBindVertexArray(VAO2);
    //绑定VBO缓冲
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    //填充VBO数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
    //绑定EBO对象
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO2);
    //填充EBO数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);
    //设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //解绑VAO
    glBindVertexArray(0);


    //创建顶点和片元着色器
    //声明着色器对象
    unsigned int vertexShader, fragmentShader, fragmentShader2;
    //创建着色器对象
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    fragmentShader2 = glCreateShader(GL_FRAGMENT_SHADER);

    //附加着色器源码
    //着色器对象，数量，源码，长度
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glShaderSource(fragmentShader2, 1, &fragmentShaderSource2, NULL);
    //编译Shader
    glCompileShader(vertexShader);
    glCompileShader(fragmentShader);
    glCompileShader(fragmentShader2);


    //检查编译结果
    //编译结果是否成功
    int success;
    //编译信息
    char infoLog[512];
    //获取编译结果
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" << endl;}
    //获取编译结果
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << endl;}
    
    //获取编译结果
    glGetShaderiv(fragmentShader2, GL_COMPILE_STATUS, &success);
    if (!success)
    {glGetShaderInfoLog(fragmentShader2, 512, NULL, infoLog);
        cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" << endl; }


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

    //声明着色器程序对象
    unsigned int shaderProgram2;
    //创建着色器程序对象
    shaderProgram2 = glCreateProgram();
    //附加着色器
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShader2);
    //链接着色器程序
    glLinkProgram(shaderProgram2);


    //获取链接结果
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << endl;}
    //获取链接结果
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram2, 512, NULL, infoLog);
        cout << "ERROR:SHADER::PROGRAM::LINKING_FAILED\n" << endl;}


    //随时间变化
    int ourColorLocation = glGetUniformLocation(shaderProgram2, "ourColor");
    //使用着色器程序
    glUseProgram(shaderProgram);
    glUseProgram(shaderProgram2);

    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glUseProgram(shaderProgram2);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        float timeValue = glfwGetTime();
        float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
        glUniform4f(ourColorLocation, greenValue / 0.5f, greenValue / 2.0f, greenValue + 0.1f, 1.0f);

        //检查并调用事件，交换缓冲
        //交换缓冲
        glfwSwapBuffers(window);
        //检查是否有事件发生，并调用相应的回调函数
        glfwPollEvents();
    }

    //释放资源
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShader2);

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO2);
    glDeleteBuffers(1, &VBO2);
    glDeleteBuffers(1, &EBO2);

    glDeleteProgram(shaderProgram);
    glDeleteProgram(shaderProgram2);
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
