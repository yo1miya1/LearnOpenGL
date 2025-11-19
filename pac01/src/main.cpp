#include "main.h"

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
    GLFWwindow* window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", NULL, NULL);
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
    // 设置视口
    glViewport(0, 0, screenWidth, screenHeight);
    glEnable(GL_PROGRAM_POINT_SIZE);
    //glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);// 开启深度测试
    //glDepthFunc(GL_LESS);// 设置深度测试方式为小于
    // 打印OpenGL版本信息
    cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
    // 打印GPU信息
    cout << "GPU: " << glGetString(GL_RENDERER) << endl;



    // Shader
    // 路径中的 " . " 指代工程目录，是 ".vcxproj" 文件所在目录
    Shader ourShader("./src/shader/vertex.glsl", "./src/shader/fragment.glsl");

    PlaneGeometry Plane1(1.0, 1.0);
    BoxGeometry Box1(1.0, 1.0, 1.0);


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


    glm::vec3 cubePositions[] = {
          glm::vec3(0.0f,  0.0f,  0.0f),
          glm::vec3(2.0f,  5.0f, -15.0f),
          glm::vec3(-1.5f, -2.2f, -2.5f),
          glm::vec3(-3.8f, -2.0f, -12.3f),
          glm::vec3(2.4f, -0.4f, -3.5f),
          glm::vec3(-1.7f,  3.0f, -7.5f),
          glm::vec3(1.3f, -2.0f, -2.5f),
          glm::vec3(1.5f,  2.0f, -2.5f),
          glm::vec3(1.5f,  0.2f, -1.5f),
          glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    

    // 渲染循环
    while (!glfwWindowShouldClose(window))
    {
        // input
        processInput(window);

        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        // 清楚缓冲
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 使用着色器
        ourShader.use();
        
        // 绑定 texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex1);


        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -5.0f));

        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(45.0f), screenWidth /screenHeight, 0.1f, 100.0f);
        
        ourShader.setMat4("view", view);
        ourShader.setMat4("projection", projection);

        glBindVertexArray(Box1.VAO);
        for (unsigned int i = 0; i < 10; i++)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            if (i % 3 == 0)
                angle = glfwGetTime() * 25.0f;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
            ourShader.setMat4("model", model);
            glDrawElements(GL_TRIANGLES, Box1.indices.size(), GL_UNSIGNED_INT, 0);
        }
        // 更新窗口
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // 手动释放资源
    Box1.dispose();
    glfwTerminate();
    return 0;
}


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
glm::mat4 Mat4_MVP( float M_angle,      // 模型角度
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


