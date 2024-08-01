#include "CHeightMap.h"

CHeightMap::CHeightMap(HeightMapInfo& _BuildInfo)
{
	if (LoadHeightMap(_BuildInfo))
	{
		BuildVertexData(_BuildInfo);
		std::cout << "Height Should have loaded successfully" << std::endl;
	}
	else
	{
		std::cout << "Height Map failed" << std::endl;
	}
}

CHeightMap::~CHeightMap()
{
}

void CHeightMap::Render(GLint _program, GLint _texture, glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _cameraPos, glm::mat4 _matrix)
{
	// bind program and VAO
	glUseProgram(_program);
	glBindVertexArray(m_VAO);

	// Model matrix
	GLint ModelMatrix = glGetUniformLocation(_program, "ModelMat");
	glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(_matrix));

	// pass camera position in via uniform
	GLint CameraPosLoc = glGetUniformLocation(_program, "CameraPos");
	glUniform3fv(CameraPosLoc, 1, glm::value_ptr(_cameraPos));

	// pass in view projection
	GLint VPMat = glGetUniformLocation(_program, "VP");
	glUniformMatrix4fv(VPMat, 1, GL_FALSE, glm::value_ptr(_projMat * _viewMat));

    // Activate and bind the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(glGetUniformLocation(_program, "Texture0"), 0);

	// render
	glDrawElements(m_DrawType, m_DrawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

    glUseProgram(0);
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
    std::vector<VertexStandardHeightMap> Vertices(VertexCount);

    float HalfWidth = (_BuildInfo.Width - 1) * _BuildInfo.CellSpacing * 0.5f;
    float HalfDepth = (_BuildInfo.Depth - 1) * _BuildInfo.CellSpacing * 0.5f;

    // texture UV map
    float TexU = 1.0f / (float)(_BuildInfo.Width - 1);
    float TexV = 1.0f / (float)(_BuildInfo.Depth - 1);

    for (unsigned int Row = 0; Row < _BuildInfo.Depth; Row++)
    {
        float PosZ = HalfDepth - (Row * _BuildInfo.CellSpacing);

        for (unsigned int Col = 0; Col < _BuildInfo.Width; Col++)
        {
            int Index = Row * _BuildInfo.Width + Col;

            float PosX = -HalfWidth + (Col * _BuildInfo.CellSpacing);
            float PosY = m_fHeightMap[Index];

            Vertices[Index].Position = glm::vec3(PosX, PosY, PosZ);
            Vertices[Index].TexCoord = glm::vec2(Col * TexU, Row * TexV);
            Vertices[Index].Normal = glm::vec3(0.0f, 1.0f, 0.0f);


            // Assign color based on height
            Vertices[Index].Color = CalculateColor(PosY);
        }
    }

    SmoothHeights(_BuildInfo);
    SmoothHeights(_BuildInfo);
    SmoothHeights(_BuildInfo);
    SmoothHeights(_BuildInfo);
    SmoothHeights(_BuildInfo);
    SmoothHeights(_BuildInfo);
    SmoothHeights(_BuildInfo);

    // calculate normal using central difference
    float InvCellSpacing = 1.0f / (2.0f * _BuildInfo.CellSpacing);
    for (unsigned int Row = 0; Row < _BuildInfo.Depth; Row++)
    {
        for (unsigned int Col = 0; Col < _BuildInfo.Width; Col++)
        {
            float RowNeg = m_fHeightMap[(Row == 0 ? Row : Row - 1) * _BuildInfo.Width + Col];
            float RowPos = m_fHeightMap[(Row < _BuildInfo.Depth - 1 ? Row + 1 : Row) * _BuildInfo.Width + Col];
            float ColNeg = m_fHeightMap[Row * _BuildInfo.Width + (Col == 0 ? Col : Col - 1)];
            float ColPos = m_fHeightMap[Row * _BuildInfo.Width + (Col < _BuildInfo.Width - 1 ? Col + 1 : Col)];

            float X = (RowNeg - RowPos);
            if (Row == 0 || Row == _BuildInfo.Depth - 1)
            {
                X *= 2.0f;
            }

            float Y = (ColPos - ColNeg);
            if (Col == 0 || Col == _BuildInfo.Width - 1)
            {
                Y *= 2.0f;
            }

            glm::vec3 TangentZ(0.0f, X * InvCellSpacing, 1.0f);
            glm::vec3 TangentX(1.0f, Y * InvCellSpacing, 0.0f);

            glm::vec3 Normal = glm::cross(TangentZ, TangentX);
            Normal = glm::normalize(Normal);

            Vertices[Row * _BuildInfo.Width + Col].Normal = Normal;
        }
    }



    m_DrawType = GL_TRIANGLES;
    unsigned int FaceCount = (_BuildInfo.Width - 1) * (_BuildInfo.Depth - 1) * 2;
    m_DrawCount = FaceCount * 3;
    std::vector<GLuint> Indices(m_DrawCount);

    int Index = 0;
    // create a grid of quads
    for (unsigned int Row = 0; Row < (_BuildInfo.Depth - 1); Row++)
    {
        for (unsigned int Col = 0; Col < (_BuildInfo.Width - 1); Col++)
        {
            // triangle 1 
            Indices[Index++] = Row * _BuildInfo.Width + Col;
            Indices[Index++] = Row * _BuildInfo.Width + Col + 1;
            Indices[Index++] = (Row + 1) * _BuildInfo.Width + Col;

            // triangle 2
            Indices[Index++] = (Row + 1) * _BuildInfo.Width + Col;
            Indices[Index++] = Row * _BuildInfo.Width + Col + 1;
            Indices[Index++] = (Row + 1) * _BuildInfo.Width + Col + 1;
        }
    }

    // vertex array and buffers
    GLuint VBO;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexStandardHeightMap) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * Indices.size(), Indices.data(), GL_STATIC_DRAW);

    // attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandardHeightMap), (void*)(offsetof(VertexStandardHeightMap, VertexStandardHeightMap::Position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStandardHeightMap), (void*)(offsetof(VertexStandardHeightMap, VertexStandardHeightMap::TexCoord)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandardHeightMap), (void*)(offsetof(VertexStandardHeightMap, VertexStandardHeightMap::Normal)));
    glEnableVertexAttribArray(2);

    // instanced VBO
    //glGenBuffers(1, &m_InstanceBuffer);

    // Unbind the VAO to avoid accidental modifications
    glBindVertexArray(0);
}

