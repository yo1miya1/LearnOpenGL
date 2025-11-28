/*
依赖于以下头文件
#pragma once

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
*/

// 变换矩阵mat4 旋转轴v3 旋转角度radius(f) 缩放比例v3 位置v3
glm::mat4 transMat( glm::mat4 _mat,glm::vec3 _rotAixs,float _angle,glm::vec3 _scale,glm::vec3 _pos)      
{
    _mat = glm::translate(_mat, _pos);
    _mat = glm::rotate(_mat, glm::radians(_angle), _rotAixs);
    _mat = glm::scale(_mat, _scale);
    return _mat;
}
// 变换矩阵mat4 旋转轴v3 旋转角度radius(f) 缩放比例v3 位置v3
void resetTransMats(glm::mat4 _mat,glm::vec3 _rotAixs,float _angle,glm::vec3 _scale,glm::vec3 _pos)     
{
    _angle = 0.0f;
    _mat = glm::mat4(1.0f);
    _rotAixs = glm::vec3(0.0f, 0.0f, 1.0f);
    _scale = glm::vec3(1.0f, 1.0f, 1.0f);
    _pos = glm::vec3(0.0f, 0.0f, 0.0f);
}

// 模型角度radius(f) 旋转轴v3 视角位置v3 视角角度f 屏幕宽f 屏幕高f 近裁平面f 远裁平面f
glm::mat4 Mat4_MVP( float M_angle,glm::vec3 M_Aixs,glm::vec3 V_pos,float P_angle,float P_width,float P_height,float P_Near,float P_Far)        
{
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    model = glm::rotate(model, M_angle, M_Aixs);
    view = glm::translate(view, V_pos);
    projection = glm::perspective(P_angle, P_width / P_height, P_Near, P_Far);

    return projection * view * model;
}
