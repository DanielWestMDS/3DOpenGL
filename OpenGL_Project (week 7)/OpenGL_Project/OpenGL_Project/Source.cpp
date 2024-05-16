#define STB_IMAGE_IMPLEMENTATION

// library define 
#define TINYOBJLOADER_IMPLEMENTATION

#include "ShaderLoader.h"
//#include "CShape.h"
#include "CCamera.h"
#include "CCube.h"
#include "CModel.h"

// global variables
GLFWwindow* Window = nullptr;
GLuint Program_FixedTri;
int iWindowSize = 800;

// pointer to shape objects
CCamera* Camera;
CCube* Cube;
// statue
CModel* Model;



// camera vars
glm::mat4 m_projMat;

glm::vec3 m_lookDir;
glm::vec3 m_upDir;
glm::vec3 m_position;

glm::mat4 m_viewMat;

// programs
GLuint Program_Color;
GLuint Program_ColorFade;
GLuint Program_PositionOnly;
GLuint Program_WorldSpace;
GLuint Program_Texture;
GLuint Program_Animation;
GLuint Program_ClipSpace;
GLuint Program_Quads;
GLuint Program_3DModel;

// texture
GLuint Texture_Awesome;
GLuint Texture_Quag;

// Object Matrices and Components
glm::vec3 QuadPosition = glm::vec3(0.0f, 0.0f, 0.0f);
glm::mat4 TranslationMat;

float QuadRotationAngle = 0.0f;
glm::mat4 RotationMat;

glm::vec3 QuadScale = glm::vec3(0.2f, 0.2f, 0.2f);
glm::mat4 ScaleMat;

glm::mat4 QuadModelMat;

// camera matrices
glm::mat4 ProjectionMat;
glm::mat4 ViewMat;
glm::mat4 ObjModelMat;

glm::vec3 CameraPos;
glm::vec3 CameraLookDir;
glm::vec3 CameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);

int x = 0;
int y = 0;

// time
float CurrentTime;
float PreviousTime;
float deltaTime;

// toggle bools
bool g_bShowMousePosition = false;
bool g_bWireFrame = false;
bool g_bShowMouse = true;

// for position callback
void CursorPositionInput(GLFWwindow* _Window, double _PosX, double _PosY)
{
	// toggle whether to show mouse position
	if (g_bShowMousePosition)
	{
		std::cout << "Cursor Coords: " << _PosX << ", " << _PosY << std::endl;
	}
}

// for single key press
void KeyInput(GLFWwindow* _Window, int _Key, int _ScanCode, int _Action, int _Mods)
{
	// toggle show mouse position in console
	if (_Key == GLFW_KEY_1 && _Action == GLFW_PRESS)
	{
		g_bShowMousePosition = !g_bShowMousePosition;
	}

	// toggle wireframe
	if (_Key == GLFW_KEY_2 && _Action == GLFW_PRESS)
	{
		g_bWireFrame = !g_bWireFrame;
	}

	// toggle show mouse
	if (_Key == GLFW_KEY_3 && _Action == GLFW_PRESS)
	{
		g_bShowMouse = !g_bShowMouse;
		// show mouse
		if (g_bShowMouse)
		{
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		}
		else
		{
			glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		}
	}

	// toggle automatic orbit
	if (_Key == GLFW_KEY_4 && _Action == GLFW_PRESS)
	{
		Camera->SetAutoCircle();
	}
}

