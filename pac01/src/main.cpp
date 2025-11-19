#include "main.h"

int main()
{
    // glfw初始化
    glfwInit();
    // 设置主要版本和次要版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // 创建窗口, 并设置回调函数
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    // 设置鼠标隐藏
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        cout << "Failed to initialize GLAD" << endl;
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);// 开启深度测试
    //glDepthFunc(GL_LESS);// 设置深度测试方式为小于
    printOpenGLInfo();
    // 设置视口
    //glEnable(GL_PROGRAM_POINT_SIZE);
    //glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    // Shader
    // 路径中的 " . " 指代工程目录，是 ".vcxproj" 文件所在目录
    Shader ourShader("./src/shader/vertex.glsl", "./src/shader/fragment.glsl");

    BoxGeometry Box1(1.0, 1.0, 1.0);

    Camera cam1(glm::vec3(0.0f, 1.0f, 5.0f));



    //声明纹理
    unsigned int tex0, tex1;
    glGenTextures(1, &tex0);
    glBindTexture(GL_TEXTURE_2D, tex0);
    //设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//缩小时使用临近
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大时使用线性
    stbi_set_flip_vertically_on_load(true);//图像y轴翻转
    //在渲染循环之前加载纹理
    int width, height, nrChannels;//nrChannles为图像的通道数，比如jpg是RGB三通道，png是RGBA四通道

    // 第一个纹理
    // 路径， 宽度， 高度， 通道数， 加载标志
    unsigned char* data = stbi_load("./static/texture/container.jpg", &width, &height, &nrChannels, 0);
    // 检查并生成纹理
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);}
    else {std::cout << "WARNING::TEXTURE0 DON`T LOAD!" << std::endl;}
    // 释放图像内存
    stbi_image_free(data);

    // 第二个纹理
    glGenTextures(1, &tex1);
    glBindTexture(GL_TEXTURE_2D, tex1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    data = stbi_load("./static/texture/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data){
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);}
    else {cout << "WARNING::TEXTURE1 DON`T LOAD!" << endl;}
    // 释放图像内存
    stbi_image_free(data);
    


    // 传参
    ourShader.use();
    ourShader.setInt("tex0", 0);
    ourShader.setInt("tex1", 1);



    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        // DeltaTime
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        // input
        processInput(window);
        // render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 清楚缓冲
        // Bind texture
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, tex0);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, tex1);
        // Use Shader
        ourShader.use();
        // MVP
        ourShader.setMat4("model", model);
        view = camera.GetViewMatrix();
        ourShader.setMat4("view", view);
        projection = glm::perspective(glm::radians(camera.Zoom), SCR_WIDTH / SCR_HEIGHT, 0.1f, 100.0f);
        ourShader.setMat4("projection", projection);
        // Bind Draw
        glBindVertexArray(Box1.VAO);
        glDrawElements(GL_TRIANGLES, Box1.indices.size(), GL_UNSIGNED_INT, 0);
        // Update
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Dispose
    Box1.dispose();
    glfwTerminate();
    return 0;
}