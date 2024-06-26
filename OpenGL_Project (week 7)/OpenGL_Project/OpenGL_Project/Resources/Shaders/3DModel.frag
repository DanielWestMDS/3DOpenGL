#version 460 core

#define MAX_POINT_LIGHTS 4
//
// point light 
struct PointLight
{
	vec3 Position;
	vec3 Color;
	float SpecularStrength;

    float AttenuationConstant;
    float AttenuationLinear;
    float AttenuationExponent;
};

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

// point light
uniform PointLight PointLightArray[MAX_POINT_LIGHTS];
uniform int PointLightCount;

// Output
out vec4 FinalColor;

vec3 CalculateLight_Point(int i)
{
    // Light Direction
    vec3 Normal = normalize(FragNormal);
    vec3 LightDir = normalize(FragPos - PointLightArray[i].Position);

    // diffuse component
    float DiffuseStrength = max(dot(Normal, -LightDir), 0.0f);
    vec3 Diffuse = DiffuseStrength * PointLightArray[i].Color;

    // specular component
    vec3 ReverseViewDir = normalize(CameraPos - FragPos);
    vec3 HalfwayVector = normalize(-LightDir + ReverseViewDir);
    float SpecularReflectivity = pow(max(dot(Normal, HalfwayVector), 0.0f), ObjectShininess);
    vec3 Specular = PointLightArray[i].SpecularStrength * SpecularReflectivity * LightColor;

    vec3 Light = Diffuse + Specular;

    //float Distance = length(PointLightArray[i].Position - FragPos);
    //float Attenuation = PointLightArray[i].AttenuationConstant + (PointLightArray[i].AttenuationLinear * Distance) + (PointLightArray[i].AttenuationExponent * pow(Distance, 2));
    //Light /= Attenuation;

    return Light;
}

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

       // point light
   vec3 TotalLightOutput = vec3(0.0f);
   for (int i = 0; i < PointLightCount; i++)
   {
       TotalLightOutput += CalculateLight_Point(i);
   }

    // combine the lighting components
    vec4 Light = vec4(Ambient + TotalLightOutput, 1.0f);

    // Calculate the final color using the light
    FinalColor = Light * texture(Texture0, FragTexCoords);
     FinalColor = vec4(TotalLightOutput, 1.0f) * texture(Texture0, FragTexCoords);
}