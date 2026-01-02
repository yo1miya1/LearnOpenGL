#include "main.h"


// main
int main()
{
    // glfw初始化
    glfwInit();
    // 设置主要版本和次要版本
    const char* glsl_version = "#version 330";

    // 片段着色器将作用域每一个采样点（采用4倍抗锯齿，则每个像素有4个片段（四个采样点））
    //glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    // 创建窗口, 并设置回调函数
    GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    OpenGLSettings();

    // 鼠标键盘事件
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);// 1.注册窗口变化监听
    glfwSetCursorPosCallback(window, mouse_callback);// 2.鼠标事件
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);// 设置鼠标隐藏


    Shader sceneShader("./src/shader/scene_vert.glsl", "./src/shader/scene_frag.glsl");
    Shader lightObjectShader("./src/shader/light_object_vert.glsl", "./src/shader/light_object_frag.glsl");
    Shader postShader("./src/shader/postShader_vert.glsl", "./src/shader/postShader_frag.glsl");

    PlaneGeometry planeGeometry(10.0, 10.0, 10.0, 10.0);
    PlaneGeometry glassGeometry(1.0, 1.0, 1.0);
    PlaneGeometry frameGeometry(2.0, 2.0);               // 窗口平面
    BoxGeometry boxGeometry(1.0, 1.0, 1.0);
    SphereGeometry sphereGeometry(0.04, 10.0, 10.0);
    SphereGeometry sphereGeometry2(0.5, 50.0, 50.0);

    unsigned int woodMap = loadTexture("./static/texture/wood.png");           // 地面
    unsigned int brickMap = loadTexture("./static/texture/brick_diffuse.jpg"); // 砖块
    unsigned int windowMap = loadTexture("./static/texture/blending_transparent_window.png"); // 窗户

    sceneShader.use();
    sceneShader.setInt("brickMap", 0);
    sceneShader.setInt("windowMap", 1);

    float factor = 0.0;
    float currentFrame;

    // 旋转矩阵
    glm::mat4 ex = glm::eulerAngleX(45.0f);
    glm::mat4 ey = glm::eulerAngleY(45.0f);
    glm::mat4 ez = glm::eulerAngleZ(45.0f);

    glm::mat4 qularXYZ = glm::eulerAngleXYZ(45.0f, 45.0f, 45.0f);

    float fov = 45.0f; // 视锥体的角度
    glm::vec3 view_translate = glm::vec3(0.0, 0.0, -5.0);
    //ImVec4 clear_color = ImVec4(25.0 / 255.0, 25.0 / 255.0, 25.0 / 255.0, 1.0); // 25, 25, 25

    glm::vec3 lightPosition = glm::vec3(1.0, 2.5, 2.0); // 光照位置

    // 传递材质属性
    sceneShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    sceneShader.setFloat("material.shininess", 32.0f);

    // 设置平行光光照属性
    sceneShader.setVec3("directionLight.ambient", 0.01f, 0.01f, 0.01f);
    sceneShader.setVec3("directionLight.diffuse", 0.2f, 0.2f, 0.2f); // 将光照调暗了一些以搭配场景
    sceneShader.setVec3("directionLight.specular", 1.0f, 1.0f, 1.0f);

    // 设置衰减
    sceneShader.setFloat("light.constant", 1.0f);
    sceneShader.setFloat("light.linear", 0.09f);
    sceneShader.setFloat("light.quadratic", 0.032f);

    // 点光源的位置
    glm::vec3 pointLightPositions[] = {
        glm::vec3(0.7f, 1.0f, 1.5f),
        glm::vec3(2.3f, 3.0f, -4.0f),
        glm::vec3(-4.0f, 2.0f, 1.0f),
        glm::vec3(1.4f, 2.0f, 0.3f) };
    // 点光源颜色
    glm::vec3 pointLightColors[] = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::vec3(0.0f, 1.0f, 0.0f) };

    // 窗户位置
    vector<glm::vec3> windowsPosWS
    {
        glm::vec3(-1.5f, 0.0f, -0.48f),
        glm::vec3(1.5f, 0.0f, 0.51f),
        glm::vec3(0.0f, 0.0f, 0.7f),
        glm::vec3(-0.3f, 0.0f, -2.3f),
        glm::vec3(0.5f, 0.0f, -0.6f)
    };

    // FrameBuffer
    unsigned int frameBuffer, texColorBuffer;//创建帧缓冲和颜色缓冲
    SetFrameBuffer(frameBuffer, texColorBuffer);

    // Render Loop
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        // 初始化
        Init(sceneShader, factor, currentFrame, frameBuffer);

        ChangeLightColor();

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, woodMap);

        float radius = 5.0f;
        float camX = sin(glfwGetTime() * 0.5) * radius;
        float camZ = cos(glfwGetTime() * 0.5) * radius;

        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(fov), (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT, 0.1f, 100.0f);

        glm::vec3 lightPos = glm::vec3(lightPosition.x * glm::sin(glfwGetTime()) * 2.0, lightPosition.y, lightPosition.z);

        sceneShader.setMat4("view", view);
        sceneShader.setMat4("projection", projection);

        sceneShader.setVec3("directionLight.direction", lightPos); // 光源位置
        sceneShader.setVec3("viewPos", camera.Position);

        pointLightPositions[0].z = camZ;
        pointLightPositions[0].x = camX;

        SetPointLights(sceneShader, pointLightPositions, pointLightColors);
        // 画地板
        DrawFloorPlane(sceneShader, planeGeometry);
        // 画不透明砖块
        DrawBaseBlockBox(brickMap, model, sceneShader, boxGeometry);
        // 画模板描边
        DrawStencilOutline(sceneShader, model, boxGeometry);
        // 画半透明窗户
        DrawTranslucencyWindow(glassGeometry, windowMap, windowsPosWS, model, sceneShader);
        // 画灯光
        DrawLightings(lightObjectShader, view, projection, model, lightPos, sphereGeometry, pointLightPositions, pointLightColors);
        // 后期处理
        PostEffect(postShader, frameGeometry, texColorBuffer);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Dispose
    boxGeometry.dispose();
    planeGeometry.dispose();
    sphereGeometry.dispose();
    glfwTerminate();

    return 0;
}

