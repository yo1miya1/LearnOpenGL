#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>

#include <tool/shader.h>
#include <tool/camera.h>
#include <geometry/BoxGeometry.h>
#include <geometry/PlaneGeometry.h>
#include <geometry/SphereGeometry.h>

#include <map>

#define STB_IMAGE_IMPLEMENTATION
#include <tool/stb_image.h>

using namespace std;

//int SCREEN_WIDTH = 1280;
//int SCREEN_HEIGHT = 720;
int SCREEN_WIDTH = 1920;
int SCREEN_HEIGHT = 1080;

// camera
// camera value
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// delta time
float deltaTime = 0.001f;
float lastTime = 0.0f;
float fov = 55.0f;

// 鼠标上一帧的位置
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;
bool firstMouse = true;

Camera camera(glm::vec3(0.0, 1.0, 6.0));

glm::vec4 clear_color = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
// MVP
glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = camera.GetViewMatrix();
glm::mat4 projection = glm::mat4(1.0f);

glm::vec3 lightColor;

void OpenGLSettings();

void Cull(bool use);

void StencilTest();

void DepthTest();

void ClearStencil();

void ClearDepth();

void ClearColor();

unsigned int loadTexture(char const* path);

void SetFrameBuffer(unsigned int& FBUFFER, unsigned int& TEXCOLORBUFFER);

void PostEffect(Shader& shader, PlaneGeometry& frameGeo, unsigned int TEXCOLORBUFFER);

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

void processInput(GLFWwindow* window);

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);


void printOpenGLInfo(const bool printInfo);

void SetPointLights(Shader& sceneShader, glm::vec3 pointLightPositions[4], glm::vec3 pointLightColors[4]);

void Init(Shader& shader, float& factor, float& cBUFFER, unsigned int& FBUFFER);

void DrawFloorPlane(Shader& shader, PlaneGeometry& floorGeo);

void DrawTranslucencyWindow(PlaneGeometry& TransGeo, unsigned int Tex, std::vector<glm::vec3>& posWS, glm::mat4& model, Shader& shader);

void ChangeLightColor();

void DrawLightings(Shader& shader, glm::mat4& view, glm::mat4& projection, glm::mat4& model, glm::vec3& lightPos, SphereGeometry& sphereGeometry, glm::vec3 pointLightPositions[4], glm::vec3 pointLightColors[4]);

void DrawBaseBlockBox(unsigned int Tex, glm::mat4& model, Shader& shader, BoxGeometry& boxGeo);

void DrawStencilOutline(Shader& shader, glm::mat4& model, BoxGeometry& boxGeo);