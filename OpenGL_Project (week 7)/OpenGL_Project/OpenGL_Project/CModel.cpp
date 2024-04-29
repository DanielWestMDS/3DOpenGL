#include "CModel.h"

// library define 
#define TINYOBJLOADER_IMPLEMENTATION

CModel::CModel(std::string FilePath)
{
	std::vector<VertexStandard> Vertices;
	tinyobj::ObjReader Reader;
	tinyobj::ObjReaderConfig ReaderConfig;

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
}
