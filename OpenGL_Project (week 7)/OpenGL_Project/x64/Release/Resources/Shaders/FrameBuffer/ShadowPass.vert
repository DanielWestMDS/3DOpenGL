#version 460 core

// vertex data interpretation
layout(location = 0) in vec3 Position;

// Inputs
uniform mat4 LightVP;
uniform mat4 ModelMatrix;

void main()
{
    gl_Position = LightVP * ModelMatrix * vec4(Position, 1.0f);
}
