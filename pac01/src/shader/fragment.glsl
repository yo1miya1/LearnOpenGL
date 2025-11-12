#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec3 ourPos;
in vec2 uv0;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
  float xy = length(ourPos.xy);
  FragColor = mix(texture(tex0, uv0), 
			  vec4(ourColor, 1.0 - xy * 2.0) * 
			  texture(tex1, uv0), 0.4f);
}