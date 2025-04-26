// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : Source.cpp
// Description : Main file for project. Contains all classes and runs an initial setup on startup and an update and a render each frame.
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#define STB_IMAGE_IMPLEMENTATION

#define TINYOBJLOADER_IMPLEMENTATION

//#include "Utils.h"
#include "ShaderLoader.h"
#include "CModel.h"
#include "CLightManager.h"
#include "CHeightMap.h"
#include "CScene.h"
#include "CFrameBufferQuad.h"
#include "CShadowMap.h"
#include "CParticleSystem.h"
#include "CGeometryBuffer.h"
#include "CTessellationMesh.h"
#include "CUserInterface.h"
#include "CObject.h"
#include "CContactListener.h"
#include "CEditorMode.h"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

//#include <reactphysics3d/reactphysics3d.h>
using namespace reactphysics3d;

// global variables
GLFWwindow* Window = nullptr;
GLuint Program_FixedTri;
int iWindowSize = 800;

// editor mode
bool g_bInEditor = true;

// pointer to shape objects
CCamera* Camera;
// tree
CModel* Tree;
// light manager
CLightManager* LightManager;
// point lights
CModel* PointLight1;
CModel* PointLight2;
CModel* PointLight3;
CModel* PointLight4;
CModel* PointLight5;
CModel* PointLight6;
CModel* PointLight7;
CModel* PointLight8;
CModel* PointLight9;
CModel* PointLight10;

// shadow model
CModel* Soldier;

// height map
CHeightMap* HeightMap;
CHeightMap* HeightMapNoise;

CFrameBufferQuad* FrameBufferQuad;

// Shadows
CShadowMap* ShadowMap;

// particle effects
CParticleSystem* RedFirework;
CParticleSystem* CyanFirework;
CParticleSystem* YellowFirework;
CParticleSystem* MagentaFirework;

// geometry buffer
CGeometryBuffer* GeometryBuffer;

// tessellation
CTessellationMesh* TessQuad;

// scenes
CScene* Scene3;
CScene* g_CurrentScene;

// programs
GLuint Program_3DModel;
GLuint Program_Lighting;
GLuint Program_InstancedLighting;

GLuint Program_PointLight1;
GLuint Program_PointLight2;
GLuint Program_PointLight3;
GLuint Program_PointLight4;
GLuint Program_PointLight5;
GLuint Program_PointLight6;
GLuint Program_PointLight7;
GLuint Program_PointLight8;
GLuint Program_PointLight9;
GLuint Program_PointLight10;

GLuint Program_HeightMap;
GLuint Program_Squares;
GLuint Program_ShadowMap;
// compute program for particles
GLuint Program_ComputeParticles;
GLuint Program_Particles;
GLuint Program_GeometryPass;
GLuint Program_GeometryPassHeightMap;
GLuint Program_LightingPass;
GLuint Program_LightingPassHeightMap;
GLuint Program_Tessellation;

// texture
GLuint Texture_Awesome;
GLuint Texture_Quag;
GLuint Texture_3;
GLuint Texture_4;
GLuint Texture_Noise;
GLuint Texture_PerlinMap;
GLuint Texture_RainNoise;
// textures for height map
GLint HeightMapTextures[4];

// Object Matrices and Components
// model to be combined with view and projection
glm::mat4 SoldierModelMat;

glm::vec3 SoldierPosition = glm::vec3(-20.0f, 6.0f, 8.0f);

glm::mat4 PLScaleMat;

// model to be combined with view and projection
glm::mat4 PLModelMat1;

// model to be combined with view and projection
glm::mat4 PLModelMat2;
glm::mat4 PLModelMat3;
glm::mat4 PLModelMat4;
glm::mat4 PLModelMat5;
glm::mat4 PLModelMat6;
glm::mat4 PLModelMat7;
glm::mat4 PLModelMat8;
glm::mat4 PLModelMat9;
glm::mat4 PLModelMat10;

glm::mat4 TreeModelMat;

glm::mat4 HeightMapModelMat;

glm::mat4 DeferredHeightMapModelMat;

glm::mat4 PerlinHeightMapModelMat;

glm::mat4 TessMatrix;

glm::mat4 TessMVP;

int x = 0;
int y = 0;

// obj count 
int g_objCount = 1000;

// time
float CurrentTime;
float PreviousTime;
float deltaTime;

// for input object
float MoveSpeed = 10.0f;

// toggle bools
bool g_bShowMousePosition = false;
bool g_bPointLightActive = true;
bool g_bShowMouse = true;
bool g_UIChange = false;
bool g_bWireframe = false;
bool g_bFKeyPressed = false;

