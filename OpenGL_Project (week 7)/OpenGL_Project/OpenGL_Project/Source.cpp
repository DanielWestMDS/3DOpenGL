#define STB_IMAGE_IMPLEMENTATION

// library define 
#define TINYOBJLOADER_IMPLEMENTATION

#include "ShaderLoader.h"
//#include "CShape.h"
#include "CAnimation.h"
#include "CCamera.h"
#include "CCube.h"
#include "CModel.h"

// global variables
GLFWwindow* Window = nullptr;
GLuint Program_FixedTri;
int iWindowSize = 800;

// pointer to shape objects
CShape* Hex1;
CShape* Hex2;
CSquare* Square1;
CSquare* Square2;
CAnimation* Animation;
CCamera* Camera;
CCube* Cube;
CModel* Model;

glm::mat4 m_projMat;

glm::vec3 m_lookDir;
glm::vec3 m_upDir;
glm::vec3 m_position;

glm::mat4 m_viewMat;

// VAO vars
GLuint VAO_Tri;
GLuint VAO_Tri2;
GLuint VAO_Quad;
GLuint VAO_Quad1;

// VBO vars
GLuint VBO_Tri;
GLuint VBO_Tri2;
GLuint VBO_Quad;
GLuint VBO_Quad1;

// EBO vars
GLuint EBO_Quad;
GLuint EBO_Quad1;

// programs
GLuint Program_Color;
GLuint Program_ColorFade;
GLuint Program_PositionOnly;
GLuint Program_WorldSpace;
GLuint Program_Texture;
GLuint Program_Animation;
GLuint Program_ClipSpace;
GLuint Program_Quads;

// texture
GLuint Texture_Awesome;
GLuint Texture_Quag;
//GLuint Texture_Animation;
GLuint Texture_Animation[6];

// Object Matrices and Components
glm::vec3 QuadPosition = glm::vec3(0.5f, 0.5f, -1.0f);
glm::mat4 TranslationMat;

float QuadRotationAngle = 0.0f;
glm::mat4 RotationMat;

glm::vec3 QuadScale = glm::vec3(1.0f, 1.0f, 1.0f);
glm::mat4 ScaleMat;

glm::mat4 QuadModelMat;

// Animation
glm::vec3 HexPosition1 = glm::vec3(200.0f, 200.0f, -1.0f);
glm::mat4 HexTranslationMat1;

glm::mat4 HexRotationMat1;

glm::vec3 HexScale1 = glm::vec3(200.0f, 200.0f, 1.0f);
glm::mat4 HexScaleMat1;

glm::mat4 HexModelMat1;

// hex 2
glm::vec3 HexPosition = glm::vec3(-200.0f, -200.0f, -1.0f);
glm::mat4 HexTranslationMat;

float HexRotationAngle = 0.0f;
glm::mat4 HexRotationMat;

glm::vec3 HexScale = glm::vec3(200.0f, 200.0f, 1.0f);
glm::mat4 HexScaleMat;

glm::mat4 HexModelMat;

// animated quad
// quad 2
glm::vec3 QuadPosition2 = glm::vec3(200.0f, -200.0f, -1.0f);
glm::mat4 TranslationMat2;

float QuadRotationAngle2 = 0.0f;
glm::mat4 RotationMat2;

glm::vec3 QuadScale2 = glm::vec3(200.0f, 200.0f, 1.0f);
glm::mat4 ScaleMat2;

glm::mat4 AnimModelMat;

// camera matrices
glm::mat4 ProjectionMat;
glm::mat4 ViewMat;
glm::mat4 ObjModelMat;

glm::vec3 CameraPos;
glm::vec3 CameraLookDir;
glm::vec3 CameraUpDir = glm::vec3(0.0f, 1.0f, 0.0f);

int x = 0;
int y = 0;

int frame = 0;
float FrameInterval = 1;
float StartTime;

// time
float CurrentTime;
float PreviousTime;
float deltaTime;

