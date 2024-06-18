#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

// output to frag shader
out vec3 FragTexCoords;

void main()
{
    vec4 Pos = projection * view * vec4(aPos, 1.0);
    gl_Position = Pos.xyww;
    FragTexCoords = vec3(aPos.xyz);  
}
