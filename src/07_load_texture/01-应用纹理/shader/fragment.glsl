#version 330 core
out vec4 FracColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D tex1;

void main()
{
  FracColor = texture(tex1, TexCoord);
}