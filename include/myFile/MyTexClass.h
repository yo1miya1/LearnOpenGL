/*  依赖于以下头文件

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#define STB_IMAGE_IMPLEMENTATION
#include "tool/stb_image.h"

#include <iostream>
using namespace std;
#include <string>
*/

class MyTexClass {
private:
    unsigned int ID;        // 纹理ID
    int width, height;      // 纹理尺寸
    int nrChannels;         // 通道数
    bool isRGB;             // 是否为RGB格式
public:
    // 默认构造函数
    MyTexClass() : ID(0), width(0), height(0), nrChannels(0), isRGB(true) {}

    // 带路径的构造函数
    MyTexClass(const char* texturePath, bool rgbFormat = true) : isRGB(rgbFormat) {
        load(texturePath);
    }

    // 析构函数
    ~MyTexClass() {
        if (ID != 0) {
            glDeleteTextures(1, &ID);
        }
    }

    // 禁止拷贝（因为OpenGL资源需要管理）
    MyTexClass(const MyTexClass&) = delete;
    MyTexClass& operator=(const MyTexClass&) = delete;

    // 允许移动
    MyTexClass(MyTexClass&& other) noexcept
        : ID(other.ID), width(other.width), height(other.height),
        nrChannels(other.nrChannels), isRGB(other.isRGB) {
        other.ID = 0; // 防止原对象删除纹理
    }

    MyTexClass& operator=(MyTexClass&& other) noexcept {
        if (this != &other) {
            if (ID != 0) {
                glDeleteTextures(1, &ID);
            }
            ID = other.ID;
            width = other.width;
            height = other.height;
            nrChannels = other.nrChannels;
            isRGB = other.isRGB;
            other.ID = 0;
        }
        return *this;
    }
    // 加载纹理函数
    void load(const char* texturePath) {
        // 生成纹理
        glGenTextures(1, &ID);
        glBindTexture(GL_TEXTURE_2D, ID);

        // 设置纹理参数
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // 加载图像数据
        stbi_set_flip_vertically_on_load(true); // 翻转Y轴，OpenGL的纹理坐标原点在左下角
        unsigned char* data = stbi_load(texturePath, &width, &height, &nrChannels, 0);

        if (data) {
            GLenum format;
            if (nrChannels == 1)
                format = GL_RED;
            else if (nrChannels == 3)
                format = GL_RGB;
            else if (nrChannels == 4)
                format = GL_RGBA;

            if (isRGB && nrChannels >= 3) {
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            }
            else {
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            }

            glGenerateMipmap(GL_TEXTURE_2D);
            std::cout << "Texture loaded successfully: " << texturePath
                << " (" << width << "x" << height << ", channels: " << nrChannels << ")" << std::endl;
        }
        else {
            std::cout << "ERROR::TEXTURE " << ID << " DATA IS NULL!" << std::endl;
            std::cout << "           MAYBE PATH IS WRONG: " << texturePath << std::endl;
        }

        stbi_image_free(data);
    }
    // 绑定纹理到指定纹理单元
    void bind(unsigned int textureUnit = 0) const {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, ID);
    }

    // 解绑纹理
    static void unbind() {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    // 设置纹理参数
    void setParameter(GLenum param, GLenum value) {
        glBindTexture(GL_TEXTURE_2D, ID);
        glTexParameteri(GL_TEXTURE_2D, param, value);
    }

    // 获取纹理ID
    unsigned int getID() const { return ID; }

    // 获取纹理尺寸
    int getWidth() const { return width; }
    int getHeight() const { return height; }

    // 检查纹理是否有效
    bool isValid() const { return ID != 0; }
};