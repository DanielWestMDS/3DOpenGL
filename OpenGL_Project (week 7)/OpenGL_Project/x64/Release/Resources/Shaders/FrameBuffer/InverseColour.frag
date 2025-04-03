#version 330 core

// Input from vertex shader
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D Texture0;

void main()
{
    FragColor = 1 - texture(Texture0, TexCoord);
}
