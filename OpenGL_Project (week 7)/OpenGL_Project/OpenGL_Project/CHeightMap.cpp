// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CHeightMap.h
// Description : Uses a raw file and buildinfo to create a bunch of vertices at different heights which connect to create a landscape 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#include "CHeightMap.h"

CHeightMap::CHeightMap(HeightMapInfo& _BuildInfo, GLint _program, GLint _textureArray[4])
{
    m_program = _program;
    m_textureArray[0] = _textureArray[0];
    m_textureArray[1] = _textureArray[1];
    m_textureArray[2] = _textureArray[2];
    m_textureArray[3] = _textureArray[3];
    if (LoadHeightMap(_BuildInfo))
    {
        BuildVertexData(_BuildInfo);
        std::cout << "Height should have loaded successfully" << std::endl;
    }
    else
    {
        std::cout << "Height Map failed" << std::endl;
    }
    
    m_fHeightLevels[0] = 4;
    m_fHeightLevels[1] = 30;
    m_fHeightLevels[2] = 150;
    //m_fHeightLevels[3] = 300;
}

CHeightMap::~CHeightMap()
{
}

void CHeightMap::Render()
{
    glUseProgram(m_program);
    glBindVertexArray(m_VAO);

    // Set textures
    for (int i = 0; i < 4; i++)
    {
        std::string uniformName = "TextureArray[" + std::to_string(i) + "]";
        GLint location = glGetUniformLocation(m_program, uniformName.c_str());
        glUniform1i(location, i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textureArray[i]);
    }

    GLint ModelMatrix = glGetUniformLocation(m_program, "model");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(m_matrix));

    GLint ViewMatrix = glGetUniformLocation(m_program, "view");
    glUniformMatrix4fv(ViewMatrix, 1, GL_FALSE, glm::value_ptr(m_viewMat));

    GLint ProjMatrix = glGetUniformLocation(m_program, "projection");
    glUniformMatrix4fv(ProjMatrix, 1, GL_FALSE, glm::value_ptr(m_projMat));

    GLint CameraPosLoc = glGetUniformLocation(m_program, "CameraPos");
    glUniform3fv(CameraPosLoc, 1, glm::value_ptr(m_cameraPos));

    // Pass height levels to the shader
    GLint HeightLevelsLoc = glGetUniformLocation(m_program, "HeightLevels");
    glUniform1fv(HeightLevelsLoc, 3, m_fHeightLevels);

    // pass in view projection for shadows
    GLint VPMat = glGetUniformLocation(m_program, "VPLight");
    glUniformMatrix4fv(VPMat, 1, GL_FALSE, glm::value_ptr(m_lightVP));

    // pass shadow texture to shader
    GLint ShadowTexture = glGetUniformLocation(m_program, "Texture_ShadowMap");
    glActiveTexture(GL_TEXTURE0 + 4);
    glBindTexture(GL_TEXTURE_2D, m_ShadowTexture);
    glUniform1i(ShadowTexture, 5);

    // shadow texture

    glDrawElements(GL_TRIANGLES, m_DrawCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void CHeightMap::RenderShadow(GLuint _ShadowProgram)
{
    glUseProgram(_ShadowProgram);
    glBindVertexArray(m_VAO);

    // Model matrix
    GLint ModelMatrix = glGetUniformLocation(m_program, "ModelMatrix");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(m_matrix));

    // pass in view projection
    GLint VPMat = glGetUniformLocation(m_program, "LightVP");
    glUniformMatrix4fv(VPMat, 1, GL_FALSE, glm::value_ptr(m_lightVP));

    glDrawElements(GL_TRIANGLES, m_DrawCount, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

bool CHeightMap::LoadHeightMap(HeightMapInfo& _BuildInfo)
{
    unsigned int VertexCount = _BuildInfo.Width * _BuildInfo.Depth;
    std::vector<unsigned char> HeightValue(VertexCount);

    std::ifstream File;
    File.open(_BuildInfo.FilePath, std::ios_base::binary);
    if (File)
    {
        File.read((char*)&HeightValue[0], (std::streamsize)HeightValue.size());
        File.close();
    }
    else
    {
        std::cout << "HEIGHT MAP FAILED TO LOAD: " << _BuildInfo.FilePath << std::endl;
        return false;
    }

    m_fHeightMap.resize(VertexCount, 0);
    for (unsigned int i = 0; i < VertexCount; i++)
    {
        m_fHeightMap[i] = static_cast<float>(HeightValue[i]);
    }
    return true;
}

void CHeightMap::BuildVertexData(HeightMapInfo& _BuildInfo)
{
    // smoothen the height map
    SmoothHeights(_BuildInfo);
    SmoothHeights(_BuildInfo);
    SmoothHeights(_BuildInfo);
    SmoothHeights(_BuildInfo);
    unsigned int VertexCount = _BuildInfo.Width * _BuildInfo.Depth;
    std::vector<VertexStandardHeightMap> Vertices(VertexCount);

    float HalfWidth = (_BuildInfo.Width - 1) * _BuildInfo.CellSpacing * 0.5f;
    float HalfDepth = (_BuildInfo.Depth - 1) * _BuildInfo.CellSpacing * 0.5f;

    float TexU = 1.0f / (float)(_BuildInfo.Width - 1);
    float TexV = 1.0f / (float)(_BuildInfo.Depth - 1);

    for (unsigned int Row = 0; Row < _BuildInfo.Width; Row++)
    {
        float PosZ = HalfDepth - (Row * _BuildInfo.CellSpacing);

        for (unsigned int Col = 0; Col < _BuildInfo.Depth; Col++)
        {
            int Index = Row * _BuildInfo.Width + Col;

            float PosX = HalfWidth + (Col * _BuildInfo.CellSpacing);
            float PosY = m_fHeightMap[Index];

            Vertices[Index].Position = glm::vec3(PosX, PosY, PosZ);
            Vertices[Index].TexCoord = glm::vec2(Col * TexU, Row * TexV);
            Vertices[Index].Normal = glm::vec3(0.0f, 1.0f, 0.0f);
            Vertices[Index].Height = PosY;
        }
    }

    // Calculate normals using central difference
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

    m_DrawType = GL_TRIANGLES;
    unsigned int FaceCount = (_BuildInfo.Width - 1) * (_BuildInfo.Depth - 1) * 2;
    m_DrawCount = FaceCount * 3;
    std::vector<GLuint> Indices(m_DrawCount);

    int Index = 0;
    for (unsigned int Row = 0; Row < (_BuildInfo.Width - 1); Row++)
    {
        for (unsigned int Col = 0; Col < (_BuildInfo.Depth - 1); Col++)
        {
            Indices[Index++] = Row * _BuildInfo.Depth + Col;
            Indices[Index++] = Row * _BuildInfo.Depth + Col + 1;
            Indices[Index++] = (Row + 1) * _BuildInfo.Depth + Col;

            Indices[Index++] = (Row + 1) * _BuildInfo.Depth + Col;
            Indices[Index++] = Row * _BuildInfo.Depth + Col + 1;
            Indices[Index++] = (Row + 1) * _BuildInfo.Depth + Col + 1;
        }
    }

    GLuint VBO;
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexStandardHeightMap) * Vertices.size(), Vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * Indices.size(), Indices.data(), GL_STATIC_DRAW);

    // Attribute pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandardHeightMap), (void*)(offsetof(VertexStandardHeightMap, Position)));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStandardHeightMap), (void*)(offsetof(VertexStandardHeightMap, TexCoord)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandardHeightMap), (void*)(offsetof(VertexStandardHeightMap, Normal)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(VertexStandardHeightMap), (void*)(offsetof(VertexStandardHeightMap, Height)));
    glEnableVertexAttribArray(3);

    //glGenBuffers(1, &m_InstanceBuffer);
    glBindVertexArray(0);
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

void CHeightMap::Update(glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _cameraPos, glm::mat4 _matrix, glm::mat4 _LightVP, GLuint _ShadowTex)
{
    m_projMat = _projMat;
    m_viewMat = _viewMat;
    m_cameraPos = _cameraPos;
    m_matrix = _matrix;
    m_lightVP = _LightVP;
    m_ShadowTexture = _ShadowTex;
}

float CHeightMap::Average(unsigned int _row, unsigned int _col, HeightMapInfo& _BuildInfo)
{
    float Total = 0.0f;
    unsigned int Count = 0;

    for (int Row = -1; Row <= 1; ++Row)
    {
        for (int Col = -1; Col <= 1; ++Col)
        {
            int NeighborRow = _row + Row;
            int NeighborCol = _col + Col;

            if (NeighborRow >= 0 && NeighborRow < _BuildInfo.Width &&
                NeighborCol >= 0 && NeighborCol < _BuildInfo.Depth)
            {
                Total += m_fHeightMap[NeighborRow * _BuildInfo.Depth + NeighborCol];
                Count++;
            }
        }
    }

    return Total / Count;
}