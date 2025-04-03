#version 460 core

// Output
out vec4 FinalColor;

// Uniform to control color
uniform vec3 Color;

in float Alpha;

void main()
{
    // Final color
    FinalColor = vec4(Color, Alpha);
}
