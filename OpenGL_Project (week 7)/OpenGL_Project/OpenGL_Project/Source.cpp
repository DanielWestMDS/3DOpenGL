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
// tree
CModel* Tree;

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

glm::vec3 QuadScale = glm::vec3(0.05f, 0.05f, 0.05f);
glm::mat4 ScaleMat;

glm::mat4 QuadModelMat;


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

// camera matrices
glm::mat4 ProjectionMat;
glm::mat4 ViewMat;
glm::mat4 ObjModelMat;

glm::vec3 CameraPos;
glm::vec3 CameraLookDir;
glm::vec3 CameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);

// Vector for instanced matrices
std::vector<glm::mat4> MVPVec;
// for random tree positions
std::vector<glm::vec3> RandomLocations;
//glm::mat4 MVPVec[256];

int x = 0;
int y = 0;

// obj count 
int g_objCount = 1000;

// time
float CurrentTime;
float PreviousTime;
float deltaTime;

// for input object
float MoveSpeed = 4.0f;

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

	// for object
	// move forward
	if (glfwGetKey(_Window, GLFW_KEY_W))
	{
		QuadPosition += Camera->GetForward() * deltaTime * MoveSpeed;
	}

	if (glfwGetKey(_Window, GLFW_KEY_S))
	{
		QuadPosition -= Camera->GetForward() * deltaTime * MoveSpeed;
	}

	if (glfwGetKey(_Window, GLFW_KEY_A))
	{
		QuadPosition -= Camera->GetRight() * deltaTime * MoveSpeed;
	}

	if (glfwGetKey(_Window, GLFW_KEY_D))
	{
		QuadPosition += Camera->GetRight() * deltaTime * MoveSpeed;
	}

	if (glfwGetKey(_Window, GLFW_KEY_Q))
	{
		QuadPosition -= Camera->GetUp() * deltaTime * MoveSpeed;
	}

	if (glfwGetKey(_Window, GLFW_KEY_E))
	{
		QuadPosition += Camera->GetUp() * deltaTime * MoveSpeed;
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

	unsigned char* ImageData = stbi_load("Resources/Textures/PolygonAncientWorlds_Texture_01_A.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);


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

	Tree = new CModel("Resources/Models/SM_Env_Tree_Palm_01.obj");

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
		MVPVec.push_back(QuadModelMat);
	}

	// set matrices as instanced vertex attribute
	GLuint VBO_Instanced;
	glGenBuffers(1, &VBO_Instanced);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
	glBufferData(GL_ARRAY_BUFFER, 100 * sizeof(glm::mat4), &RandomLocations[0], GL_STATIC_DRAW);

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

	//HexRotationAngle += 0.5;

	//x++;
	//y++;
	//QuadPosition1 = glm::vec3(((x^2) % 2) - 0.5, ((y^2) % 2) - 0.5, 0);

		// calculate model matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), QuadPosition);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((QuadRotationAngle) * 10), glm::vec3(1.0f, 1.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), QuadScale);
	QuadModelMat = TranslationMat * RotationMat * ScaleMat;

	QuadModelMat = Camera->GetProjMat() * Camera->GetViewMat() * QuadModelMat;

	// for instanced matrices
	TreeTranslationMat = glm::translate(glm::mat4(1.0f), TreePosition);
	TreeRotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((TreeRotationAngle) * 10), glm::vec3(1.0f, 1.0f, 1.0f));
	TreeScaleMat = glm::scale(glm::mat4(1.0f), TreeScale);
	// multiply translation rotation scale for model matrix
	TreeModelMat = TreeTranslationMat * TreeRotationMat * TreeScaleMat;
	// get MVP by multiplying by camera projection and view matrices
	TreeModelMat = Camera->GetProjMat() * Camera->GetViewMat() * TreeModelMat;

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
	Camera->Update(CurrentTime, iWindowSize, Window, deltaTime);
	Camera->PrintCamPos();

	//system("CLS");

	// INPUTS

}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Cube->Render(Program_Quads, Texture_Quag, QuadModelMat, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat());
	Model->Render(Program_Quads, Texture_Quag, QuadModelMat, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat());

	// many trees
	Tree->RenderInstanced(Program_3DModel, Texture_Quag, MVPVec, CurrentTime, TreeModelMat);
	

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