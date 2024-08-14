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
#include "CCamera.h"
#include "CModel.h"
#include "CSkyBox.h"
#include "CLightManager.h"
#include "CHeightMap.h"
#include "CPerlinNoise.h"
#include "CScene.h"
#include "CQuad.h"
#include "CFramebuffer.h"

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

// height map
CHeightMap* HeightMap;

// perlin noise
CPerlinNoise* NoiseMap;

// ui quad for perlin noise
CQuad* PerlinQuad;

// framebuffer
CFramebuffer* FrameBuffer;

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

// texture
GLuint Texture_Awesome;
GLuint Texture_Quag;
GLuint Texture_3;
GLuint Texture_4;
// textures for height map
GLint HeightMapTextures[4];

// Object Matrices and Components
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

// mouse position
glm::vec2 g_MousePos;

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

	//// toggle lighting
	//if (_Key == GLFW_KEY_1 && _Action == GLFW_PRESS)
	//{
	//	g_bPointLightActive = !g_bPointLightActive;
	//}

}

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

// custom functions for tidy code
void InitialSetup()
{
	// program for 3d model
	Program_3DModel = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert",
		"Resources/Shaders/3DModel.frag");

	// program for lighting
	Program_Lighting = ShaderLoader::CreateProgram("Resources/Shaders/InstancedArray_Standard.vert",
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

	// flip image
	stbi_set_flip_vertically_on_load(true);

	Texture_Quag = LoadTexture("Resources/Textures/PolygonAncientWorlds_Texture_01_A.png");
	Texture_Awesome = LoadTexture("Resources/Textures/360_F_107140090_3eRlMItNMxEcw67BDq0lPAppu5q62QUw.jpg");
	Texture_3 = LoadTexture("Resources/Textures/PolygonAncientWorlds_Statue_01.png");
	Texture_4 = LoadTexture("Resources/Textures/snow.jpg");
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

	Skybox = new CSkyBox(sFaces, "Resources/Models/cube.obj", Program_Skybox);

	PointLight1 = new CModel("Resources/Models/SM_Prop_Statue_02.obj", Program_PointLight1, Texture_Quag, PLModelMat1);

	PointLight2 = new CModel("Resources/Models/SM_Prop_Statue_02.obj", Program_PointLight2, Texture_Quag, PLModelMat2);

	LightManager = new CLightManager();

	LightManager->AddPointLight(glm::vec3(20.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f, 1.0f, 0.045f, 0.0075f);
	LightManager->AddPointLight(glm::vec3(-20.0f, 0.0f, -20.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 1.0f, 0.045f, 0.0075f);

	NoiseMap = new CPerlinNoise(512, 512);

	PerlinQuad = new CQuad();

	//FrameBuffer = new CFramebuffer(iWindowSize, iWindowSize);

	// scenes
	Scene1 = new CScene();
	Scene2 = new CScene();
	Scene3 = new CScene();
	Scene4 = new CScene();

	HeightMapInfo info;
	info.FilePath = "Resources/Textures/Noise/.raw";
	info.Width = 512;
	info.Depth = 512;
	info.CellSpacing = 1.0f;

	HeightMap = new CHeightMap(info, Program_HeightMap, HeightMapTextures);

	// add objects to scenes
	Scene1->AddObject(Skybox);
	Scene1->AddHeightMap(HeightMap);

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
	glDepthFunc(GL_CCW);
}

void Update()
{
	glfwPollEvents();

	// get current time
	CurrentTime = (float)glfwGetTime();
	deltaTime = CurrentTime - PreviousTime;
	PreviousTime = CurrentTime;

	// calculate quad model matrix once
	QuadModelMat = MakeModelMatrix(glm::vec3(1000.0f, 1000.0f, 1000.0f), 10.0f, 180.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	// combine for MVP
	QuadModelMat = Camera->GetUIProjMat() * /*Camera->GetUIViewMat() **/ QuadModelMat;

	// UI model matrix
	PerlinQuadModelMat = Camera->GetUIProjMat() * /*Camera->GetUIViewMat() **/ QuadModelMat;

	// camera update
	Camera->Update(iWindowSize, Window, g_MousePos, deltaTime);	
	//Camera->PrintCamPos();

	// models update
	PointLight1->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition());
	PointLight2->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition());

	// height map
	HeightMap->Update(Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition(), QuadModelMat);

	// UI perlin noise
	PerlinQuad->Update(Program_Squares, Texture_Awesome, PerlinQuadModelMat, Camera->GetUIProjMat(), Camera->GetViewMat());
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//FrameBuffer->Bind();

	// many trees
	// point lights
	LightManager->UpdateShader(Program_Lighting, g_bPointLightActive);
	LightManager->UpdateShader(Program_HeightMap, g_bPointLightActive);

	// scenes
	switch (g_iSceneNumber)
	{
	case 1:
		Scene1->Render();
		break;
	case 2:
		Scene2->Render();
		break;
	case 3:
		PerlinQuad->Render();
		Scene3->Render();
		break;
	case 4:
		Scene4->Render();
		break;
	}

	//FrameBuffer->Unbind();
	//PerlinQuad->Render();

	// unbind
	glBindVertexArray(0);
	
	glUseProgram(0);

	glfwSwapBuffers(Window);
}

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

	delete HeightMap;

	delete NoiseMap;

	delete PerlinQuad;

	return 0;
}