void AnimateTexture(GLuint _texture)
{
	if (CurrentTime >= 3)
	{
		glfwSetTime(0);
	}
};

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

	// flip image
	stbi_set_flip_vertically_on_load(true);

	// Load Image data
	int ImageWidth;
	int ImageHeight;
	int ImageComponents;

	//animation
	unsigned char* Frame1 = stbi_load("Resources/Textures/Frame1.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);

	// create and bind new texture
	glGenTextures(1, &Texture_Animation[0]);
	glBindTexture(GL_TEXTURE_2D, Texture_Animation[0]);

	// Check if image is RGBA or RGB
	GLint LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, Frame1);

	// generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	// free memory
	stbi_image_free(Frame1);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned char* Frame2 = stbi_load("Resources/Textures/Frame2.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);

	// create and bind new texture
	glGenTextures(1, &Texture_Animation[1]);
	glBindTexture(GL_TEXTURE_2D, Texture_Animation[1]);

	// Check if image is RGBA or RGB
	LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, Frame2);

	// generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	// free memory
	stbi_image_free(Frame2);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned char* Frame3 = stbi_load("Resources/Textures/Frame3.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);

	// create and bind new texture
	glGenTextures(1, &Texture_Animation[2]);
	glBindTexture(GL_TEXTURE_2D, Texture_Animation[2]);

	// Check if image is RGBA or RGB
	LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, Frame3);

	// generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	// free memory
	stbi_image_free(Frame3);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned char* Frame4 = stbi_load("Resources/Textures/Frame4.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);

	// create and bind new texture
	glGenTextures(1, &Texture_Animation[3]);
	glBindTexture(GL_TEXTURE_2D, Texture_Animation[3]);

	// Check if image is RGBA or RGB
	LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, Frame4);

	// generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	// free memory
	stbi_image_free(Frame4);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned char* Frame5 = stbi_load("Resources/Textures/Frame5.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);

	// create and bind new texture
	glGenTextures(1, &Texture_Animation[4]);
	glBindTexture(GL_TEXTURE_2D, Texture_Animation[4]);

	// Check if image is RGBA or RGB
	LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, Frame5);

	// generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	// free memory
	stbi_image_free(Frame5);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned char* Frame6 = stbi_load("Resources/Textures/Frame6.png", &ImageWidth, &ImageHeight, &ImageComponents, 0);

	// create and bind new texture
	glGenTextures(1, &Texture_Animation[5]);
	glBindTexture(GL_TEXTURE_2D, Texture_Animation[5]);

	// Check if image is RGBA or RGB
	LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

	// Populate the texture with the image
	glTexImage2D(GL_TEXTURE_2D, 0, LoadedComponents, ImageWidth, ImageHeight, 0, LoadedComponents, GL_UNSIGNED_BYTE, Frame6);

	// generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	// free memory
	stbi_image_free(Frame6);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	unsigned char* ImageData = stbi_load("Resources/Textures/Boots.jpg",
		&ImageWidth, &ImageHeight, &ImageComponents, 0);

	// create and bind new texture
	glGenTextures(1, &Texture_Quag);
	glBindTexture(GL_TEXTURE_2D, Texture_Quag);


	// Check if image is RGBA or RGB
	LoadedComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

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

	Hex1 = new CShape();
	Hex2 = new CShape();

	Square1 = new CSquare();
	Square2 = new CSquare();

	Animation = new CAnimation();

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
}

void Update()
{
	glfwPollEvents();

	// get current time
	CurrentTime = (float)glfwGetTime();
	deltaTime = CurrentTime - PreviousTime;
	PreviousTime = CurrentTime;

	HexRotationAngle += 0.5;

	//x++;
	//y++;
	//QuadPosition1 = glm::vec3(((x^2) % 2) - 0.5, ((y^2) % 2) - 0.5, 0);

		// calculate model matrix
	TranslationMat = glm::translate(glm::mat4(1.0f), QuadPosition);
	RotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((HexRotationAngle) * 10), glm::vec3(1.0f, 1.0f, 1.0f));
	ScaleMat = glm::scale(glm::mat4(1.0f), QuadScale);
	QuadModelMat = TranslationMat * RotationMat * ScaleMat;

	// calculate model matrix
	HexTranslationMat1 = glm::translate(glm::mat4(1.0f), HexPosition1 /** cos(CurrentTime)*/);
	HexRotationMat1 = glm::rotate(glm::mat4(1.0f), glm::radians((HexRotationAngle) /** 10*/), glm::vec3(0.0f, 0.0f, 1.0f));
	HexScaleMat1 = glm::scale(glm::mat4(1.0f), HexScale1 /** cos(CurrentTime)*/);
	HexModelMat1 = HexTranslationMat1 * HexRotationMat1 * HexScaleMat1;

	// calculate model matrix
	TranslationMat2 = glm::translate(glm::mat4(1.0f), QuadPosition2 /** cos(CurrentTime)*/);
	RotationMat2 = glm::rotate(glm::mat4(1.0f), glm::radians((QuadRotationAngle2) /** 10*/), glm::vec3(0.0f, 0.0f, 1.0f));
	ScaleMat2 = glm::scale(glm::mat4(1.0f), QuadScale2 /** cos(CurrentTime)*/);
	AnimModelMat = TranslationMat2 * RotationMat2 * ScaleMat2;

	// calculate model matrix
	HexTranslationMat = glm::translate(glm::mat4(1.0f), HexPosition /** cos(CurrentTime)*/);
	HexRotationMat = glm::rotate(glm::mat4(1.0f), glm::radians((HexRotationAngle) /** 10*/), glm::vec3(0.0f, 0.0f, 1.0f));
	HexScaleMat = glm::scale(glm::mat4(1.0f), HexScale /** cos(CurrentTime)*/);
	HexModelMat = HexTranslationMat * HexRotationMat * HexScaleMat;

	if (StartTime >= CurrentTime + FrameInterval)
	{
		if (frame >= 5)
		{
			frame = 0;
		}
		else
		{
			frame++;
		}

		StartTime = CurrentTime;
	}
	else
	{
		StartTime += 0.5;
	}

	Camera->Update(CurrentTime, iWindowSize);
}

void Render()
{
	glClear(GL_COLOR_BUFFER_BIT);

	//Hex1->Render(Program_ClipSpace, Texture_Quag, HexModelMat1, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat(), glm::vec3(0.0f, 1.0f, 1.0f));
	//Hex2->Render(Program_ClipSpace, Texture_Quag, HexModelMat, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat(), glm::vec3(0.0f, 1.0f, 1.0f));

	//Square1->Render(Program_Quads, Texture_Quag, QuadModelMat, CurrentTime);
	//Square2->Render(Program_Texture, Texture_Awesome, QuadModelMat, CurrentTime);

	//Animation->Render(Program_Quads, Texture_Animation[frame], AnimModelMat, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat());

	Cube->Render(Program_Quads, Texture_Quag, QuadModelMat, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat());
	Model->Render(Program_Quads, Texture_Quag, QuadModelMat, CurrentTime, Camera->GetProjMat(), Camera->GetViewMat());

	// unbind
	glBindVertexArray(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
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

	//// Request OpenGL 3.3 core profile
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

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