#version 460 core

#define MAX_POINT_LIGHTS 4

// Point light struct
struct PointLight
{
    vec3 Position;
    vec3 Color;
    float SpecularStrength;

    float AttenuationConstant;
    float AttenuationLinear;
    float AttenuationExponent;
};

// Directional light struct
struct DirectionalLight
{
    vec3 Direction;
    vec3 Color;
    float SpecularStrength;
};

// Inputs from Vertex Shader
in vec3 FragNormal;
in vec2 FragTexCoords;
in vec3 FragPos;
in float FragHeight;

// Uniform Inputs
uniform sampler2D TextureArray[4];
uniform float HeightLevels[3];
uniform float AmbientStrength = 0.3f;
uniform vec3 AmbientColor = vec3(1.0f, 1.0f, 1.0f);
uniform vec3 LightColor;
uniform vec3 LightPos;
uniform vec3 CameraPos;
uniform float LightSpecularStrength;
uniform float ObjectShininess;
uniform PointLight PointLightArray[MAX_POINT_LIGHTS];
uniform int PointLightCount;
uniform bool bPointLightOn;
uniform DirectionalLight DirectionLight;

out vec4 FinalColor;

// Function to calculate point light
vec3 CalculateLight_Point(int i)
{
    vec3 Normal = normalize(FragNormal);
    vec3 LightDir = normalize(FragPos - PointLightArray[i].Position);

    float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * PointLightArray[i].Color;

    vec3 ReverseViewDir = normalize(CameraPos - FragPos);
    vec3 HalfwayVector = normalize(LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), ObjectShininess);
    vec3 Specular = PointLightArray[i].SpecularStrength * SpecularReflectivity * LightColor;

    vec3 Light = Diffuse + Specular;

    float Distance = length(PointLightArray[i].Position - FragPos);
    float Attenuation = PointLightArray[i].AttenuationConstant + (PointLightArray[i].AttenuationLinear * Distance) + PointLightArray[i].AttenuationExponent * pow(Distance, 2);
    Light /= Attenuation;

    return Light;
}

// Function to calculate directional light
vec3 CalculateLight_Direction()
{
    vec3 Normal = normalize(FragNormal);
    vec3 LightDir = normalize(DirectionLight.Direction);

    float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * DirectionLight.Color;

    vec3 ReverseViewDir = normalize(CameraPos - FragPos);
    vec3 HalfwayVector = normalize(LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), ObjectShininess);
    vec3 Specular = DirectionLight.SpecularStrength * SpecularReflectivity * LightColor;

    vec3 Light = Diffuse + Specular;

    return Light;
}

void main()
{
    vec3 Ambient = AmbientStrength * AmbientColor;
    vec3 TotalLightOutput = Ambient;

    if (bPointLightOn)
    {
        TotalLightOutput += CalculateLight_Direction();
        for (int i = 0; i < PointLightCount; i++)
        {
            TotalLightOutput += CalculateLight_Point(i);
        }
    }

    vec4 texColor;
    if (FragHeight < HeightLevels[0])
    {
        texColor = texture(TextureArray[3], FragTexCoords);
    }
    else if (FragHeight < HeightLevels[1])
    {
        texColor = texture(TextureArray[1], FragTexCoords);
    }
    else if (FragHeight < HeightLevels[2])
    {
        texColor = texture(TextureArray[2], FragTexCoords);
    }
    else
    {
        texColor = texture(TextureArray[0], FragTexCoords);
    }

    FinalColor = vec4(TotalLightOutput, 1.0f) * texColor;
}
