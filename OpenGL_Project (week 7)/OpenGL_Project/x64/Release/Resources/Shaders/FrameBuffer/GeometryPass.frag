#version 460 core

layout(location = 0) out vec4 Texture_Position;
layout(location = 1) out vec4 Texture_Normal;
layout(location = 2) out vec4 Texture_AlbedoShininess;

// Inputs from Vertex Shader
in vec3 FragNormal;
in vec2 FragTexCoords;
in vec3 FragPos;

// uniform inputs
uniform sampler2D Texture0;
uniform float ObjectShininess;

// output
out vec4 FinalColor;

void main()
{
	Texture_Position = vec4(FragPos, 1.0f);
	Texture_Normal = vec4(normalize(FragNormal), 1.0f);
	Texture_AlbedoShininess.rgb = texture(Texture0, FragTexCoords).rgb;
	Texture_AlbedoShininess.a = ObjectShininess;
}