// scene number 
int g_iSceneNumber = 1;
// renderbuffer program
int g_iProgram = 0;

// mouse position
glm::vec2 g_MousePos;

// Quad Model Mat values
glm::vec3 QuadPosition = glm::vec3(100.0f, 100.0f, 0.0f);
glm::mat4 QuadTranslationMat;

float QuadRotationAngle = 0.0f;
glm::mat4 QuadRotationMat;

glm::vec3 QuadScale = glm::vec3(2.0f, 2.0f, 2.0f);
glm::mat4 QuadScaleMat;

bool g_bIsObjectSelected = false;
CObject* SelectedObject = nullptr;

// Vector for instanced matrices
std::vector<glm::mat4> MVPVec;
// for random tree positions
std::vector<glm::vec3> RandomLocations;

// Define the six faces of the cube map in a vector
std::vector<std::string> sFaces = {
	"Resources/Textures/Right.png",
	"Resources/Textures/Left.png",
	"Resources/Textures/Top.png",
	"Resources/Textures/Bottom.png",
	"Resources/Textures/Back.png",
	"Resources/Textures/Front.png"
};

// physics 
PhysicsCommon g_physicsCommon;

int g_iPhysicsSteps = 20;

// create the physics world
PhysicsWorld* g_physicsWorld = g_physicsCommon.createPhysicsWorld();

/// <summary>
/// Creates a model matrix for objects in the scene
/// </summary>
/// <param name="_position"></param>
/// <param name="_scale"></param>
/// <param name="_rotationAngle"></param>
/// <param name="_rotationMat"></param>
/// <returns></returns>
glm::mat4 MakeModelMatrix(glm::vec3 _position, float _scale, float _rotationAngle, glm::vec3 _rotationMat)
{
	// calculate model matrix
	glm::mat4 TranslationMat = glm::translate(glm::mat4(1.0f), _position);
	glm::mat4 RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians(_rotationAngle), _rotationMat);
	glm::mat4 ScaleMat = glm::scale(glm::mat4(1.0f), glm::vec3(_scale, _scale, _scale));
	glm::mat4 OutputMat = TranslationMat * RotationMat * ScaleMat;

	//OutputMat = Camera->GetProjMat() * Camera->GetViewMat() * OutputMat;
	return OutputMat;
}

