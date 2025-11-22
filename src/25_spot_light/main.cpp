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
    printOpenGLInfo(true);
    // 设置视口
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



    // Shader
    Shader ourShader("./src/shader/vertex.glsl", "./src/shader/fragment.glsl");
    Shader lightShader("./src/shader/lightvs.glsl", "./src/shader/lightfs.glsl");

    BoxGeometry Box1(1.0, 1.0, 1.0);
    //SphereGeometry Box1(1.5f, 16, 16);
    SphereGeometry PointLight(0.2f);
    Camera cam1(glm::vec3(0.0f, 1.0f, 5.0f));



    // 纹理
    unsigned int tex0, tex1;
    stbi_set_flip_vertically_on_load(true);
    float borderColor[] = { 0.3f, 0.1f, 0.7f, 1.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

    loadTexture(tex0, "./static/texture/container2.png", false);
    loadTexture(tex1, "./static/texture/lighting_maps_specular_color.png", false);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex1);



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



    model = glm::mat4();
    model = glm::translate(model, lightPosition);
    model = glm::scale(model, glm::vec3(0.2f));

    

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
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);// 清除缓冲
        
        
        float radius = 10.0f;
        float camX = sin(glfwGetTime()) * radius;
        float camZ = cos(glfwGetTime()) * radius;

        // MVP
        model = glm::mat4(1.0f);
        view = camera.GetViewMatrix();
        projection = glm::mat4(1.0f);

        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        float rotate = glfwGetTime() * 0.2f;
        glm::qua<float> qu = glm::qua<float>(glm::vec3(rotate, rotate, rotate));
        model = glm::mat4_cast(qu);
        model = glm::mat4(1.0f);
        

        glm::vec3 diffuseColor = lightColor * glm::vec3(0.7f); // 降低影响
        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.4f); // 很低的影响


        // Use Shader
        ourShader.use();

        ourShader.setMat4("model", model);
        ourShader.setMat4("view", view);
        ourShader.setVec3("viewPos", camera.Position);
        ourShader.setMat4("projection", projection);

        ourShader.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
        ourShader.setInt("material.diffuse", 0);
        ourShader.setInt("material.specular", 1);
        ourShader.setFloat("material.shininess", 32.0f);
        
        ourShader.setVec3("light.position", camera.Position);
        ourShader.setVec3("light.direction", camera.Front);
        ourShader.setFloat("light.cutOff", glm::cos(glm::radians(12.5f)));
        ourShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));

        ourShader.setVec3("light.ambient", ambientColor);
        ourShader.setVec3("light.diffuse", diffuseColor);
        ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        ourShader.setFloat("light.constant", 1.0f);
        ourShader.setFloat("light.linear", 0.09f);
        ourShader.setFloat("light.quadratic", 0.032f);

        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);

            float angle = 10.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            ourShader.setMat4("model", model);

            glBindVertexArray(Box1.VAO);
            glDrawElements(GL_TRIANGLES, Box1.indices.size(), GL_UNSIGNED_INT, 0);
        }



        model = glm::mat4(1.0f);
        model = glm::translate(model, lightPosition);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        
        diffuseColor = lightColor * glm::vec3(1.0f); 
        ambientColor = diffuseColor * glm::vec3(0.9f);

        lightShader.use();
        lightShader.setMat4("model", model);
        lightShader.setMat4("view", view);
        lightShader.setMat4("projection", projection);

        lightShader.setVec3("light.position", lightPosition);
        lightShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
        lightShader.setVec3("light.diffuse", 1.0f, 1.0f, 1.0f);
        lightShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

        glBindVertexArray(PointLight.VAO);
        glDrawElements(GL_TRIANGLES, PointLight.indices.size(), GL_UNSIGNED_INT, 0);

        // Update
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    // Dispose
    Box1.dispose();
    PointLight.dispose();
    glfwTerminate();
    return 0;
}