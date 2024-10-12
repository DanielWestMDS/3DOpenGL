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
in vec4 FragPos_LightSpace;
in float FragHeight;

// Uniform Inputs
uniform sampler2D TextureArray[4];
uniform float HeightLevels[4];
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

// shadow
uniform sampler2D Texture_ShadowMap;

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

float CalculateShadow()
{
    // Perspective divide to get NDC range (-1 to 1)
    vec3 NDC_Space = FragPos_LightSpace.xyz / FragPos_LightSpace.w;

    // Convert to depth range (0 to 1)
    vec3 ProjCoordinates = 0.5f * NDC_Space + 0.5f;

	float shadowbias = 0.005f;

    // Get the depth of the current fragment from the lights perspective
    float CurrentDepth = ProjCoordinates.z - shadowbias;
	

	vec2 TexelSize = 1.0f/textureSize(Texture_ShadowMap,0);
	float Shadow = 0.0f;
	int Count = 0;
	for (int Row = -1; Row <= 1; Row++)
	{
		for (int Col = -1; Col <= 1; Col++)
		{
			vec2 TextureCoordOffset = ProjCoordinates.xy + vec2(Row, Col) * TexelSize;
			float PCF_Depth = texture(Texture_ShadowMap, TextureCoordOffset).x;
			Shadow += CurrentDepth > PCF_Depth ? 1.0f : 0.0f;
			Count++;
		}
	}
	
	Shadow /= float(Count);
    // Get the closest depth value from the lught perspective along the projection
    //float LightClosestDepth = texture(Texture_ShadowMap, ProjCoordinates.xy).r;

    //float Shadow = CurrentDepth > LightClosestDepth ? 1.0f : 0.0f;
    return Shadow;
}

void main()
{
    vec3 Ambient = AmbientStrength * AmbientColor;
    vec3 TotalLightOutput = Ambient;

    TotalLightOutput += CalculateLight_Direction();

    if (bPointLightOn)
    {
        for (int i = 0; i < PointLightCount; i++)
        {
            TotalLightOutput += CalculateLight_Point(i);
        }
    }

    // shadows
    float Shadow = CalculateShadow();
    vec3 LightShadow = Ambient + ((1.0f - Shadow) * TotalLightOutput);

    vec4 texColor;

    if (FragHeight < HeightLevels[0])
    {
        texColor = texture(TextureArray[0], FragTexCoords);
    }
    else if (FragHeight < HeightLevels[1])
    {
        float blendFactor = (FragHeight - HeightLevels[0]) / (HeightLevels[1] - HeightLevels[0]);
        texColor = mix(texture(TextureArray[0], FragTexCoords), texture(TextureArray[1], FragTexCoords), blendFactor);
    }
    else if (FragHeight < HeightLevels[2])
    {
        float blendFactor = (FragHeight - HeightLevels[1]) / (HeightLevels[2] - HeightLevels[1]);
        texColor = mix(texture(TextureArray[1], FragTexCoords), texture(TextureArray[2], FragTexCoords), blendFactor);
    }
    else if (FragHeight > 220)
    {
        texColor = texture(TextureArray[3], FragTexCoords);
    }
    else
    {
        float blendFactor = (FragHeight - HeightLevels[2]) / (255 - HeightLevels[2]);
        texColor = mix(texture(TextureArray[2], FragTexCoords), texture(TextureArray[3], FragTexCoords), blendFactor);
    }

    FinalColor = vec4(LightShadow, 1.0f) * texColor;
}
