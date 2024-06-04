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
#include "CButton.h"
#include "CSkyBox.h"
#include "CLightManager.h"

// global variables
GLFWwindow* Window = nullptr;
GLuint Program_FixedTri;
int iWindowSize = 800;

// pointer to shape objects
CCamera* Camera;
// statue
CModel* Model;
// tree
CModel* Tree;
// UI input button
CButton* Button;
// skybox
CSkyBox* Skybox;
// light manager
CLightManager* LightManager;

// camera vars
glm::mat4 m_projMat;

glm::vec3 m_lookDir;
glm::vec3 m_upDir;
glm::vec3 m_position;

glm::mat4 m_viewMat;

// programs
GLuint Program_Quads;
GLuint Program_3DModel;
GLuint Program_Lighting;
GLuint Program_Skybox;

// texture
GLuint Texture_Awesome;
GLuint Texture_Quag;

// Object Matrices and Components
// translation
glm::vec3 SoldierPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 TranslationMat;

// rotation
float SoldierRotationAngle = 0.0f;
glm::mat4 RotationMat;

// scale
glm::vec3 SoldierScale = glm::vec3(0.15f, 0.15f, 0.15f);
glm::mat4 ScaleMat;

// model to be combined with view and projection
glm::mat4 SoldierModelMat;


// Tree Model mat
// translation
glm::vec3 TreePosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 TreeTranslationMat;
//rotation
float TreeRotationAngle = 0.0f;
glm::mat4 TreeRotationMat;
// scale
glm::vec3 TreeScale = glm::vec3(0.005f, 0.005f, 0.005f);
glm::mat4 TreeScaleMat;
// model matrix
glm::mat4 TreeModelMat;

// Quad Model Mat values
glm::vec3 QuadPosition = glm::vec3(100.0f, 100.0f, 0.0f);
glm::mat4 QuadTranslationMat;

float QuadRotationAngle = 0.0f;
glm::mat4 QuadRotationMat;

glm::vec3 QuadScale = glm::vec3(100.0f, 100.0f, 1.0f);
glm::mat4 QuadScaleMat;

glm::mat4 QuadModelMat;

// Vector for instanced matrices
std::vector<glm::mat4> MVPVec;
// for random tree positions
std::vector<glm::vec3> RandomLocations;

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
bool g_bWireFrame = false;
bool g_bShowMouse = true;
bool g_UIChange = false;

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
	// toggle wireframe
	if (_Key == GLFW_KEY_1 && _Action == GLFW_PRESS)
	{
		g_bWireFrame = !g_bWireFrame;
	}

	//// for object
	//// move forward
	//if (glfwGetKey(_Window, GLFW_KEY_W))
	//{
	//	// use camera forward
	//	SoldierPosition -= Camera->GetForward() * deltaTime * MoveSpeed;
	//}

	//// move back
	//if (glfwGetKey(_Window, GLFW_KEY_S))
	//{
	//	// use camera forward but reverse
	//	SoldierPosition += Camera->GetForward() * deltaTime * MoveSpeed;
	//}

	//// move left
	//if (glfwGetKey(_Window, GLFW_KEY_A))
	//{
	//	// use camera right but reverse
	//	SoldierPosition += Camera->GetRight() * deltaTime * MoveSpeed;
	//}

	//// move right
	//if (glfwGetKey(_Window, GLFW_KEY_D))
	//{
	//	// use camera forward
	//	SoldierPosition -= Camera->GetRight() * deltaTime * MoveSpeed;
	//}

	//// move up
	//if (glfwGetKey(_Window, GLFW_KEY_Q))
	//{
	//	// use camera up but reverse
	//	SoldierPosition -= Camera->GetUp() * deltaTime * MoveSpeed;
	//}

	//// move down
	//if (glfwGetKey(_Window, GLFW_KEY_E))
	//{
	//	// use camera up 
	//	SoldierPosition += Camera->GetUp() * deltaTime * MoveSpeed;
	//}
}