float CHeightMap::Average(unsigned int _row, unsigned int _col, HeightMapInfo& _BuildInfo)
{
    float sum = 0.0f;
    int count = 0;

    // Loop over the 3x3 neighborhood
    for (int row = -1; row <= 1; row++)
    {
        for (int col = -1; col <= 1; col++)
        {
            int neighborRow = static_cast<int>(_row) + row;
            int neighborCol = static_cast<int>(_col) + col;

            // Check if the neighbor is within the heightmap bounds
            if (neighborRow >= 0 && neighborRow < static_cast<int>(_BuildInfo.Depth) &&
                neighborCol >= 0 && neighborCol < static_cast<int>(_BuildInfo.Width))
            {
                sum += m_fHeightMap[neighborRow * _BuildInfo.Width + neighborCol];
                count++;
            }
        }
    }

    // Return the average value
    return count > 0 ? sum / count : 0.0f;
    //return 0;
}

void CHeightMap::SmoothHeights(HeightMapInfo& _BuildInfo)
{
    std::vector<float> SmoothedMap(m_fHeightMap.size());

    for (unsigned int Row = 0; Row < _BuildInfo.Width; Row++)
    {
        for (unsigned int Col = 0; Col < _BuildInfo.Depth; Col++)
        {
            SmoothedMap[Row * _BuildInfo.Depth + Col] = Average(Row, Col, _BuildInfo);
        }
    }

    m_fHeightMap = SmoothedMap;
}

glm::vec3 CHeightMap::CalculateColor(float height)
{
    // Example color mapping based on height
    if (height < 20.0f)
        return glm::vec3(0.0f, 0.0f, 1.0f); // Blue
    else if (height < 40.0f)
        return glm::vec3(0.0f, 1.0f, 0.0f); // Green 
    else if (height < 60.0f)
        return glm::vec3(0.5f, 0.5f, 0.0f); // Brown 
    else
        return glm::vec3(1.0f, 1.0f, 1.0f); // White 
}
