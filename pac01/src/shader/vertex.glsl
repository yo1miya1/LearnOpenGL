#version 330 core
layout(location = 0) in vec3 Position;
layout(location = 1) in vec3 Normal;
layout(location = 2) in vec2 UV;

out vec3 outNormal;
out vec3 outFragPos;
out vec2 outUV;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main() {

  gl_Position = projection * view * model * vec4(Position, 1.0f);

  outNormal = mat3(transpose(inverse(model))) * Normal;
  outFragPos = vec3(model * vec4(Position, 1.0));
  outUV = UV;
}