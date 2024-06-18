#version 460 core

// Vertex Data Interpretation
layout (location = 0) in vec3 Position;
layout (location = 2) in vec3 Normal;
layout (location = 1) in vec2 TexCoords;

// Inputs
uniform mat4 ModelMat;
uniform mat4 VP;

uniform vec3 SecondColor;
uniform float CurrentTime;

// Outputs to Fragment Shader
out vec3 FragPos;
out vec2 FragTexCoords;
out vec3 FragNormal;

// Shader Functionality
void main()
{
	gl_Position = VP * ModelMat * vec4(Position, 1.0f);
	FragTexCoords = TexCoords;
	FragPos = vec3(ModelMat * vec4(Position, 1.0f));
	FragNormal = mat3(transpose(inverse(ModelMat))) * Normal;
	//FragColor = mix(Color, SecondColor, (sin(CurrentTime) * cos(CurrentTime) + 0.5);
} 