// custom functions for tidy code
void InitialSetup()
{
	// load program for single model / UI
	Program_Quads = ShaderLoader::CreateProgram("Resources/Shaders/Squares.vert.txt",
		"Resources/Shaders/Texture.frag.txt");

	// program for 3d model
	Program_3DModel = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert.txt",
		"Resources/Shaders/3DModel.frag.txt");

	// program for lighting
	Program_Lighting = ShaderLoader::CreateProgram("Resources/Shaders/InstancedArray_Standard.vert.txt",
		"Resources/Shaders/Lighting_BlinnPhong.frag.txt");

	// program for skybox
	Program_Skybox = ShaderLoader::CreateProgram("Resources/Shaders/Skybox.vert.txt",
		"Resources/Shaders/Skybox.frag.txt");

	// flip image
	stbi_set_flip_vertically_on_load(true);

	// Load Image data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	// create and bind new texture
	glGenTextures(1, &Texture_Quag);
	glBindTexture(GL_TEXTURE_2D, Texture_Quag);

	unsigned char* ImageData = stbi_load("Resources/Textures/PolygonAncientWorlds_Texture_01_A.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);


	// Check if image is RGBA or RGB
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);
	
	// generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// second image 
	// Load Image data
	unsigned char* ImageData1 = stbi_load("Resources/Textures/PolygonAncientWorlds_Statue_01.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);

	// create and bind new texture
	glGenTextures(1, &Texture_Awesome);
	glBindTexture(GL_TEXTURE_2D, Texture_Awesome);

	// Check if image is RGBA or RGB
	LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData1);

	// generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	// free memory
	stbi_image_free(ImageData1);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	Camera = new CCamera();

	Model = new CModel("Resources/Models/SM_Prop_Statue_02.obj");

	Tree = new CModel("Resources/Models/SM_Env_Tree_Palm_01.obj");

	Button = new CButton();

	Skybox = new CSkyBox(sFaces, "Resources/Models/cube.obj");

	LightManager = new CLightManager();

	LightManager->AddPointLight(glm::vec3(25.0f, 15.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), 1.0f);
	LightManager->AddPointLight(glm::vec3(-25.0f, 15.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f);

	// set background colour
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Map the ange of the window for when the buffer clears
	glViewport(0, 0, iWindowSize, iWindowSize);

	// clear vector just in case
	MVPVec.clear();
	// add matrices to matrix vec for each tree
	for (unsigned int i = 0; i < g_objCount; i++)
	{
		// randomize x and z positions to disperse trees
		RandomLocations.push_back(glm::vec3((rand() % 8000) - 4000, 0, (rand() % 8000) - 4000)); // random square around 0, 0, 0
		// add random matrix to MVP so that the size is correct
		MVPVec.push_back(SoldierModelMat);
	}

	// set matrices as instanced vertex attribute
	GLuint VBO_Instanced;
	glGenBuffers(1, &VBO_Instanced);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
	glBufferData(GL_ARRAY_BUFFER, 1000 * sizeof(glm::mat4), &RandomLocations[0], GL_STATIC_DRAW);

	// bind instance buffer to attribue location
	glBindVertexArray(Tree->GetVAO());
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
	glVertexAttribDivisor(2, 1);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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


	// calculate model matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), SoldierPosition);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((SoldierRotationAngle) * 10), glm::vec3(1.0f, 1.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), SoldierScale);
	SoldierModelMat = TranslationMat * RotationMat * ScaleMat;

	SoldierModelMat = Camera->GetProjMat() * Camera->GetViewMat() * SoldierModelMat;

	// for instanced matrices
	TreeTranslationMat = glm::translate(glm::mat4(1.0f), TreePosition);
	TreeRotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((TreeRotationAngle) * 10), glm::vec3(1.0f, 1.0f, 1.0f));
	TreeScaleMat = glm::scale(glm::mat4(1.0f), TreeScale);
	// multiply translation rotation scale for model matrix
	TreeModelMat = TreeTranslationMat * TreeRotationMat * TreeScaleMat;
	// get MVP by multiplying by camera projection and view matrices
	TreeModelMat = Camera->GetProjMat() * Camera->GetViewMat() * TreeModelMat;

	// calculate quad model matrix once
	QuadTranslationMat = glm::translate(glm::mat4(1.0f), QuadPosition);
	QuadRotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((QuadRotationAngle)), glm::vec3(0.0f, 0.0f, 1.0f));
	QuadScaleMat = glm::scale(glm::mat4(1.0f), QuadScale);
	QuadModelMat = QuadTranslationMat * QuadRotationMat * QuadScaleMat;

	// combine for MVP
	QuadModelMat = Camera->GetUIProjMat() * /*Camera->GetUIViewMat() **/ QuadModelMat;

	// apply random position to every matrix in the vector
	for (unsigned int i = 0; i < g_objCount; i++)
	{
		glm::mat4 newModelMat;
		glm::mat4 randPosMatrix = glm::translate(glm::mat4(1.0f), RandomLocations[i]);
		newModelMat = randPosMatrix * TreeRotationMat * TreeScaleMat;
		MVPVec[i] = newModelMat;
	}

	for (auto& matrix : MVPVec)
	{
		matrix = Camera->GetProjMat() * Camera->GetViewMat() * matrix;
	}

	// camera update
	Camera->Update(CurrentTime, iWindowSize, Window, g_MousePos, deltaTime);	
	Camera->PrintCamPos();
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// UI button 
	// change texture if mouse overlapping
	if (g_UIChange)
	{
		// single soldier model
		Model->Render(Program_3DModel, Texture_Awesome, SoldierModelMat, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition());

		// many trees
		Tree->RenderInstanced(Program_Lighting, Texture_Awesome, RandomLocations, TreeModelMat, Camera->GetPosition());

		// Button
		Button->Render(Program_Quads, Texture_Awesome, QuadModelMat, CurrentTime, Camera->GetUIProjMat(), Camera->GetViewMat());
	}
	else
	{
		// single soldier model
		Model->Render(Program_3DModel, Texture_Quag, SoldierModelMat, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat(), Camera->GetPosition());

		// many trees
		Tree->RenderInstanced(Program_Lighting, Texture_Quag, RandomLocations, TreeModelMat, Camera->GetPosition());
		// Button
		Button->Render(Program_Quads, Texture_Quag, QuadModelMat, CurrentTime, Camera->GetUIProjMat(), Camera->GetViewMat());
	}

	// skybox
	glm::mat4 view = Camera->GetViewMat();
	glm::mat4 projection = Camera->GetProjMat();
	Skybox->Render(Program_Skybox, view, projection);

	// point lights
	LightManager->UpdateShader(Program_Lighting);

	// unbind
	glBindVertexArray(0);

	// show wireframe
	if (g_bWireFrame)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
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

	return 0;
}