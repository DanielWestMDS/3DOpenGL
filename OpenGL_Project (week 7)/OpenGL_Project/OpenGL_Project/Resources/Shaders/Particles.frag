#version 460 core

// Output
out vec4 FinalColor;

// Uniform to control color
uniform vec4 Color;

void main()
{
    // Final color
    FinalColor = Color;
}
