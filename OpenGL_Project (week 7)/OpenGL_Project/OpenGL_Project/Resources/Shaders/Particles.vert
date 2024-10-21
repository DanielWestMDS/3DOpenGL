#version 460 core

layout (location = 0) in vec4 Position;

uniform mat4 VP;

out float Alpha; 

void main()
{
	Alpha = Position.w;
	gl_Position = VP * vec4(Position.xyz, 1.0f);
}
