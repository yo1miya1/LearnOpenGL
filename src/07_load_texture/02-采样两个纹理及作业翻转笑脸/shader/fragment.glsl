#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D tex0;
uniform sampler2D tex1;

void main()
{
	vec2 uv0 = TexCoord;
	vec2 uv1 = vec2(TexCoord.x * -1, TexCoord.y);
	vec4 col0 = texture(tex0, uv0);
	vec4 col1 = texture(tex1, uv1);
	FragColor = mix(col0, col1, 0.2);
}