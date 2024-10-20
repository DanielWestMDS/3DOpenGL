#version 460 core

#define MAX_POINT_LIGHTS 4

// Point light structure
struct PointLight
{
    vec3 Position;
    vec3 Color;
    float SpecularStrength;
    float AttenuationConstant;
    float AttenuationLinear;
    float AttenuationExponent;
};

// Directional light structure
struct DirectionalLight
{
    vec3 Direction;
    vec3 Color;
    float SpecularStrength;
};

// Inputs from Vertex Shader
in vec2 FragTexCoords;

// Uniforms
uniform sampler2D Texture_Position;
uniform sampler2D Texture_Normal;
uniform sampler2D Texture_AlbedoShininess;

uniform vec3 CameraPos;

uniform PointLight PointLightArray[MAX_POINT_LIGHTS];
uniform int PointLightCount;
uniform bool bPointLightOn;

uniform DirectionalLight DirectionLight;

// Ambient light settings
uniform float AmbientStrength = 0.3f;
uniform vec3 AmbientColor = vec3(1.0f, 1.0f, 1.0f);

// Output
out vec4 FinalColor;

// Point light calculation
vec3 CalculateLight_Point(int i, vec3 _FragPos, vec3 _FragNormal, float ObjectShininess)
{
    vec3 Normal = normalize(_FragNormal);
    vec3 LightDir = normalize(PointLightArray[i].Position - _FragPos);

    // Diffuse component
    float DiffuseStrength = max(dot(Normal, LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * PointLightArray[i].Color;

    // Specular component
    vec3 ReverseViewDir = normalize(CameraPos - _FragPos);
    vec3 HalfwayVector = normalize(LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), ObjectShininess);
    vec3 Specular = PointLightArray[i].SpecularStrength * SpecularReflectivity * PointLightArray[i].Color;

    // Combine diffuse and specular
    vec3 Light = Diffuse + Specular;

    // Attenuation
    float Distance = length(PointLightArray[i].Position - _FragPos);
    float Attenuation = PointLightArray[i].AttenuationConstant + 
                        PointLightArray[i].AttenuationLinear * Distance + 
                        PointLightArray[i].AttenuationExponent * pow(Distance, 2);
    Light /= Attenuation;

    return Light;
}

// Directional light calculation
vec3 CalculateLight_Direction(vec3 _FragPos, vec3 _FragNormal, float ObjectShininess)
{
    vec3 Normal = normalize(_FragNormal);
    vec3 LightDir = normalize(DirectionLight.Direction);

    // Diffuse component
    float DiffuseStrength = max(dot(Normal, LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * DirectionLight.Color;

    // Specular component
    vec3 ReverseViewDir = normalize(CameraPos - _FragPos);
    vec3 HalfwayVector = normalize(LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), ObjectShininess);
    vec3 Specular = DirectionLight.SpecularStrength * SpecularReflectivity * DirectionLight.Color;

    // Combine diffuse and specular
    return Diffuse + Specular;
}

void main()
{
    // Fetch data from G-buffer textures
    vec3 FragPos = texture(Texture_Position, FragTexCoords).xyz;
    vec3 FragNormal = normalize(texture(Texture_Normal, FragTexCoords).xyz);
    vec3 FragAlbedo = texture(Texture_AlbedoShininess, FragTexCoords).rgb;
    float ObjectShininess = texture(Texture_AlbedoShininess, FragTexCoords).a;

    // Ambient light component
    vec3 Ambient = AmbientStrength * AmbientColor;

    // Total light contribution
    vec3 TotalLightOutput = Ambient;

    // Directional light contribution
    TotalLightOutput += CalculateLight_Direction(FragPos, FragNormal, ObjectShininess);

    // Point light contribution (if enabled)
    if (bPointLightOn)
    {
        for (int i = 0; i < PointLightCount; ++i)
        {
            TotalLightOutput += CalculateLight_Point(i, FragPos, FragNormal, ObjectShininess);
        }
    }

    // Final color, multiplied by the albedo (diffuse color) of the object
    FinalColor = vec4(TotalLightOutput * FragAlbedo, 1.0f);
}
