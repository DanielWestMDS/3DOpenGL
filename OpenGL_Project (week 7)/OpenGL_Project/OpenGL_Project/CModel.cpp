#include "CModel.h"

#include "Utils.h"




CModel::CModel(std::string FilePath, GLint _program, GLint _texture, glm::vec3 _position)
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
                if (TinyObjVertex.texcoord_index >= 0) // Negative states no TexCoord data
                {
                    Vertex.texcoord = {
                        Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 0],
                        Attrib.texcoords[2 * size_t(TinyObjVertex.texcoord_index) + 1],
                    };
                }
                if (TinyObjVertex.normal_index >= 0) // Negative states no Normal data
                {
                    Vertex.normal = {
                    Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 0],
                    Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 1],
                    Attrib.normals[3 * size_t(TinyObjVertex.normal_index) + 2],
                    };
                }
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
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStandard), (void*)(offsetof(VertexStandard, VertexStandard::normal)));
    glEnableVertexAttribArray(2);

    // instanced VBO
    glGenBuffers(1, &InstanceBuffer);

    // Unbind the VAO to avoid accidental modifications
    glBindVertexArray(0);

    // get an array of just vertices positions
    std::vector<glm::vec3> VertPositions;

    for (auto VertPos : Vertices)
    {
        VertPositions.push_back(VertPos.position);
    }

    // set values
    m_LocalAABB = ComputeLocalAABB(VertPositions);

    m_sFilePath = FilePath;
    m_program = _program;
    m_texture = _texture;
    m_Position = _position;
    m_fScale = 1.0f;
    m_fRotationAngle = 1.0f;
    m_RotationAxis = glm::vec3(1.0f, 1.0f, 1.0f);
    SetModelMat(MakeModelMatrix(m_Position, m_fScale, m_fRotationAngle, m_RotationAxis));;
    m_fShininess = 0.5f;
}

CModel::~CModel()
{
    glDeleteBuffers(1, &InstanceBuffer);
}

void CModel::Update(glm::mat4 _projMat, glm::mat4 _viewMat, glm::vec3 _cameraPos)
{
    m_projMat = _projMat;
    m_viewMat = _viewMat;
    m_cameraPos = _cameraPos;
    // TODO: maybe make this not happen every frame for every single object?
    // making NaN
    SetModelMat(MakeModelMatrix(m_Position, m_fScale, m_fRotationAngle, m_RotationAxis));
}

void CModel::Render()
{
    // bind program and VAO
    glUseProgram(m_program);
    glBindVertexArray(VAO);

    // Model matrix
    GLint ModelMatrix = glGetUniformLocation(m_program, "ModelMat");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(m_modelMat));

    // pass camera position in via uniform
    GLint CameraPosLoc = glGetUniformLocation(m_program, "CameraPos");
    glUniform3fv(CameraPosLoc, 1, glm::value_ptr(m_cameraPos));

    // pass in view projection
    GLint VPMat = glGetUniformLocation(m_program, "VP");
    glUniformMatrix4fv(VPMat, 1, GL_FALSE, glm::value_ptr(m_projMat * m_viewMat));

    // pass shadow texture to shader
    GLint ShadowTexture = glGetUniformLocation(m_program, "Texture_ShadowMap");
    glUniform1i(ShadowTexture, 5);
    glActiveTexture(GL_TEXTURE0 + 5);
    glBindTexture(GL_TEXTURE_2D, m_shadowTexture);

    // render
    glDrawArrays(DrawType, 0, DrawCount);

    glBindVertexArray(0);
}

void CModel::RenderShadow(GLuint _ShadowProgram, glm::mat4 _LightVP)
{
    glUseProgram(_ShadowProgram);

    // Model matrix
    GLint ModelMatrix = glGetUniformLocation(_ShadowProgram, "ModelMatrix");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(m_modelMat));

    // pass in view projection
    GLint VPMat = glGetUniformLocation(_ShadowProgram, "LightVP");
    glUniformMatrix4fv(VPMat, 1, GL_FALSE, glm::value_ptr(_LightVP));

    glDrawArrays(DrawType, 0, DrawCount);

    glBindVertexArray(0);
}

