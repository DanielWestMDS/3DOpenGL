#version 460 core

// Vertex Data Interpretation
layout (location = 0) in vec3 Position;
layout (location = 1) in vec2 TexCoords;

// Outputs to Fragment Shader
out vec2 FragTexCoords;

// Shader Functionality
void main()
{
    // Calculate final position
    gl_Position = vec4(Position, 1.0f);

    // Pass data to fragment shader
    FragTexCoords = TexCoords;
}
