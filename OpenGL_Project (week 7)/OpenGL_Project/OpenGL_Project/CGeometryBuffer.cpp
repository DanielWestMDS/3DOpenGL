#include "CGeometryBuffer.h"
#include <iostream>

CGeometryBuffer::CGeometryBuffer()
{
	/* --- texture position ---*/

	// generate and bind frame buffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// generate bind and create texture
	glGenTextures(1, &m_TexturePosition);
	glBindTexture(GL_TEXTURE_2D, m_TexturePosition);

	// data gets filled each frame
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);

	// texture should match screen size
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	glFramebufferTexture2D(GL_FRAMEBUFFER,			// target 
		GL_COLOR_ATTACHMENT0,	// attachment type
		GL_TEXTURE_2D,			// texture target type
		m_TexturePosition,		// texture id
		0);						// mipmap level

	/* --- texture normal ---*/
		// generate and bind frame buffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// generate bind and create texture
	glGenTextures(1, &m_TextureNormal);
	glBindTexture(GL_TEXTURE_2D, m_TextureNormal);

	// data gets filled each frame
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);

	// texture should match screen size
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	glFramebufferTexture2D(GL_FRAMEBUFFER,			// target 
		GL_COLOR_ATTACHMENT1,	// attachment type
		GL_TEXTURE_2D,			// texture target type
		m_TextureNormal,		// texture id
		0);						// mipmap level

	/* --- texture albedo shininess ---*/
		// generate and bind frame buffer
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// generate bind and create texture
	glGenTextures(1, &m_TextureAlbedoShininess);
	glBindTexture(GL_TEXTURE_2D, m_TextureAlbedoShininess);

	// data gets filled each frame
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);

	// texture should match screen size
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	glFramebufferTexture2D(GL_FRAMEBUFFER,			// target 
		GL_COLOR_ATTACHMENT2,	// attachment type
		GL_TEXTURE_2D,			// texture target type
		m_TextureAlbedoShininess,		// texture id
		0);						// mipmap level

	// define the color buffers as output targets from the fragment shader
	GLuint Attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, };
	glDrawBuffers(3, Attachments);

	// depth texture
	glGenTextures(1, &m_TextureDepth);
	glBindTexture(GL_TEXTURE_2D, m_TextureDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 00, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
	glTextureParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_TextureDepth, 0);

	//GLuint RBO;
	//glGenRenderbuffers(1, &RBO);
	//glBindRenderbuffer(GL_RENDERBUFFER, RBO);

	//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 800);
	//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

	// check frame buffer was completed correctly
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer error: geometry buffer failed to initialise correctly. " << std::endl;
	}

	//Unbind 
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CGeometryBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void CGeometryBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
