#pragma once

// Library Includes
#include <glew.h>
#include <glfw3.h>
#include <iostream>

class ShaderLoader
{
	
public:	
	static GLuint CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);

	static GLuint CreateProgram_C(std::string ComputeShaderFilename);

	static GLuint CreateProgram_VTF(const char* vertex, const char* TCS, const char* TES, const char* Fragment);

private:
	ShaderLoader(void);
	~ShaderLoader(void);
	static GLuint CreateShader(GLenum shaderType, const char* shaderName);
	static std::string ReadShaderFile(const char *filename);
	static void PrintErrorDetails(bool isShader, GLuint id, const char* name);
};