// 函数
// // 窗口变动监听
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
// 键盘输入监听
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
    // 相机按键控制
    // 相机移动
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(FORWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(LEFT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(RIGHT, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(DOWN, deltaTime);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
    {
        camera.ProcessKeyboard(UP, deltaTime);
    }
}
// 鼠标移动监听
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}
// 加载纹理贴图
unsigned int loadTexture(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    // 图像y轴翻转
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
// 打印OpenGL信息
void printOpenGLInfo(const bool printInfo)
{
    if (printInfo)
    {
        cout << "OpenGL Version: " << glGetString(GL_VERSION) << endl;
        cout << "GPU: " << glGetString(GL_RENDERER) << endl;
    }
}

void SetPointLights(Shader& sceneShader, glm::vec3  pointLightPositions[4], glm::vec3  pointLightColors[4])
{
    for (unsigned int i = 0; i < 4; i++)
    {

        // 设置点光源属性
        sceneShader.setVec3("pointLights[" + std::to_string(i) + "].position", pointLightPositions[i]);
        sceneShader.setVec3("pointLights[" + std::to_string(i) + "].ambient", 0.01f, 0.01f, 0.01f);
        sceneShader.setVec3("pointLights[" + std::to_string(i) + "].diffuse", pointLightColors[i]);
        sceneShader.setVec3("pointLights[" + std::to_string(i) + "].specular", 1.0f, 1.0f, 1.0f);

        // // 设置衰减
        sceneShader.setFloat("pointLights[" + std::to_string(i) + "].constant", 1.0f);
        sceneShader.setFloat("pointLights[" + std::to_string(i) + "].linear", 0.09f);
        sceneShader.setFloat("pointLights[" + std::to_string(i) + "].quadratic", 0.032f);
    }
}

// 修改光源颜色
void ChangeLightColor()
{
    lightColor.x = sin(glfwGetTime() * 2.0f);
    lightColor.y = sin(glfwGetTime() * 0.7f);
    lightColor.z = sin(glfwGetTime() * 1.3f);
}

// 绘制地板
void DrawFloorPlane(Shader& shader, PlaneGeometry& floorGeo)
{
    glStencilMask(0x00);

    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));

    shader.setFloat("uvScale", 4.0f);
    shader.setMat4("model", model);

    glBindVertexArray(floorGeo.VAO);
    glDrawElements(GL_TRIANGLES, floorGeo.indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

// 绘制窗户
void DrawTranslucencyWindow(PlaneGeometry& TransGeo, unsigned int Tex, std::vector<glm::vec3>& posWS, glm::mat4& model, Shader& shader)
{
    glDisable(GL_CULL_FACE);
    glBindVertexArray(TransGeo.VAO);
    glBindTexture(GL_TEXTURE_2D, Tex);
    // 对透明物体动态排序
    map<float, glm::vec3> sorted;
    for (unsigned int i = 0; i < posWS.size(); i++)
    {
        float distance = glm::length(camera.Position - posWS[i]);
        sorted[distance] = posWS[i];
    }
    for (std::map<float, glm::vec3>::reverse_iterator iterator = sorted.rbegin(); iterator != sorted.rend(); iterator++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, iterator->second);
        shader.setMat4("model", model);
        glDrawElements(GL_TRIANGLES, TransGeo.indices.size(), GL_UNSIGNED_INT, 0);
    }
    glEnable(GL_CULL_FACE);
}

// 绘制灯光物体
void DrawLightings(Shader& shader, glm::mat4& view, glm::mat4& projection, glm::mat4& model, glm::vec3& lightPos, SphereGeometry& sphereGeometry, glm::vec3  pointLightPositions[4], glm::vec3  pointLightColors[4])
{
    // ************************************************************
    shader.use();
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);

    model = glm::mat4(1.0f);
    model = glm::translate(model, lightPos);

    shader.setMat4("model", model);
    shader.setVec3("lightColor", glm::vec3(1.0f, 1.0f, 1.0f));

    glBindVertexArray(sphereGeometry.VAO);
    glDrawElements(GL_TRIANGLES, sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0);

    for (unsigned int i = 0; i < 4; i++)
    {
        model = glm::mat4(1.0f);
        model = glm::translate(model, pointLightPositions[i]);

        shader.setMat4("model", model);
        shader.setVec3("lightColor", pointLightColors[i]);

        glBindVertexArray(sphereGeometry.VAO);
        glDrawElements(GL_TRIANGLES, sphereGeometry.indices.size(), GL_UNSIGNED_INT, 0);
    }
    // ************************************************************
}

