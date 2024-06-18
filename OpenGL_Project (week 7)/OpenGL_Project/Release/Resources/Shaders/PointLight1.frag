#version 460 core

// Inputs from vertex shader
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

uniform sampler2D Texture0;

// Output
out vec4 FinalColor;

void main()
{
    // Final color
    FinalColor = vec4(0.0f, 0.0f, 1.0f, 1.0f);
}
