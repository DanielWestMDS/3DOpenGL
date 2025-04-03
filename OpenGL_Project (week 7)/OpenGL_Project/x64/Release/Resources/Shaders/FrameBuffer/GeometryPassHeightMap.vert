#version 460 core

layout(location = 0) in vec3 Position;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 Normal;
layout(location = 3) in float Height;

out vec2 FragTexCoords;
out vec3 FragNormal;
out vec3 FragPos;
out float FragHeight;
out vec4 FragPos_LightSpace;

// Shadow
uniform mat4 VPLight;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(Position, 1.0f);
    FragTexCoords = TexCoord;
    FragNormal = mat3(transpose(inverse(model))) * Normal;
    FragPos = vec3(model * vec4(Position, 1.0f));

    // for shadows
    FragPos_LightSpace = VPLight * vec4(FragPos, 1.0f);

    // for textures at different heights
    FragHeight = Height;
}
