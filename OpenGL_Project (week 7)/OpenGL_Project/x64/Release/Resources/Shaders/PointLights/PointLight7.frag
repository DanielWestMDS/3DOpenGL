#version 460 core

// inputs from frag shader 
in vec2 FragTexCoords;
in vec3 FragNormal;
in vec3 FragPos;

uniform sampler2D Texture0;

// output
out vec4 FinalColor;

void main()
{
	// final color
	FinalColor = vec4(1.0f, 1.0f, 1.0f, 0.5f);
}