void CModel::RenderGeometryInstanced(GLint _program, GLint _texture, std::vector<glm::vec3> _instancePositions, glm::mat4 _modelMat, glm::vec3 _cameraPos, glm::mat4 _VP)
{
    // bind program and VAO
    glUseProgram(_program);
    glBindVertexArray(VAO);

    // Model matrix
    GLint ModelMatrix = glGetUniformLocation(_program, "ModelMat");
    glUniformMatrix4fv(ModelMatrix, 1, GL_FALSE, glm::value_ptr(_modelMat));

    // Activate and bind the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, _texture);
    glUniform1i(glGetUniformLocation(_program, "Texture0"), 0);

    // pass camera position in via uniform
    GLint CameraPosLoc = glGetUniformLocation(_program, "CameraPos");
    glUniform3fv(CameraPosLoc, 1, glm::value_ptr(_cameraPos));

    // pass in view projection
    GLint VPMat = glGetUniformLocation(_program, "VP");
    glUniformMatrix4fv(VPMat, 1, GL_FALSE, glm::value_ptr(_VP));

    // Bind and fill the instance buffer
    glBindBuffer(GL_ARRAY_BUFFER, InstanceBuffer);
    glBufferData(GL_ARRAY_BUFFER, _instancePositions.size() * sizeof(glm::vec3), _instancePositions.data(), GL_DYNAMIC_DRAW);

    // Set the instancePosition attribute (location 3)
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(3);
    // glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    // glVertexAttribDivisor(3, 1); // Divisor for instancing

     // render
    glDrawArraysInstanced(DrawType, 0, DrawCount, (GLsizei)_instancePositions.size());

    glBindVertexArray(0);
}

void CModel::SetMeshFilePath(std::string _path)
{
    m_sFilePath = _path;
}

void CModel::SetPosition(glm::vec3 _newPosition)
{
    m_Position = _newPosition;
}

void CModel::SetRotation(glm::vec3 _axis, float _amount)
{
    m_fRotationAngle = _amount;
    m_RotationAxis = _axis;
}

void CModel::SetScale(float _newScale)
{
    m_fScale = _newScale;
}

void CModel::SetProgram(GLint _program)
{
    m_program = _program;
}

void CModel::SetTexture(GLint _texture)
{
    m_texture = _texture;
}

glm::vec3 CModel::GetPosition()
{
    return m_Position;
}

glm::vec3 CModel::GetRotation()
{
    return m_RotationAxis;
}

std::string CModel::GetMeshFilePath()
{
    return m_sFilePath;
}

float CModel::GetScale()
{
    return m_fScale;
}

GLint CModel::GetProgram()
{
    return m_program;
}

GLint CModel::GetTexture()
{
    return m_texture;
}

AxisAlignedBoundingBox CModel::ComputeLocalAABB(const std::vector<glm::vec3>& vertices)
{
    glm::vec3 Min = vertices[0];
    glm::vec3 Max = vertices[0];

    for (const glm::vec3& v : vertices) 
    {
        Min = glm::min(Min, v);
        Max = glm::max(Max, v);
    }

    return AxisAlignedBoundingBox{ Min, Max };
}

AxisAlignedBoundingBox CModel::GetWorldAABB()
{
    // local model var
    glm::mat4 Model = GetModelMat();

    // 8 corners of the box
    glm::vec3 corners[8];
    corners[0] = glm::vec3(Model * glm::vec4(m_LocalAABB.Min.x, m_LocalAABB.Min.y, m_LocalAABB.Min.z, 1.0));
    corners[1] = glm::vec3(Model * glm::vec4(m_LocalAABB.Max.x, m_LocalAABB.Min.y, m_LocalAABB.Min.z, 1.0));
    corners[2] = glm::vec3(Model * glm::vec4(m_LocalAABB.Min.x, m_LocalAABB.Max.y, m_LocalAABB.Min.z, 1.0));
    corners[3] = glm::vec3(Model * glm::vec4(m_LocalAABB.Min.x, m_LocalAABB.Min.y, m_LocalAABB.Max.z, 1.0));
    corners[4] = glm::vec3(Model * glm::vec4(m_LocalAABB.Max.x, m_LocalAABB.Max.y, m_LocalAABB.Min.z, 1.0));
    corners[5] = glm::vec3(Model * glm::vec4(m_LocalAABB.Max.x, m_LocalAABB.Min.y, m_LocalAABB.Max.z, 1.0));
    corners[6] = glm::vec3(Model * glm::vec4(m_LocalAABB.Min.x, m_LocalAABB.Max.y, m_LocalAABB.Max.z, 1.0));
    corners[7] = glm::vec3(Model * glm::vec4(m_LocalAABB.Max.x, m_LocalAABB.Max.y, m_LocalAABB.Max.z, 1.0));

    // new min/max from transformed corners
    glm::vec3 newMin = corners[0];
    glm::vec3 newMax = corners[0];

    for (int i = 1; i < 8; ++i) 
    {
        newMin = glm::min(newMin, corners[i]);
        newMax = glm::max(newMax, corners[i]);
    }

    return AxisAlignedBoundingBox{ newMin, newMax };
}