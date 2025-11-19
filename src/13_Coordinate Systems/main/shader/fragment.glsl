#version 330 core
out vec4 FragColor;

in vec2 uv0;

uniform sampler2D tex0;
uniform sampler2D tex1;

void main() {
	FragColor = mix(texture(tex0, uv0), texture(tex1, uv0), 0.2f);
}