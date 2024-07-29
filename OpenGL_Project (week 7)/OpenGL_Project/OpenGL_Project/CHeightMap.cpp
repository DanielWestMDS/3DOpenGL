#include "CHeightMap.h"

CHeightMap::CHeightMap(HeightMapInfo& _BuildInfo)
{
	if (LoadHeightMap(_BuildInfo))
	{
		BuildVertexData(_BuildInfo);
		BuildEBO(_BuildInfo);
	}
	else
	{
		std::cout << "Height Map failed" << std::endl;
	}
}

CHeightMap::~CHeightMap()
{
}

bool CHeightMap::LoadHeightMap(HeightMapInfo& _BuildInfo)
{
	unsigned int VertexCount = _BuildInfo.Width * _BuildInfo.Depth;
	// vector contains height infor for each vertex
	std::vector<unsigned char> HeightValue(VertexCount);

	// read the data from RAW file
	std::ifstream File;
	File.open(_BuildInfo.FilePath, std::ios_base::binary);
	if (File)
	{
		// read the raw bytes
		File.read((char*)&HeightValue[0], (std::streamsize)HeightValue.size());
		File.close();
	}
	else
	{
		std::cout << "HEIGHT MAP FAILED TO LOAD: " << _BuildInfo.FilePath << std::endl;
		return false;
	}

	// transfer data to float array
	m_fHeightMap.resize(VertexCount, 0);
	for (unsigned int i = 0; i < VertexCount; i++)
	{
		m_fHeightMap[i] = (float)HeightValue[i];
	}
	return true;
}

void CHeightMap::BuildVertexData(HeightMapInfo& _BuildInfo)
{
	unsigned int VertexCount = _BuildInfo.Width * _BuildInfo.Depth;
	std::vector<VertexStandard> Vertices(VertexCount);

	float HalfWidth = (_BuildInfo.Width - 1) * _BuildInfo.CellSpacing * 0.5;
	float HalfDepth = (_BuildInfo.Depth - 1) * _BuildInfo.CellSpacing * 0.5;

	// texture UV map
	float TexU = 1.0f / (float)(_BuildInfo.Width - 1);
	float TexV = 1.0f / (float)(_BuildInfo.Depth - 1);

	for (unsigned int Row = 0; Row < _BuildInfo.Width; Row++)
	{
		float PosZ = HalfDepth - (Row * _BuildInfo.CellSpacing);

		for (unsigned int Col = 0; Col < _BuildInfo.CellSpacing; Col++)
		{
			int Index = Row * _BuildInfo.Width + Col;

			float PosX = HalfWidth + (Col * _BuildInfo.CellSpacing);
			float PosY = m_fHeightMap[Index];

			Vertices[Index].Position = glm::vec3(PosX, PosY, PosZ);
			// add a multiplier here to repeat texture
			Vertices[Index].TexCoord = glm::vec2(Col * TexU, Row * TexV);
			Vertices[Index].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
		}
	}

	// calculate normal using central difference
	float InvCellSpacing = 1.0f / (2.0f * _BuildInfo.CellSpacing);
	for (unsigned int Row = 0; Row < _BuildInfo.Width; Row++)
	{
		for (unsigned int Col = 0; Col < _BuildInfo.Depth; Col++)
		{
			float RowNeg = m_fHeightMap[(Row == 0 ? Row : Row - 1) * _BuildInfo.Depth + Col];
			float RowPos = m_fHeightMap[(Row < _BuildInfo.Width - 1 ? Row + 1 : Row) * _BuildInfo.Depth + Col];
			float ColNeg = m_fHeightMap[Row * _BuildInfo.Depth + (Col == 0 ? Col : Col - 1)];
			float ColPos = m_fHeightMap[Row * _BuildInfo.Depth + (Col < _BuildInfo.Depth - 1 ? Col + 1 : Col)];

			float X = (RowNeg - RowPos);
			if (Row == 0 || Row == _BuildInfo.Width - 1)
			{
				X *= 2.0f;
			}

			float Y = (ColPos - ColNeg);
			if (Col == 0 || Col == _BuildInfo.Depth - 1)
			{
				Y *= 2.0f;
			}

			glm::vec3 TangentZ(0.0f, X * InvCellSpacing, 1.0f);
			glm::vec3 TangentX(1.0f, Y * InvCellSpacing, 0.0f);

			glm::vec3 Normal = glm::cross(TangentZ, TangentX);
			Normal = glm::normalize(Normal);

			Vertices[Row * _BuildInfo.Depth + Col].Normal = Normal;
		}
	}
}

void CHeightMap::BuildEBO(HeightMapInfo& _BuildInfo)
{
	m_DrawType = GL_TRIANGLES;
	unsigned int FaceCount = (_BuildInfo.Width - 1) * (_BuildInfo.Depth - 1) * 2;
	m_DrawCount = FaceCount * 3;
	std::vector<GLuint> Indices(m_DrawCount);

	int Index = 0;
	// create a grid of quads
	for (unsigned int Row = 0; Row < (_BuildInfo.Width - 1); Row++)
	{
		for (unsigned int Col = 0; Col < (_BuildInfo.Depth - 1); Col++)
		{
			// triangle 1 
			Indices[Index++] = Row * _BuildInfo.Depth + Col;
			Indices[Index++] = Row * _BuildInfo.Depth + Col + 1;
			Indices[Index++] = (Row + 1) * _BuildInfo.Depth + Col;

			Indices[Index++] = (Row + 1) * _BuildInfo.Depth + Col;
			Indices[Index++] = Row * _BuildInfo.Depth + Col + 1;
			Indices[Index++] = (Row + 1) * _BuildInfo.Depth + Col + 1;
		}
	}

	// build EBO
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

	// VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// vertex attribute pointers
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}
