// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) Media Design School
//
// File Name : CQuad.h
// Description : orthographic quad to render the perlin noise png 
// Author : Daniel West
// Mail : daniel.west@mds.ac.nz

#include "CCamera.h"

class CQuad 
{
public:

    /// <summary>
    /// constructor. Creates vertices based on parameters passed in. Creates VAO, EBO and VBO
    /// </summary>
    /// <param name="_x"></param>
    /// <param name="_y"></param>
    /// <param name="_width"></param>
    /// <param name="_height"></param>
    /// <param name="_texture"></param>
    /// <param name="_program"></param>
    CQuad(float _x, float _y, float _width, float _height, GLuint _texture, GLuint _program);

    ~CQuad();

    /// <summary>
    /// Passes texture and orthographic projection matrix into the shader via uniform. Binds VAO
    /// </summary>
    /// <param name="camera"></param>
    virtual void Render(CCamera& camera);

    /// <summary>
    /// Sets the texture member variable to a new value passed through the parameter
    /// </summary>
    /// <param name="_texture"></param>
    void UpdateTexture(GLuint _texture);

    /// <summary>
    /// updates the quad position based on value passed through the parameter
    /// </summary>
    /// <param name="_newX"></param>
    /// <param name="_newY"></param>
    void SetPosition(float _newX, float _newY);

protected:
    GLuint VAO, VBO, EBO, m_texture, m_program;
    float x, y, m_fWidth, m_fHeight;
    //void LoadTexture(const char* _texturePath);

    /// <summary>
    /// Binds the VBO when vertices are updated
    /// </summary>
    void UpdateVertices();
};