// 绘制砖块
void DrawBaseBlockBox(unsigned int Tex, glm::mat4& model, Shader& shader, BoxGeometry& boxGeo)
{
    //-------------------------------------------------------------
    // 正常绘制对象写入缓冲区
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glStencilMask(0xff);

    glBindTexture(GL_TEXTURE_2D, Tex);
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0, 1.0, -1.0));
    model = glm::scale(model, glm::vec3(2.0, 2.0, 2.0));

    shader.setFloat("uvScale", 1.0f);
    shader.setMat4("model", model);

    glBindVertexArray(boxGeo.VAO);
    glDrawElements(GL_TRIANGLES, boxGeo.indices.size(), GL_UNSIGNED_INT, 0);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0, 0.5, 2.0));
    shader.setMat4("model", model);

    glBindVertexArray(boxGeo.VAO);
    glDrawElements(GL_TRIANGLES, boxGeo.indices.size(), GL_UNSIGNED_INT, 0);

}

// 画模板描边
void DrawStencilOutline(Shader& shader, glm::mat4& model, BoxGeometry& boxGeo)
{
    // 绘制放大的版本，然后禁用模板写入
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilMask(0x00);
    glDisable(GL_DEPTH_TEST);

    shader.setFloat("stencil", 1.0);
    float scale = 1.03;
    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(1.0, 1.0, -1.0));
    model = glm::scale(model, glm::vec3(2.0 * scale, 2.0 * scale, 2.0 * scale));

    shader.setFloat("uvScale", 1.0f);
    shader.setMat4("model", model);

    glBindVertexArray(boxGeo.VAO);
    glDrawElements(GL_TRIANGLES, boxGeo.indices.size(), GL_UNSIGNED_INT, 0);

    model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(-1.0, 0.5, 2.0));
    model = glm::scale(model, glm::vec3(scale, scale, scale));
    shader.setMat4("model", model);

    glBindVertexArray(boxGeo.VAO);
    glDrawElements(GL_TRIANGLES, boxGeo.indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
    glStencilMask(0xff);
    glStencilFunc(GL_ALWAYS, 1, 0xff);
    glEnable(GL_DEPTH_TEST);
    shader.setFloat("stencil", 0.0);
    //-------------------------------------------------------------
}
//Cull
void Cull(bool use = true)
{
    if (use)
    {
        glEnable(GL_CULL_FACE);//CullFace
        glCullFace(GL_BACK);//CullFront
    }
    else
        glDisable(GL_CULL_FACE);
}
//StengilTest
void StencilTest()
{
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
}
//DepthTest
void DepthTest()
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}
void ClearStencil()
{
    glClear(GL_STENCIL_BUFFER_BIT);
}
void ClearDepth()
{
    DepthTest();
    glClear(GL_DEPTH_BUFFER_BIT);
}
//ClearColor
void ClearColor()
{
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}
void Init(Shader& shader, float& factor, float& cBUFFER, unsigned int& FBUFFER)
{
    cBUFFER = glfwGetTime();
    deltaTime = cBUFFER - lastTime;
    lastTime = cBUFFER;

    shader.use();

    factor = glfwGetTime();
    shader.setFloat("factor", -factor * 0.3);

    glBindFramebuffer(GL_FRAMEBUFFER, FBUFFER);


    //清除缓冲
    ClearColor();
    ClearDepth();
    ClearStencil();
}

