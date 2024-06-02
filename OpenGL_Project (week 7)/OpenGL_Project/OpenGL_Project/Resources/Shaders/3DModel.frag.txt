#version 460 core

// Inputs from Vertex Shader
in vec3 FragNormal;
in vec2 FragTexCoords;
in vec3 FragPos;

// Uniform Inputs
uniform sampler2D Texture0;
// how strong ambient light is
uniform float AmbientStrength = 0.15f;
uniform vec3 AmbientColor = vec3(1.0f, 1.0f, 1.0f);

// diffuse
uniform vec3 LightColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 LightPos = vec3(-300.0f, 000.0f, 100.0f);

// specular
uniform vec3 CameraPos;
uniform float LightSpecularStrength = 1.0f;
uniform float ObjectShininess = 32.0f;

// Output
out vec4 FinalColor;

void main()
{
    // Ambient component
    vec3 Ambient = AmbientStrength * AmbientColor;
    
    // Light Direction
    vec3 Normal = normalize(FragNormal);
    vec3 LightDir = normalize(FragPos - LightPos);

    // diffuse component
    float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * LightColor;

    // specular component
    vec3 ReverseViewDir = normalize(CameraPos - FragPos);
    vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), ObjectShininess);
    vec3 Specular = LightSpecularStrength * SpecularReflectivity * LightColor;

    // combine the lighting components
    vec4 Light = vec4(Ambient + Diffuse + Specular, 1.0f);

    // Calculate the final color using the light
    FinalColor = Light * texture(Texture0, FragTexCoords);
}