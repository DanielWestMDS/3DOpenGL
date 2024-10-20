#version 460 core

// Vertex Data Interpretation
layout (location = 0) in vec3 Position;
layout (location = 2) in vec3 Normal;
layout (location = 1) in vec2 TexCoords;
layout (location = 3) in vec3 instancePosition;

uniform mat4 ModelMat;
uniform mat4 VP;

// Outputs to Fragment Shader
out vec3 FragNormal;
out vec2 FragTexCoords;
out vec3 FragPos;

// Shader Functionality
void main()
{
    // Calculate final position
    gl_Position = VP * ModelMat * vec4(Position, 1.0f);

    // Pass data to fragment shader
    FragTexCoords = TexCoords;
    FragPos = vec3(ModelMat * vec4(Position, 1.0f));
    FragNormal = mat3(transpose(inverse(ModelMat))) * Normal;
}
