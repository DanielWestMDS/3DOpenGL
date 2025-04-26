// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CModel.h
// Description : Loads a model from an obj file. Contains Render for a single object and Render instanced for instanced model loading.
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#pragma once
#include <tiny_obj_loader.h>
#include "CCamera.h"
#include <iostream>

struct AxisAlignedBoundingBox
{
	glm::vec3 Min;
	glm::vec3 Max;
};

/// <summary>
/// Holds vertex information for each point on the model.
/// </summary>
struct VertexStandard
{
public:
	glm::vec3 position;
	glm::vec2 texcoord;
	glm::vec3 normal;

	VertexStandard()
	{
		normal = glm::vec3(0.0f);
		position = glm::vec3(0.0f);
		texcoord = glm::vec2(0.0f);
	};
	VertexStandard(glm::vec3 pos, glm::vec2 texc)
	{
		normal = glm::vec3(0.0f);
		position = pos;
		texcoord = texc;
	}
};

class CModel
{

public:
	/// <summary>
	/// Constructor. Reads data from an obj file passed via filepath and creates vertex information using the struct. Also creates and binds VBO
	/// </summary>
	/// <param name="FilePath"></param>
	/// <param name="_program"></param>
	/// <param name="_texture"></param>
	/// <param name="_matrix"></param>
	CModel(std::string FilePath, GLint _program, GLint _texture, glm::vec3 _position, float _scale, float _rotationAngle, glm::vec3 _rotationMat);

	/// <summary>
	/// destructor
	/// </summary>
	~CModel();

	/// <summary>
	/// Updates matrices so the model is displayed correctly after the camera moves
	/// </summary>
	/// <param name="_projMat"></param>
	/// <param name="_viewMat"></param>
	/// <param name="_cameraPos"></param>
	virtual void Update(glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _cameraPos);

	/// <summary>
	/// Binds VAO and passes matrices into shader
	/// </summary>
	virtual void Render();

	/// <summary>
	/// Calculates shadow. passes in necessary uniforms
	/// </summary>
	virtual void RenderShadow(GLuint _ShadowProgram, glm::mat4 _LightVP);

	/// <summary>
	/// Instanced rendering with uniforms for geometry
	/// </summary>
	/// <param name="_program"></param>
	/// <param name="_texture"></param>
	/// <param name="_instancePositions"></param>
	/// <param name="_modelMat"></param>
	/// <param name="_cameraPos"></param>
	/// <param name="_VP"></param>
	virtual void RenderGeometryInstanced(GLint _program, GLint _texture, std::vector<glm::vec3> _instancePositions, glm::mat4 _modelMat, glm::vec3 _cameraPos, glm::mat4 _VP);

	/// <summary>
	/// getter for VAO
	/// </summary>
	/// <returns></returns>
	GLuint GetVAO() { return VAO; };

	/// <summary>
	/// getter for model matrix
	/// </summary>
	/// <returns></returns>
	glm::mat4 GetModelMat() { return m_modelMat; };


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
	/// sets the new model matrix
	/// </summary>
	/// <param name="_newMatrix"></param>
	void SetModelMat(glm::mat4 _newMatrix) { m_modelMat = _newMatrix; };

	void SetPosition(glm::vec3 _newPosition);

	void SetRotation(glm::vec3 _axis, float _amount);

	void SetScale(float _newScale);

	glm::vec3 GetPosition();

	AxisAlignedBoundingBox ComputeLocalAABB(const std::vector<glm::vec3>& vertices);

	AxisAlignedBoundingBox GetWorldAABB();

protected:
	GLuint VAO;
	GLuint DrawCount;
	GLuint InstanceBuffer;
	int DrawType;
	int m_CountInstanced;

	float m_fScale;
	glm::vec3 m_Position;
	float m_fRotationAngle = 0.0f;
	glm::vec3 m_RotationAxis = glm::vec3(0.0f);

	AxisAlignedBoundingBox m_LocalAABB;

	GLint m_program = 0;
	GLint m_texture = 0;
	GLint m_shadowTexture = 0;
	glm::mat4 m_modelMat = glm::mat4();
	glm::mat4 m_projMat = glm::mat4();
	glm::mat4 m_viewMat = glm::mat4();
	glm::vec3 m_cameraPos = glm::vec3();

	float m_fShininess;
};

