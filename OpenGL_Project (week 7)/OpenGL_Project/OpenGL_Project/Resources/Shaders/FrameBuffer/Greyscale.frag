#version 330 core

// Input from vertex shader
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D Texture0;

void main()
{
    vec4 color = texture(Texture0, TexCoord);
    float luminance = 0.3 * color.r + 0.11 * color.g + 0.59 * color.b;
    FragColor = vec4(luminance, luminance, luminance, color.a);
}