void CreateActor()
{
	glm::vec3 newActorPosition;
	// set new position to be in front of camera
	newActorPosition = Camera->GetPosition() + (Camera->GetForward() * -50.f);

	CModel* NewModel = new CModel("Resources/Models/SM_Prop_Statue_02.obj", Program_Lighting, Texture_Quag, newActorPosition, 0.15f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	CObject* NewObject = new CObject(NewModel, newActorPosition, g_physicsWorld, g_physicsCommon);

	// add the object to the scene
	g_CurrentScene->AddObject(NewObject);
}

// for position callback
void CursorPositionInput(GLFWwindow* _Window, double _PosX, double _PosY)
{
	// toggle whether to show mouse position
	if (g_bShowMousePosition)
	{
		std::cout << "Cursor Coords: " << _PosX << ", " << _PosY << std::endl;
	}

	g_MousePos = glm::vec2(_PosX, _PosY);

	if ((_PosX > 50 && _PosX < 150) && (_PosY > 50 && _PosY < 150))
	{
		g_UIChange = true;
	}
	else
	{
		g_UIChange = false;
	}

	ImGui_ImplGlfw_CursorPosCallback(Window, _PosX, _PosY);
}

// for single key press
void KeyInput(GLFWwindow* _Window, int _Key, int _ScanCode, int _Action, int _Mods)
{
	//// for sxene rendering
	//if (_Key == GLFW_KEY_1 && _Action == GLFW_PRESS)
	//{
	//	g_iSceneNumber = 1;
	//	g_CurrentScene = Scene1;
	//}

	//if (_Key == GLFW_KEY_2 && _Action == GLFW_PRESS)
	//{
	//	g_iSceneNumber = 2;
	//	g_CurrentScene = Scene2;
	//}

	if (_Key == GLFW_KEY_3 && _Action == GLFW_PRESS)
	{
		g_iSceneNumber = 3;
		g_CurrentScene = Scene3;
	}

	//if (_Key == GLFW_KEY_4 && _Action == GLFW_PRESS)
	//{
	//	g_iSceneNumber = 4;
	//	g_CurrentScene = Scene4;
	//}


	if (_Key == GLFW_KEY_5 && _Action == GLFW_PRESS)
	{
		//g_iSceneNumber = 5;
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(4.0f);
	}


	if (_Key == GLFW_KEY_6 && _Action == GLFW_PRESS)
	{
		//g_iSceneNumber = 6;
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	// for object
// move forward
	if (glfwGetKey(_Window, GLFW_KEY_UP))
	{
		// use camera forward
		SoldierPosition.x += deltaTime * MoveSpeed;
	}

	// move back
	if (glfwGetKey(_Window, GLFW_KEY_DOWN))
	{
		// use camera forward but reverse
		SoldierPosition.x -= deltaTime * MoveSpeed;
	}

	// move left
	if (glfwGetKey(_Window, GLFW_KEY_LEFT))
	{
		// use camera right but reverse
		SoldierPosition.z -= deltaTime * MoveSpeed;
	}

	// move right
	if (glfwGetKey(_Window, GLFW_KEY_RIGHT))
	{
		// use camera forward
		SoldierPosition.z += deltaTime * MoveSpeed;
	}


	// toggle lighting
	if (_Key == GLFW_KEY_P && _Action == GLFW_PRESS)
	{
		g_bPointLightActive = !g_bPointLightActive;
	}

	// create actor
	if (_Key == GLFW_KEY_O && _Action == GLFW_PRESS)
	{
		CreateActor();
	}

	// testing
	if (_Key == GLFW_KEY_I && _Action == GLFW_PRESS)
	{
		Scene3->MoveObjects();
	}

	// toggle wireframe
	if (_Key == GLFW_KEY_L && _Action == GLFW_PRESS)
	{
		g_bWireframe = !g_bWireframe;

		if (g_bWireframe)
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else
		{
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
	}

	// fireworks
	if (_Key == GLFW_KEY_F && _Action == GLFW_PRESS)
	{
		g_bFKeyPressed = true;
	}
	else
	{
		g_bFKeyPressed = false;
	}
}

/// <summary>
/// Converts an openGL vector3 into a reactphysics3d vector3
/// </summary>
/// <param name="_GLMVec"></param>
/// <returns></returns>
Vector3 GLMVec3ToReactVec3(glm::vec3 _GLMVec)
{
	Vector3 ReactVec3;
	ReactVec3.x = _GLMVec.x;
	ReactVec3.y = _GLMVec.y;
	ReactVec3.z = _GLMVec.z;

	return ReactVec3;
}

struct Raycast
{
	glm::vec3 Origin;
	glm::vec3 Direction;
};

Raycast GenerateRaycastFromMouse(float _mouseX, float _mouseY, int _screenWidth, int _screenHeight)
{
	glm::mat4 View = Camera->GetViewMat();
	glm::mat4 Proj = Camera->GetProjMat();

	float x = (2.0f * _mouseX) / _screenWidth - 1.0f;
	float y = 1.0f - (2.0f * _mouseY) / _screenHeight;
	glm::vec4 RayClip = glm::vec4(x, y, -1.0f, 1.0f);

	glm::vec4 RayEye = glm::inverse(Proj) * RayClip;
	RayEye = glm::vec4(RayEye.x, RayEye.y, -1.0f, 0.0f);

	glm::vec3 rayDirWorld = glm::normalize(glm::vec3(glm::inverse(View) * RayEye));
	glm::vec3 rayOrigin = Camera->GetPosition();

	return Raycast{ rayOrigin, rayDirWorld };
}

// axis aligned bounding box
bool RayIntersectsAABB(const glm::vec3& _rayOrigin, const glm::vec3& _rayDir, const glm::vec3& _aabbMin, const glm::vec3& _aabbMax, float& _tOut) 
{
	// minimum and maximum of the ray
	float tMin = 0.0f;
	float tMax = 10000.0f;

	for (int i = 0; i < 3; ++i) 
	{
		float invD = 1.0f / _rayDir[i];
		float t0 = (_aabbMin[i] - _rayOrigin[i]) * invD;
		float t1 = (_aabbMax[i] - _rayOrigin[i]) * invD;

		if (invD < 0.0f) std::swap(t0, t1);

		tMin = std::max(tMin, t0);
		tMax = std::min(tMax, t1);

		if (tMax < tMin) return false;
	}

	_tOut = tMin;
	return true;
}

void FindSelectedObject()
{
	// make t as big as possible to find the smallest t for closest object
	float fClosestT = FLT_MAX;
	Raycast MouseRay = GenerateRaycastFromMouse(g_MousePos.x, g_MousePos.y, iWindowSize, iWindowSize);

	for (CObject* Object : g_CurrentScene->GetObjects())
	{
		float t;

		if (RayIntersectsAABB(MouseRay.Origin, MouseRay.Direction, Object->GetModel()->GetWorldAABB().Min, Object->GetModel()->GetWorldAABB().Max, t))
		{
			if (t < fClosestT)
			{
				fClosestT = t;
				SelectedObject = Object;
				g_bIsObjectSelected = true;
			}
		}
	}
}



/// <summary>
/// Load an openGL texture from a filepath
/// </summary>
/// <param name="_filepath"></param>
/// <returns></returns>
GLuint LoadTexture(std::string _filepath)
{
	GLuint Texture;

	// Load Image data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	// create and bind new texture
	glGenTextures(1, &Texture);
	glBindTexture(GL_TEXTURE_2D, Texture);

	unsigned char* ImageData = stbi_load(_filepath.c_str(), &ImageWidth, &ImageHeight, &ImageComponents, 0);


	// Check if image is RGBA or RGB
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);

	// generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// free memory
	stbi_image_free(ImageData);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	return Texture;
}

/// <summary>
/// runs once at the beginning of the project
/// </summary>
void InitialSetup()
{
	// program for 3d model
	Program_3DModel = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/3DModel.frag");

	// program for lighting
	Program_Lighting = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/Lighting_BlinnPhong.frag");

	// program for lighting
	Program_InstancedLighting = ShaderLoader::CreateProgram("Resources/Shaders/InstancedArray_Standard.vert",
		"Resources/Shaders/Lighting_PointLights.frag");

	// program for point light 1
	Program_PointLight1 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLights/PointLight1.frag");

	// program for point light 2
	Program_PointLight2 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLights/PointLight2.frag");

	// program for point light 3
	Program_PointLight3 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLights/PointLight3.frag");

	// program for point light 4
	Program_PointLight4 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLights/PointLight4.frag");

	// program for point light 5
	Program_PointLight5 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLights/PointLight5.frag");

	// program for point light 6
	Program_PointLight6 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLights/PointLight6.frag");

	// program for point light 7
	Program_PointLight7 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLights/PointLight7.frag");

	// program for point light 8
	Program_PointLight8 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLights/PointLight8.frag");

	// program for point light 9
	Program_PointLight9 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLights/PointLight9.frag");

	// program for point light 10
	Program_PointLight10 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLights/PointLight10.frag");

	Program_HeightMap = ShaderLoader::CreateProgram("Resources/Shaders/HeightMap.vert",
		"Resources/Shaders/Lighting_HeightMap.frag");

	// for orthographic squares
	Program_Squares = ShaderLoader::CreateProgram("Resources/Shaders/Squares.vert",
		"Resources/Shaders/Squares.frag");

	// renderbuffer for shadow texture
	Program_ShadowMap = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/ShadowPass.vert",
		"Resources/Shaders/FrameBuffer/ShadowPass.frag");

	// particles
	Program_Particles = ShaderLoader::CreateProgram("Resources/Shaders/Particles.vert",
		"Resources/Shaders/Particles.frag");

	// particle compute shader
	Program_ComputeParticles = ShaderLoader::CreateProgram_C("ComputeParticles.comp");

	// geometry buffer
	Program_GeometryPass = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/GeometryPass.vert",
		"Resources/Shaders/FrameBuffer/GeometryPass.frag");

	Program_GeometryPassHeightMap = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/GeometryPassHeightMap.vert",
		"Resources/Shaders/FrameBuffer/GeometryPassHeightMap.frag");

	// screen space render pass buffer
	Program_LightingPass = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/LightingPass.vert",
		"Resources/Shaders/FrameBuffer/LightingPass.frag");

	Program_LightingPassHeightMap = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/LightingPassHeightMap.vert",
		"Resources/Shaders/FrameBuffer/LightingPassHeightMap.frag");

	// tessellation program
	Program_Tessellation = ShaderLoader::CreateProgram_VTF("Tessellation.vert", 
		"Tessellation.tcs", 
		"Tessellation.tes", 
		"Tessellation.frag");

	// flip image
	stbi_set_flip_vertically_on_load(true);

	Texture_Awesome = LoadTexture("Resources/Textures/SkyboxBack.jpg");
	Texture_Quag = LoadTexture("Resources/Textures/PolygonAncientWorlds_Texture_01_A.png");
	Texture_3 = LoadTexture("Resources/Textures/Halfrack logo.png");
	Texture_4 = LoadTexture("Resources/Textures/PolygonAncientWorlds_Statue_01.png");
	
	Texture_RainNoise = LoadTexture("Resources/Textures/ad56fba948dfba9ae698198c109e71f118a54d209c0ea50d77ea546abad89c57.png");
	HeightMapTextures[0] = Texture_Quag;
	HeightMapTextures[1] = Texture_Awesome;
	HeightMapTextures[2] = Texture_3;
	HeightMapTextures[3] = Texture_4;

	// calculate model matrix
	SoldierModelMat = MakeModelMatrix(SoldierPosition, 0.15f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// for point lights
	PLModelMat1 = MakeModelMatrix(glm::vec3(10.0f, 5.0f, 10.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// point light 2
	PLModelMat2 = MakeModelMatrix(glm::vec3(10.0f, 5.0f, 0.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// point light 3
	PLModelMat3 = MakeModelMatrix(glm::vec3(10.0f, 5.0f, -10.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// point light 4
	PLModelMat4 = MakeModelMatrix(glm::vec3(0.0f, 5.0f, 10.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
												  
	// point light 5							  
	PLModelMat5 = MakeModelMatrix(glm::vec3(0.0f, 5.0f, 0.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
												  
	// point light 6							  
	PLModelMat6 = MakeModelMatrix(glm::vec3(0.0f, 5.0f, -10.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// point light 7
	PLModelMat7 = MakeModelMatrix(glm::vec3(-10.0f, 5.0f, 10.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// point light 8
	PLModelMat8 = MakeModelMatrix(glm::vec3(-10.0f, 5.0f, 0.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// point light 9
	PLModelMat9 = MakeModelMatrix(glm::vec3(-10.0f, 5.0f, -10.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// point light 10
	PLModelMat10 = MakeModelMatrix(glm::vec3(-20.0f, 5.0f, 0.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// for instanced matrices
	TreeModelMat = MakeModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.005f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// for Perlin noise quad
	PerlinHeightMapModelMat = MakeModelMatrix(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	TessMatrix = MakeModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 100.0f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// initialise objects
	Camera = new CCamera();

	LightManager = new CLightManager();

	// change attenuation so you can see all the different point lights
	float exponent = 0.1f;

	// blue
	LightManager->AddPointLight(glm::vec3(10.0f, 5.0f, 10.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.4f, 0.045f, exponent);
	// green
	LightManager->AddPointLight(glm::vec3(10.0f, 5.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), 1.0f, 1.4f, 0.045f, exponent);
	// red
	LightManager->AddPointLight(glm::vec3(10.0f, 5.0f, -10.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.5f, 0.045f, exponent);
	// cyan
	LightManager->AddPointLight(glm::vec3(0.0f, 5.0f, 10.0f), glm::vec3(0.0f, 1.0f, 1.0f), 1.0f, 1.4f, 0.045f, exponent);
	// yellow
	LightManager->AddPointLight(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), 1.0f, 1.4f, 0.045f, exponent);
	// magenta
	LightManager->AddPointLight(glm::vec3(0.0f, 5.0f, -10.0f), glm::vec3(1.0f, 0.0f, 1.0f), 1.0f, 1.4f, 0.045f, exponent);
	// white
	LightManager->AddPointLight(glm::vec3(-10.0f, 5.0f, 10.0f), glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 1.6f, 0.045f, exponent);
	// purple?
	LightManager->AddPointLight(glm::vec3(-10.0f, 5.0f, 0.0f), glm::vec3(0.5f, 0.0f, 1.0f), 1.0f, 1.4f, 0.045f, exponent);
	// orange?
	LightManager->AddPointLight(glm::vec3(-10.0f, 5.0f, -10.0f), glm::vec3(1.0f, 0.5f, 0.5f), 1.0f, 1.4f, 0.045f, exponent);
	// dark blue
	LightManager->AddPointLight(glm::vec3(-20.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.5f), 1.0f, 1.4f, 0.045f, exponent);

	FrameBufferQuad = new CFrameBufferQuad(Texture_Awesome, Texture_RainNoise, Program_LightingPass);

	GeometryBuffer = new CGeometryBuffer();

	ShadowMap = new CShadowMap(iWindowSize, iWindowSize);

	RedFirework = new CParticleSystem(Camera, Program_Particles, 
		Program_ComputeParticles, 
		glm::vec3((int)rand() % 40, (int)rand() % 40, (int)rand() % 40),
		glm::vec3(1.0f, 0.0f, 0.0f));

	CyanFirework = new CParticleSystem(Camera, Program_Particles,
		Program_ComputeParticles,
		glm::vec3((int)rand() % 40, (int)rand() % 40, (int)rand() % 40), 
		glm::vec3(0.0f, 1.0f, 1.0f));

	MagentaFirework = new CParticleSystem(Camera, Program_Particles,
		Program_ComputeParticles,
		glm::vec3((int)rand() % 40, (int)rand() % 40, (int)rand() % 40),
		glm::vec3(1.0f, 0.0f, 1.0f));

	YellowFirework = new CParticleSystem(Camera, Program_Particles,
		Program_ComputeParticles,
		glm::vec3((int)rand() % 40, (int)rand() % 40, (int)rand() % 40),
		glm::vec3(1.0f, 1.0f, 0.0f));
	
	TessQuad = new CTessellationMesh(Texture_Quag);

	// scenes
	Scene3 = new CScene();

	HeightMapInfo infoNoise;
	infoNoise.FilePath = "Resources/Textures/Noise/.raw";
	infoNoise.Width = 512;
	infoNoise.Depth = 512;
	infoNoise.CellSpacing = 1.0f;

	HeightMapInfo info;
	info.FilePath = "Resources/Textures/Heightmap0.raw";
	info.Width = 512;
	info.Depth = 512;
	info.CellSpacing = 1.0f;

	// noise texture
	HeightMapNoise = new CHeightMap(infoNoise, Program_HeightMap, HeightMapTextures);
	HeightMap = new CHeightMap(info, Program_HeightMap, HeightMapTextures);

	// load noise texture after creating heightmap
	Texture_PerlinMap = LoadTexture("Resources/Textures/Noise/COLOURED.jpg");														


	// add objects to scenes															
	//Scene1->AddObject(Skybox);
	//Scene1->AddObject(PointLight1);
	//Scene1->AddObject(PointLight2);													
	Scene3->AddHeightMap(HeightMapNoise);
	//Scene3->AddObject(Soldier);
																						
	//Scene2->AddObject(new CObject(PointLight1, glm::vec3(10.0f, 5.0f, 10.0f), g_physicsWorld, g_physicsCommon));
	//Scene2->AddObject(new CObject(PointLight2, glm::vec3(10.0f, 5.0f, 0.0f), g_physicsWorld, g_physicsCommon));
	//Scene2->AddObject(new CObject(PointLight3, glm::vec3(10.0f, 5.0f, -10.0f), g_physicsWorld, g_physicsCommon));
	//Scene2->AddObject(new CObject(PointLight4, glm::vec3(0.0f, 5.0f, 0.0f), g_physicsWorld, g_physicsCommon));
	//Scene2->AddObject(new CObject(PointLight5, glm::vec3(0.0f, 5.0f, -10.0f), g_physicsWorld, g_physicsCommon));
	//Scene2->AddObject(new CObject(PointLight6, glm::vec3(-10.0f, 5.0f, 10.0f), g_physicsWorld, g_physicsCommon));
	//Scene2->AddObject(new CObject(PointLight7, glm::vec3(-10.0f, 5.0f, 0.0f), g_physicsWorld, g_physicsCommon));
	//Scene2->AddObject(new CObject(PointLight8, glm::vec3(-10.0f, 5.0f, -10.0f), g_physicsWorld, g_physicsCommon));
	//Scene2->AddObject(new CObject(PointLight9, glm::vec3(-20.0f, 5.0f, 0.0f), g_physicsWorld, g_physicsCommon));
	//Scene2->AddObject(new CObject(PointLight10, glm::vec3(0.0f, 5.0f, 10.0f), g_physicsWorld, g_physicsCommon));

	g_CurrentScene = Scene3;

	// physics 
	g_physicsWorld->setGravity(Vector3(0.0f, -5.0f, 0.0f));
	CContactListener listener;
	g_physicsWorld->setEventListener(&listener);

	// set background colour															
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Map the ange of the window for when the buffer clears							
	glViewport(0, 0, iWindowSize, iWindowSize);

	// Height map terrain																
	//Mesh_Terrain();

	// Mouse Callback
	glfwSetCursorPosCallback(Window, CursorPositionInput);

	// for UI clicking
	//glfwSetMouseButtonCallback(Window, CursorPositionInput);

	// Toggle Callback
	glfwSetKeyCallback(Window, KeyInput);

	// anti aliasing 
	glfwWindowHint(GLFW_SAMPLES, 4);
	glEnable(GL_MULTISAMPLE);

	// depth buffer
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// culling
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
}

/// <summary>
/// runs once every tick. Handles anything not to do with rendering to the screen
/// </summary>
void Update()
{
	glfwPollEvents();

	// get current time
	CurrentTime = (float)glfwGetTime();
	deltaTime = CurrentTime - PreviousTime;
	PreviousTime = CurrentTime;

	// update physics
	// only if in play mode?
	CEditorMode& Editor = CEditorMode::GetInstance();

	if (!Editor.GetInEditor())
	{
		g_physicsWorld->update(deltaTime);
	}

	// calculate quad model matrix evert frame
	HeightMapModelMat = MakeModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.15f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	DeferredHeightMapModelMat = MakeModelMatrix(glm::vec3(-50.0f, 0.0f, -30.0f), 0.15f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	SoldierModelMat = MakeModelMatrix(SoldierPosition, 0.15f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// combine for MVP
	//HeightMapModelMat = Camera->GetUIProjMat() * /*Camera->GetUIViewMat() **/ HeightMapModelMat;

	// camera update
	Camera->Update(iWindowSize, Window, g_MousePos, deltaTime);	
	//Camera->PrintCamPos();

	// models update
	g_CurrentScene->Update(Camera, deltaTime);

	// height map
	HeightMap->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition(), DeferredHeightMapModelMat, LightManager->GetVP(), ShadowMap->GetShadowTexture());
	HeightMapNoise->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition(), HeightMapModelMat, LightManager->GetVP(), ShadowMap->GetShadowTexture());

	// tessellation
	TessMVP = Camera->GetVP() * TessMatrix;

	// scenes
	switch (g_iSceneNumber)
	{
	case 1:

		break;
	case 2:
		break;
	case 3:
		// particles
		RedFirework->Update(deltaTime, g_bFKeyPressed);
		CyanFirework->Update(deltaTime, g_bFKeyPressed);
		MagentaFirework->Update(deltaTime, g_bFKeyPressed);
		YellowFirework->Update(deltaTime, g_bFKeyPressed);
		break;
	}
	//NoiseMap->AnimationGrowth(Texture_Quag, Texture_Awesome);

	// UI perlin noise
	//PerlinQuad->Update(Program_Squares, Texture_Awesome, PerlinHeightMapModelMat, Camera->GetUIProjMat(), Camera->GetViewMat());

	// mouse click
	if (glfwGetMouseButton(Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		FindSelectedObject();
	}
}

CUserInterface ui;

// only happens in debug mode
void RenderGUI()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::Begin("My name is window bkvskfbsbf");
	ImGui::Text("poop poop popoppopopoopoooop");
	ImGui::End();

	// custom buttons created inside this function
	// Begin UI rendering
	ImGui::Begin("Buttons");
	
	CEditorMode& Editor = CEditorMode::GetInstance();

	if (Editor.GetInEditor())
	{
		// Play Button
		if (ui.CreateButton("Play", []() {
			std::cout << "Play button clicked" << std::endl;
			}, ImVec2(120, 40),
				ImVec4(0.1f, 0.8f, 0.1f, 1.0f),  // Normal color
				ImVec4(0.3f, 0.6f, 0.9f, 1.0f)))  // Hover color)
		{

			Editor.SetInEditor(false);
		}
	}
	else
	{
		// Play Button
		if (ui.CreateButton("Stop", []() {
			std::cout << "Stop button clicked" << std::endl;
			}, ImVec2(120, 40),
				ImVec4(0.8f, 0.1f, 0.1f, 1.0f),  // Normal color
				ImVec4(0.3f, 0.6f, 0.9f, 1.0f)))  // Hover color)
		{

			Editor.SetInEditor(true);
		}
	}



	// Create a styled button
	if (ui.CreateButton("Print Position", []() {
		std::cout << "Styled button clicked!" << std::endl;
		}, ImVec2(120, 40),
			ImVec4(0.2f, 0.5f, 0.8f, 1.0f),  // Normal color
			ImVec4(0.3f, 0.6f, 0.9f, 1.0f)))  // Hover color)
	{
		std::cout << SelectedObject->GetPosition().x << ", " << SelectedObject->GetPosition().y << ", "  << SelectedObject->GetPosition().z << ", " << std::endl;
	}

	ImGui::End();

	ImGui::SetNextWindowSize(ImVec2(500, 500));
	if (ImGui::Begin("Selected Object", NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse))
	{
		if (SelectedObject != nullptr)
		{
			// get values to later set
			float xPos = SelectedObject->GetPosition().x;
			float yPos = SelectedObject->GetPosition().y;
			float zPos = SelectedObject->GetPosition().z;

			bool bGravity = SelectedObject->IsGravityEnabled();

			ImGui::Checkbox("HasGravity", &bGravity);
			ImGui::InputFloat("X: ", &xPos);
			ImGui::InputFloat("Y: ", &yPos);
			ImGui::InputFloat("Z: ", &zPos);

			SelectedObject->SetGravityEnabled(bGravity);
			SelectedObject->SetPosition(glm::vec3(xPos, yPos, zPos));

			if (ui.CreateButton("Destroy", []() {
				std::cout << "Destroy button clicked" << std::endl;
				}, ImVec2(120, 40),
					ImVec4(0.2f, 0.5f, 0.8f, 1.0f),  // color
					ImVec4(0.3f, 0.6f, 0.9f, 1.0f)))  // hover color)
			{
				// remove the object from the scene
				g_CurrentScene->RemoveObject(SelectedObject);

				// delete the object
				delete SelectedObject;
				// set the current object to null
				SelectedObject = nullptr;
			}
		}
	}	
	
	ImGui::End();


	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

/// <summary>
/// Runs once every tick. handles anything visual such as lighting and rendering
/// </summary>
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// lighting
	LightManager->UpdateShader(Program_Lighting, g_bPointLightActive);
	LightManager->UpdateShader(Program_InstancedLighting, g_bPointLightActive);
	LightManager->UpdateShader(Program_HeightMap, g_bPointLightActive);
	LightManager->UpdateShader(Program_LightingPass, g_bPointLightActive);

	g_CurrentScene->Render();

	// scenes
	//switch (g_iSceneNumber)
	//{
	//case 1:
	//	ShadowMap->Bind();

	//	//Scene3->RenderShadow(Program_ShadowMap, LightManager->GetVP());

	//	ShadowMap->Unbind();
	//	Scene3->Render();

	//	break;
	//case 2:
	//	FrameBufferQuad->SetProgram(Program_LightingPass);
	//	GeometryBuffer->Bind();
	//	Tree->RenderGeometryInstanced(Program_GeometryPass, Texture_Quag, RandomLocations, TreeModelMat, Camera->GetPosition(), Camera->GetVP());
	//	HeightMap->RenderGeometry(Program_GeometryPassHeightMap);
	//	GeometryBuffer->PopulateProgram(Program_LightingPass, Camera->GetPosition());
	//	GeometryBuffer->Unbind();

	//	FrameBufferQuad->RenderLightingPass();

	//	GeometryBuffer->WriteDepth();

	//	// Enable blending for point lights
	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	Scene2->Render();

	//	glDisable(GL_BLEND);

	//	break;
	//case 3:

	//	glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//	RedFirework->Render();
	//	CyanFirework->Render();
	//	MagentaFirework->Render();
	//	YellowFirework->Render();

	//	glDisable(GL_BLEND);

	//	break;
	//case 4:
	//	TessQuad->Render(Program_Tessellation, TessMVP);
	//	break;
	//default:
	//	break;
	//}

	// unbind
	glBindVertexArray(0);
	
	glUseProgram(0);

	RenderGUI();

	glfwSwapBuffers(Window);
}

/// <summary>
/// initialises openGL and runs update and render every frame
/// </summary>
/// <returns></returns>
int main()
{
	// initialise GLFW and set version to 4.6
	glfwInit();
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);


	// Make a GLFW window
	Window = glfwCreateWindow(iWindowSize, iWindowSize, "First OpenGL Window", NULL, NULL);
	if (Window == NULL)
	{
		std::cout << "GLFW failed to initialise properly. Terminating Program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(Window);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(Window, true);
	ImGui_ImplOpenGL3_Init("#version 460");


	// initialize GLEW for function pointers
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Glew failed to initialize properly. Terminating program." << std::endl;
		system("pause");

		glfwTerminate();
		return -1;
	}

	// initial program elements setup
	InitialSetup();

	//Main Loop
	while (glfwWindowShouldClose(Window) == false)
	{

		// update all objects
		Update();


		// render all objects
		Render();

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	// shut down correctly
	glfwTerminate();

	g_physicsCommon.destroyPhysicsWorld(g_physicsWorld);

	// delete dynamically allocated memory
	delete Camera;

	delete Tree;

	delete LightManager;

	delete PointLight1;
	delete PointLight2;

	delete HeightMap;
	delete HeightMapNoise;

	delete TessQuad;

	return 0;
}

