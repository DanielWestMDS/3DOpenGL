#include "CCamera.h"

class CQuad 
{
public:
    CQuad(float _x, float _y, float _width, float _height, GLuint _texture, GLuint _program);

    ~CQuad();

    virtual void Render(CCamera& camera);

    void RenderFrameBuffer();

    void UpdateTexture(GLuint _texture);

    void SetPosition(float _newX, float _newY);

protected:
    GLuint VAO, VBO, EBO, m_texture, m_program;
    float x, y, m_fWidth, m_fHeight;
    //void LoadTexture(const char* _texturePath);

    void UpdateVertices();
};
