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

// library define 
#define TINYOBJLOADER_IMPLEMENTATION

#include "ShaderLoader.h"
#include "CModel.h"
#include "CSkyBox.h"
#include "CLightManager.h"
#include "CHeightMap.h"
#include "CPerlinNoise.h"
#include "CScene.h"
#include "CFrameBufferQuad.h"
#include "CQuad.h"
#include "CFramebuffer.h"
#include "CShadowMap.h"
#include "CParticleSystem.h"

// global variables
GLFWwindow* Window = nullptr;
GLuint Program_FixedTri;
int iWindowSize = 800;

// pointer to shape objects
CCamera* Camera;
// tree
//CModel* Tree;
// skybox
CSkyBox* Skybox;
// light manager
CLightManager* LightManager;
// point lights
CModel* PointLight1;
CModel* PointLight2;

// shadow object
CModel* Model;

// height map
CHeightMap* HeightMapNoise;

// perlin noise
CPerlinNoise* NoiseMap;

// ui quad for perlin noise
CQuad* PerlinQuad;
CFrameBufferQuad* FrameBufferQuad;

// framebuffer
CFramebuffer* FrameBuffer;

// Shadows
CShadowMap* ShadowMap;

// particle effects
CParticleSystem* Particles;

// scenes
CScene* Scene1;
CScene* Scene2;
CScene* Scene3;
CScene* Scene4;

// programs
GLuint Program_3DModel;
GLuint Program_Lighting;
GLuint Program_Skybox;
GLuint Program_PointLight1;
GLuint Program_PointLight2;
GLuint Program_HeightMap;
GLuint Program_Squares;
GLuint Program_RenderBuffer;
GLuint Program_InverseColour;
GLuint Program_GreyScale;
GLuint Program_RenderBufferNone;
GLuint Program_Effect;
GLuint Program_Cartoon;
GLuint Program_Rain;
GLuint Program_ShadowMap;
// compute program for particles
GLuint Program_ComputeParticles;
GLuint Program_Particles;

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
// translation
glm::vec3 SoldierPosition = glm::vec3(1000.0f, 1000.0f, 0.0f);
glm::mat4 TranslationMat;

// rotation
float SoldierRotationAngle = 0.0f;
glm::mat4 RotationMat;

// scale
glm::vec3 SoldierScale = glm::vec3(0.0015f, 0.0015f, 0.0015f);
glm::mat4 ScaleMat;

// model to be combined with view and projection
glm::mat4 SoldierModelMat;

glm::mat4 PLScaleMat;

// model to be combined with view and projection
glm::mat4 PLModelMat1;

// model to be combined with view and projection
glm::mat4 PLModelMat2;

glm::mat4 TreeModelMat;

glm::mat4 QuadModelMat;

glm::mat4 PerlinQuadModelMat;

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


// Define the six faces of the cube map in a vector
std::vector<std::string> sFaces = {
	"Resources/Textures/Right.png",
	"Resources/Textures/Left.png",
	"Resources/Textures/Top.png",
	"Resources/Textures/Bottom.png",
	"Resources/Textures/Back.png",
	"Resources/Textures/Front.png"
};


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
}

