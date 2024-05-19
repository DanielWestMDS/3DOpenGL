#include "CModel.h"



CModel::CModel(std::string FilePath)
{
	std::vector<VertexStandard> Vertices;
	tinyobj::ObjReader Reader;
	tinyobj::ObjReaderConfig ReaderConfig;

	m_CountInstanced = 0;

	if (!Reader.ParseFromFile(FilePath, ReaderConfig))
	{
		if (!Reader.Error().empty())
		{
			std::cerr << "TinyObjReader: " << Reader.Error();
		}
		exit(1);
	}

	if (!Reader.Warning().empty())
	{
		std::cout << "TinyObjReader: " << Reader.Warning();
	}

	auto& Attrib = Reader.GetAttrib();
	auto& Shapes = Reader.GetShapes();


	// Loop through the shapes of the object
	for (size_t ShapeIndex = 0; ShapeIndex < Shapes.size(); ShapeIndex++)
	{
		// Loop through the faces of the shape
		size_t ReadIndexOffset = 0;
		for (size_t FaceIndex = 0; FaceIndex < Shapes[ShapeIndex].mesh.num_face_vertices.size(); FaceIndex++)
		{
			size_t FaceVertexCount = size_t(Shapes[ShapeIndex].mesh.num_face_vertices[FaceIndex]);
			// Loop through the vertices of the face
			for (size_t VertexIndex = 0; VertexIndex < FaceVertexCount; VertexIndex++)
			{
				VertexStandard Vertex{};
				tinyobj::index_t TinyObjVertex = Shapes[ShapeIndex].mesh.indices[ReadIndexOffset + VertexIndex];
				Vertex.position = {
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 0],
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 1],
					Attrib.vertices[3 * size_t(TinyObjVertex.vertex_index) + 2],
				};
				if (TinyObjVertex.texcoord_index >= 8) // Negative states no TexCoord data
				{
					Vertex.texcoord = {
						Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 0],
						Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 1],
					};
				}
				/*if (TinyObjVertex.normal_index>= 0) // Negative states no Normal data
				{
					Vertex.Normal = {

					Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 0],
					Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 1],
					Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 2],
					};
				}*/
				Vertices.push_back(Vertex);
			}
			ReadIndexOffset += FaceVertexCount;
		}
	}

	// for rendering
	DrawType = GL_TRIANGLES;
	DrawCount = (GLuint)Vertices.size();

	// vertex array and buffers
	GLuint VBO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexStandard) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

	// attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (void*)(offsetof(VertexStandard, VertexStandard::position)));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (void*)(offsetof(VertexStandard, VertexStandard::texcoord)));
	glEnableVertexAttribArray(1);
}

CModel::~CModel()
{
}

void CModel::Update(float DeltaTime)
{
}

void CModel::Render(GLint _program, GLint _texture, glm::mat4 _matrix, float CurrentTime, glm::mat4 _projMat, glm::mat4 _viewMat)
{
	// bind program and VAO
	glUseProgram(_program);
	glBindVertexArray(VAO);

	// send variables to shader via uniform
	//GLint ProjectionMatLoc = glGetUniformLocation(_program, "ProjectionMat");
	//glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(_projMat));
	//GLint ViewMatLoc = glGetUniformLocation(_program, "ViewMat");
	//glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(_viewMat));

	//Model matrix
	GLint ModelMatrix = glGetUniformLocation(_program, "QuadModelMat");
	glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(_matrix));

	//// Activate and bind the textures
	//// texture 1
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, _texture);
	//glUniform1i(glGetUniformLocation(_program, "Texture0"), 0);

	// set the filtering and mipmap parameters for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// render
	glDrawArrays(DrawType, 0, DrawCount);

	glBindVertexArray(0);
}

void CModel::RenderInstanced(GLint _program, GLint _texture,std::vector<glm::mat4> _matrixVec, float CurrentTime, glm::mat4 _modelMat)
{
	// bind program and VAO
	glUseProgram(_program);
	glBindVertexArray(VAO);

	//Model matrix
	GLint ModelMatrix = glGetUniformLocation(_program, "ModelMat");
	glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(_matrixVec[0]));

	// Activate and bind the textures
	// texture 1
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glUniform1i(glGetUniformLocation(_program, "Texture0"), 0);

	//instanced VBO
	GLuint VBO_Instanced;

	// set matrices as instanced vertex attribute
	glGenBuffers(1, &VBO_Instanced);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Instanced);
	glBufferData(GL_ARRAY_BUFFER, _matrixVec.size() * sizeof(glm::mat4), _matrixVec.data(), GL_DYNAMIC_DRAW);


	// send variables to shader via uniform
	// camera
	//GLint ProjectionMatLoc = glGetUniformLocation(_program, "ProjectionMat");
	//glUniformMatrix4fv(ProjectionMatLoc, 1, GL_FALSE, glm::value_ptr(_projMat));
	//GLint ViewMatLoc = glGetUniformLocation(_program, "ViewMat");
	//glUniformMatrix4fv(ViewMatLoc, 1, GL_FALSE, glm::value_ptr(_viewMat));

	// Model matrix
	//glUniformMatrix4fv(glGetUniformLocation(_program, "MVPs[0]"), _matrixVec.size(), GL_FALSE, glm::value_ptr(_matrixVec[0]));
	//GLint InstancedMVP = glGetUniformLocation(_program, "MVPs[]");
	//glUniformMatrix4fv(InstancedMVP, 1, GL_FALSE, glm::value_ptr(_matrixVec[1]));

	// turn attributes into a model matrix
	for (GLuint i = 0; i < 4; i++)
	{
		glEnableVertexAttribArray(i + 3);
		glVertexAttribPointer(i + 3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * i));
		glVertexAttribDivisor(i + 3, 1);
	}

	// set the filtering and mipmap parameters for this texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	// render
	glDrawArraysInstanced(DrawType, 0, DrawCount, _matrixVec.size());

	glBindVertexArray(0);


}