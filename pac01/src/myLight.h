/*
依赖于以下头文件 

#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "tool/shader.h"

需要在片元着色器中定义并以下结构
    定向光源
    struct DirLight
    {
        vec3 direction;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
    }
    uniform DirLight dirLight;

    点光源 
    struct PointLight
    {
    vec3 positions;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float constant;
    float linear;
    float quadratic;
    }
        单
    uniform PointLight pointLight;
        多
    uniform PointLight pointLights[num];

    聚光光源
    struct SpotLight
    {
        vec3 position;
        vec3 direction;
        float cutOff;
        float outerCutOff;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float constant;
        float linear;
        float quadratic;
    }
    uniform SpotLight spotLight;

*/

// 定向光源
// v3: 方向 环境光 散射光 镜面光
// 构造时只传方向，或是全部传入
struct DirLight
{
    glm::vec3 direction;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    DirLight() : direction(glm::vec3(0.0f, -1.0f, 0.0f)), ambient(glm::vec3(0.05f)), diffuse(glm::vec3(0.4f)), specular(glm::vec3(1.0f)) {}
    DirLight(glm::vec3 dir) : direction(dir), ambient(glm::vec3(0.05f)), diffuse(glm::vec3(0.4f)), specular(glm::vec3(1.0f)) {}
    DirLight(glm::vec3 dir, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec) : direction(dir), ambient(amb), diffuse(diff), specular(spec) {}

    // 平行光
    // shader名 
    void setDirLight(Shader Shader)
    {
        Shader.setVec3("dirLight.direction", this->direction);
        Shader.setVec3("dirLight.ambient", this->ambient);
        Shader.setVec3("dirLight.diffuse", this->diffuse);
        Shader.setVec3("dirLight.specular", this->specular);
    }
};



// 点光源
// 位置 环境光 散射光 镜面光 常量1.0f 线性0.09f 二次方0.032f
struct PointLight
{
    glm::vec3 positions;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
};
// 点光源 多光源
// shader名 光源结构 光源数量
void setPointLights(Shader Shader, PointLight light, unsigned int num)
{
    Shader.setVec3("pointLights[" + std::to_string(num) + "].position", light.positions);
    Shader.setVec3("pointLights[" + std::to_string(num) + "].ambient", light.ambient);
    Shader.setVec3("pointLights[" + std::to_string(num) + "].diffuse", light.diffuse);
    Shader.setVec3("pointLights[" + std::to_string(num) + "].specular", light.specular);
    Shader.setFloat("pointLights[" + std::to_string(num) + "].constant", light.constant);
    Shader.setFloat("pointLights[" + std::to_string(num) + "].linear", light.linear);
    Shader.setFloat("pointLights[" + std::to_string(num) + "].quadratic", light.quadratic);
}
// 点光源 单光源
// shader名 光源结构
void setPointLight(Shader Shader, PointLight light)
{
    Shader.setVec3("pointLights.position", light.positions);
    Shader.setVec3("pointLights.ambient", light.ambient);
    Shader.setVec3("pointLights.diffuse", light.diffuse);
    Shader.setVec3("pointLights.specular", light.specular);
    Shader.setFloat("pointLights.constant", light.constant);
    Shader.setFloat("pointLights.linear", light.linear);
    Shader.setFloat("pointLights.quadratic", light.quadratic);
}



// 聚光光源
// 位置 方向 内外圆锥 环境光 散射光 镜面光 常量1.0f 线性0.09f 二次方0.032f
struct SpotLight
{
    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float constant;
    float linear;
    float quadratic;
};
// 聚光 
// shader名 聚光结构
void setSpotLight(Shader Shader, SpotLight light)
{
    float inRa = glm::cos(glm::radians(light.cutOff));
    float outRa = glm::cos(glm::radians(light.outerCutOff));
    // SpotLight
    Shader.setVec3("spotLight.position", light.position);
    Shader.setVec3("spotLight.direction", light.direction);
    Shader.setFloat("spotLight.cutOff", inRa);
    Shader.setFloat("spotLight.outerCutOff", outRa);
    Shader.setFloat("spotLight.constant", light.constant);
    Shader.setFloat("spotLight.linear", light.linear);
    Shader.setFloat("spotLight.quadratic", light.quadratic);
    Shader.setVec3("spotLight.ambient", light.ambient);
    Shader.setVec3("spotLight.diffuse", light.diffuse);
    Shader.setVec3("spotLight.specular", light.specular);
}
// 聚光颜色随时间变化
void ChangeSpotLightColorWithTime(glm::vec3 pointLDiffs[])
{
    pointLDiffs[0] = glm::vec3(sin(glfwGetTime()), 0.0f, 0.0f);
    pointLDiffs[1] = glm::vec3(-sin(glfwGetTime()), sin(glfwGetTime()), 0.0f);
    pointLDiffs[2] = glm::vec3(sin(glfwGetTime()), -0.0f, -sin(glfwGetTime()));
    pointLDiffs[3] = glm::vec3(0.0f, -sin(glfwGetTime()), sin(glfwGetTime()));
}