// for single key press
void KeyInput(GLFWwindow* _Window, int _Key, int _ScanCode, int _Action, int _Mods)
{
	// for sxene rendering
	if (_Key == GLFW_KEY_1 && _Action == GLFW_PRESS)
	{
		g_iSceneNumber = 1;
	}

	if (_Key == GLFW_KEY_2 && _Action == GLFW_PRESS)
	{
		g_iSceneNumber = 2;
	}

	if (_Key == GLFW_KEY_3 && _Action == GLFW_PRESS)
	{
		g_iSceneNumber = 3;
	}

	if (_Key == GLFW_KEY_4 && _Action == GLFW_PRESS)
	{
		g_iSceneNumber = 4;
	}


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

	if (_Key == GLFW_KEY_TAB && _Action == GLFW_PRESS)
	{
		if (g_iProgram <= 4)
		{
			g_iProgram++;
		}
		else
		{
			g_iProgram = 0;
		}

		switch (g_iProgram)
		{
		case 0:
			Program_RenderBuffer = Program_RenderBufferNone;
			break;

		case 1:
			Program_RenderBuffer = Program_GreyScale;
			break;

		case 2:
			Program_RenderBuffer = Program_InverseColour;
			break;

		case 3:
			Program_RenderBuffer = Program_Rain;
			break;

		case 4:
			Program_RenderBuffer = Program_Effect;
			break;

		case 5:
			Program_RenderBuffer = Program_Cartoon;
			break;

		default:
			break;
		}
	}

		// for object
	// move forward
	if (glfwGetKey(_Window, GLFW_KEY_UP))
	{
		// use camera forward
		SoldierPosition -= Camera->GetForward() * deltaTime * MoveSpeed;
	}
	
	// move back
	if (glfwGetKey(_Window, GLFW_KEY_DOWN))
	{
		// use camera forward but reverse
		SoldierPosition += Camera->GetForward() * deltaTime * MoveSpeed;
	}
	
	// move left
	if (glfwGetKey(_Window, GLFW_KEY_LEFT))
	{
		// use camera right but reverse
		SoldierPosition += Camera->GetRight() * deltaTime * MoveSpeed;
	}
	
	// move right
	if (glfwGetKey(_Window, GLFW_KEY_RIGHT))
	{
		// use camera forward
		SoldierPosition -= Camera->GetRight() * deltaTime * MoveSpeed;
	}
	//// toggle lighting
	//if (_Key == GLFW_KEY_1 && _Action == GLFW_PRESS)
	//{
	//	g_bPointLightActive = !g_bPointLightActive;
	//}

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

/// <summary>
/// runs once at the beginning of the project
/// </summary>
void InitialSetup()
{
	// program for 3d model
	//Program_3DModel = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
	//	"Resources/Shaders/3DModel.frag");

	// program for lighting
	Program_Lighting = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/Lighting_BlinnPhong.frag");

	// program for skybox
	Program_Skybox = ShaderLoader::CreateProgram("Resources/Shaders/Skybox.vert",
		"Resources/Shaders/Skybox.frag");

	// program for point light 1
	Program_PointLight1 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLight1.frag");

	// program for point light 2
	Program_PointLight2 = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/PointLight2.frag");

	Program_HeightMap = ShaderLoader::CreateProgram("Resources/Shaders/HeightMap.vert",
		"Resources/Shaders/Lighting_HeightMap.frag");

	// for orthographic squares
	Program_Squares = ShaderLoader::CreateProgram("Resources/Shaders/Squares.vert",
		"Resources/Shaders/Squares.frag");

	// base renderbuffer
	Program_RenderBuffer = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/RenderBuffer.vert",
		"Resources/Shaders/FrameBuffer/RenderBuffer.frag");

	// renderbuffer with no effects
	Program_RenderBufferNone = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/RenderBuffer.vert",
		"Resources/Shaders/FrameBuffer/RenderBuffer.frag");

	// renderbuffer with inverse colour
	Program_InverseColour = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/RenderBuffer.vert",
		"Resources/Shaders/FrameBuffer/InverseColour.frag");

	// renderbuffer with luminosity method greyscale
	Program_GreyScale = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/RenderBuffer.vert",
		"Resources/Shaders/FrameBuffer/Greyscale.frag");

	// renderbuffer with something idk
	Program_Effect = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/RenderBuffer.vert",
		"Resources/Shaders/FrameBuffer/Effect.frag");

	// renderbuffer with rain
	Program_Rain = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/RenderBuffer.vert",
		"Resources/Shaders/FrameBuffer/Rain.frag");

	// renderbuffer with cartoon effect
	Program_Cartoon = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/RenderBuffer.vert",
		"Resources/Shaders/FrameBuffer/Cartoon.frag");

	// renderbuffer with cartoon effect
	Program_ShadowMap = ShaderLoader::CreateProgram("Resources/Shaders/FrameBuffer/ShadowPass.vert",
		"Resources/Shaders/FrameBuffer/ShadowPass.frag");

	// particles
	Program_Particles = ShaderLoader::CreateProgram("Resources/Shaders/Particles.vert",
		"Resources/Shaders/Particles.frag");

	// particle compute shader
	Program_ComputeParticles = ShaderLoader::CreateProgram_C("ComputeParticles.comp");

	// flip image
	stbi_set_flip_vertically_on_load(true);

	Texture_Quag = LoadTexture("Resources/Textures/SkyboxBack.jpg");
	Texture_Awesome = LoadTexture("Resources/Textures/360_F_107140090_3eRlMItNMxEcw67BDq0lPAppu5q62QUw.jpg");
	Texture_3 = LoadTexture("Resources/Textures/pexels-pixmike-413195.jpg");
	Texture_4 = LoadTexture("Resources/Textures/PolygonAncientWorlds_Statue_01.png");
	Texture_RainNoise = LoadTexture("Resources/Textures/ad56fba948dfba9ae698198c109e71f118a54d209c0ea50d77ea546abad89c57.png");
	HeightMapTextures[0] = Texture_Quag;
	HeightMapTextures[1] = Texture_Awesome;
	HeightMapTextures[2] = Texture_3;
	HeightMapTextures[3] = Texture_4;

	// calculate model matrix
	SoldierModelMat = MakeModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.15f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// for point lights
	PLModelMat1 = MakeModelMatrix(glm::vec3(20.0f, 0.0f, 20.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// point light 2
	PLModelMat2 = MakeModelMatrix(glm::vec3(-20.0f, 0.0f, -20.0f), 0.008f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// for instanced matrices
	TreeModelMat = MakeModelMatrix(glm::vec3(0.0f, 0.0f, 0.0f), 0.005f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// for Perlin noise quad
	PerlinQuadModelMat = MakeModelMatrix(glm::vec3(1.0f, 1.0f, 1.0f), 1.0f, 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

	// initialise objects
	Camera = new CCamera();

	Model = new CModel("Resources/Models/SM_Prop_Statue_02.obj", Program_Lighting, Texture_4, SoldierModelMat);

	Skybox = new CSkyBox(sFaces, "Resources/Models/cube.obj", Program_Skybox);

	PointLight1 = new CModel("Resources/Models/SM_Prop_Statue_02.obj", Program_PointLight1, Texture_Quag, PLModelMat1);

	PointLight2 = new CModel("Resources/Models/SM_Prop_Statue_02.obj", Program_PointLight2, Texture_Quag, PLModelMat2);

	LightManager = new CLightManager();

	LightManager->AddPointLight(glm::vec3(20.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f, 0.045f, 0.0075f);
	LightManager->AddPointLight(glm::vec3(-20.0f, 0.0f, -20.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f, 0.045f, 0.0075f);

	NoiseMap = new CPerlinNoise(512, 512);

	PerlinQuad = new CQuad(10, 10, 100, 100, Texture_3, Program_Squares);

	FrameBufferQuad = new CFrameBufferQuad(Texture_Awesome, Texture_RainNoise, Program_RenderBuffer);

	FrameBuffer = new CFramebuffer(iWindowSize, iWindowSize);

	ShadowMap = new CShadowMap(iWindowSize, iWindowSize);

	Particles = new CParticleSystem(Camera, Program_Particles, Program_ComputeParticles, glm::vec3(0.0f, 0.0f, 0.0f));

	// scenes
	Scene1 = new CScene();
	Scene2 = new CScene();
	Scene3 = new CScene();
	Scene4 = new CScene();

	HeightMapInfo infoNoise;
	infoNoise.FilePath = "Resources/Textures/Heightmap0.raw";
	infoNoise.Width = 512;
	infoNoise.Depth = 512;
	infoNoise.CellSpacing = 1.0f;

	// noise texture

	Texture_Noise = LoadTexture("Resources/Textures/Noise/" + std::to_string(NoiseMap->GetSeed()) + ".jpg");

	HeightMapNoise = new CHeightMap(infoNoise, Program_HeightMap, HeightMapTextures);
	// load noise texture after creating heightmap
	Texture_PerlinMap = LoadTexture("Resources/Textures/Noise/COLOURED.jpg");

	// add objects to scenes
	//Scene1->AddObject(Skybox);
	Scene1->AddObject(PointLight1);
	//Scene1->AddObject(PointLight2);
	Scene3->AddHeightMap(HeightMapNoise);
	Scene3->AddObject(Model);

	// set background colour
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Map the ange of the window for when the buffer clears
	glViewport(0, 0, iWindowSize, iWindowSize);

	// Height map terrain
	//Mesh_Terrain();

	// Mouse Callback
	glfwSetCursorPosCallback(Window, CursorPositionInput);
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

	// calculate quad model matrix once
	QuadModelMat = MakeModelMatrix(glm::vec3(1000.0f, 0.0f, 1000.0f), 10.0f, 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	// combine for MVP
	QuadModelMat = Camera->GetUIProjMat() * /*Camera->GetUIViewMat() **/ QuadModelMat;

	// camera update
	Camera->Update(iWindowSize, Window, g_MousePos, deltaTime);	
	//Camera->PrintCamPos();

	switch (g_iSceneNumber)
	{
	case 1:
		FrameBuffer->Bind();
		Scene1->RenderShadow(Program_ShadowMap, LightManager->GetVP());
		FrameBuffer->Unbind();
		break;
	case 2:
		// particles
		Particles->Update(deltaTime);
		break;
	case 3:
		// models update
		// calculate model matrix
		TranslationMat = glm::translate(glm::mat4(1.0f), SoldierPosition);
		RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((SoldierRotationAngle) * 10), glm::vec3(1.0f, 1.0f, 1.0f));
		ScaleMat = glm::scale(glm::mat4(1.0f), SoldierScale);
		SoldierModelMat = TranslationMat * RotationMat * ScaleMat;

		SoldierModelMat = Camera->GetProjMat() * Camera->GetViewMat() * SoldierModelMat;

		Model->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition(), SoldierModelMat);

		// point light update
		PointLight1->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition(), PLModelMat1);
		PointLight2->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition(), PLModelMat2);

		HeightMapNoise->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition(), QuadModelMat, LightManager->GetVP(), ShadowMap->GetShadowTexture());

		break;
	}

	//NoiseMap->AnimationGrowth(Texture_Quag, Texture_Awesome);

	// UI perlin noise
	//PerlinQuad->Update(Program_Squares, Texture_Awesome, PerlinQuadModelMat, Camera->GetUIProjMat(), Camera->GetViewMat());
}

/// <summary>
/// Runs once every tick. handles anything visual such as lighting and rendering
/// </summary>
void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// lighting
	LightManager->UpdateShader(Program_Lighting, g_bPointLightActive);
	LightManager->UpdateShader(Program_HeightMap, g_bPointLightActive);

	// scenes
	switch (g_iSceneNumber)
	{
	case 1:
		FrameBuffer->Bind();
		Scene1->RenderShadow(Program_ShadowMap, LightManager->GetVP());
		FrameBuffer->Unbind();
		break;
	case 2:
		Scene2->RenderShadow(Program_ShadowMap, LightManager->GetVP());
		Particles->Render();
		break;
	case 3:
		ShadowMap->Bind();
		//FrameBuffer->Bind();

		Scene3->RenderShadow(Program_ShadowMap, LightManager->GetVP());

		ShadowMap->Unbind();
		//FrameBuffer->Unbind();
		PerlinQuad->UpdateTexture(Texture_PerlinMap);
		PerlinQuad->Render(*Camera);
		Scene3->Render();
		//Model->Render();
		//FrameBufferQuad->SetProgram(Program_RenderBuffer);
		//FrameBufferQuad->UpdateTexture(FrameBuffer->GetRenderTexture());
		//FrameBufferQuad->Render();
		break;
	}

	// unbind
	glBindVertexArray(0);
	
	glUseProgram(0);

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

	// shut down correctly
	glfwTerminate();

	// delete dynamically allocated memory
	delete Camera;

	//delete Tree;

	delete Skybox;

	delete LightManager;

	delete PointLight1;
	delete PointLight2;

	delete HeightMapNoise;

	delete NoiseMap;

	delete PerlinQuad;

	return 0;
}