// custom functions for tidy code
void InitialSetup()
{
	// load programs
	Program_FixedTri = ShaderLoader::CreateProgram("Resources/Shaders/FixedTriangle.vert.txt",
		"Resources/Shaders/FixedColor.frag.txt");

	Program_PositionOnly = ShaderLoader::CreateProgram("Resources/Shaders/PositionOnly.vert.txt",
		"Resources/Shaders/FixedColor.frag.txt");

	Program_Color = ShaderLoader::CreateProgram("Resources/Shaders/VertexColor.vert.txt",
		"Resources/Shaders/VertexColor.frag.txt");

	Program_ColorFade = ShaderLoader::CreateProgram("Resources/Shaders/VertexColor.vert.txt",
		"Resources/Shaders/VertexColorFade.frag.txt");

	Program_WorldSpace = ShaderLoader::CreateProgram("Resources/Shaders/WorldSpace.vert.txt",
		"Resources/Shaders/VertexColorFade.frag.txt");

	Program_Texture = ShaderLoader::CreateProgram("Resources/Shaders/Texture.vert.txt",
		"Resources/Shaders/TextureMix.frag.txt");

	Program_ClipSpace = ShaderLoader::CreateProgram("Resources/Shaders/HexOne.vert.txt",
		"Resources/Shaders/VertexColorFade.frag.txt");

	Program_Quads = ShaderLoader::CreateProgram("Resources/Shaders/Squares.vert.txt",
		"Resources/Shaders/Texture.frag.txt");

	// program for 3d model
	Program_3DModel = ShaderLoader::CreateProgram("Resources/Shaders/3DModel.vert.txt",
		"Resources/Shaders/3DModel.frag.txt");

	// flip image
	stbi_set_flip_vertically_on_load(true);

	// Load Image data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	// create and bind new texture
	glGenTextures(1, &Texture_Quag);
	glBindTexture(GL_TEXTURE_2D, Texture_Quag);

	unsigned char* ImageData = stbi_load("Resources/Textures/PolygonAncientWorlds_Statue_01.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);


	// Check if image is RGBA or RGB
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, ImageData);
	
	// generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);

	// second image 
	// Load Image data
	unsigned char* ImageData1 = stbi_load("Resources/Textures/Capguy_Walk.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);

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

	Cube = new CCube();

	Model = new CModel("Resources/Models/SM_Prop_Statue_02.obj");

	//ProjectionMat = glm::ortho(0.0f, (float)iWindowSize, (float)iWindowSize, 0.0f, 0.1f, 100.0f);

	//glm::vec3 ObjPosition = glm::vec3(0.0f, -100.0f, 0.0f);
	//float ObjRotationAngle = 0.0f;
	//glm::vec3 ObjScale = glm::vec3(400.0f, 400.0f, 1.0f);

	//ViewMat = glm::lookAt(CameraPos, CameraPos + CameraLookDir, CameraUpDir);

	// set background colour
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f);

	// Map the ange of the window for when the buffer clears
	glViewport(0, 0, iWindowSize, iWindowSize);

	float HalfWindow = (float)iWindowSize * 0.5;

	m_projMat = glm::ortho(-HalfWindow, HalfWindow, -HalfWindow, HalfWindow, 0.1f, 100.0f);
	m_lookDir = glm::vec3(0.0f, 0.0f, -1.0f);
	m_upDir = glm::vec3(0.0f, 1.0f, 0.0f);
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_viewMat = glm::lookAt(m_position, m_position + m_lookDir, m_upDir);

	// Mouse Callback
	glfwSetCursorPosCallback(Window, CursorPositionInput);
	// Toggle Callback
	glfwSetKeyCallback(Window, KeyInput);

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

	//HexRotationAngle += 0.5;

	//x++;
	//y++;
	//QuadPosition1 = glm::vec3(((x^2) % 2) - 0.5, ((y^2) % 2) - 0.5, 0);

		// calculate model matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), QuadPosition);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((QuadRotationAngle) * 10), glm::vec3(1.0f, 1.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), QuadScale);
	QuadModelMat = TranslationMat * RotationMat * ScaleMat;

	Camera->Update(CurrentTime, iWindowSize, Window, deltaTime);
	Camera->PrintCamPos();

	//system("CLS");

	// INPUTS

}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Cube->Render(Program_Quads, Texture_Quag, QuadModelMat, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat());
	Model->Render(Program_Quads, Texture_Quag, QuadModelMat, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat());

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