void OpenGLSettings()
{
    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    DepthTest();
    StencilTest();
    Cull();

    printOpenGLInfo(true);
    stbi_set_flip_vertically_on_load(true);
}

void SetFrameBuffer(unsigned int& FBUFFER, unsigned int& TEXCOLORBUFFER)
{

    glGenFramebuffers(1, &FBUFFER);
    glBindFramebuffer(GL_FRAMEBUFFER, FBUFFER);
    // 生成纹理

    glGenTextures(1, &TEXCOLORBUFFER);
    glBindTexture(GL_TEXTURE_2D, TEXCOLORBUFFER);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SCREEN_WIDTH, SCREEN_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    // 将它附加到当前绑定的帧缓冲对象
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TEXCOLORBUFFER, 0);
    unsigned int renderBuffer;
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SCREEN_WIDTH, SCREEN_HEIGHT);//处理深度和模板
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;

    glBindFramebuffer(GL_FRAMEBUFFER, 0);//恢复默认的缓冲
}

void PostEffect(Shader& shader, PlaneGeometry& frameGeo, unsigned int TEXCOLORBUFFER)
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);// 返回默认的帧缓冲对象
    glDisable(GL_DEPTH_TEST);// 关闭深度测试

    ClearColor();

    shader.use();

    glBindVertexArray(frameGeo.VAO);
    glBindTexture(GL_TEXTURE_2D, TEXCOLORBUFFER);
    glDrawElements(GL_TRIANGLES, frameGeo.indices.size(), GL_UNSIGNED_INT, 0);
}