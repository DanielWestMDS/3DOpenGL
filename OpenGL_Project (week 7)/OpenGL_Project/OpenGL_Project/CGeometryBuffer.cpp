#include "CGeometryBuffer.h"
#include <iostream>

CGeometryBuffer::CGeometryBuffer()
{
	/* --- texture position ---*/

	// Generate and bind the framebuffer only once
	glGenFramebuffers(1, &m_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

	// Generate, bind, and create the position texture
	glGenTextures(1, &m_TexturePosition);
	glBindTexture(GL_TEXTURE_2D, m_TexturePosition);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_TexturePosition, 0);

	/* --- texture normal --- */
	glGenTextures(1, &m_TextureNormal);
	glBindTexture(GL_TEXTURE_2D, m_TextureNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_TextureNormal, 0);

	/* --- texture albedo shininess --- */
	glGenTextures(1, &m_TextureAlbedoShininess);
	glBindTexture(GL_TEXTURE_2D, m_TextureAlbedoShininess);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, 800, 800, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_TextureAlbedoShininess, 0);

	// Define the color buffers as output targets from the fragment shader
	GLuint Attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, Attachments);

	// Depth texture
	glGenTextures(1, &m_TextureDepth);
	glBindTexture(GL_TEXTURE_2D, m_TextureDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 800, 800, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_TextureDepth, 0);

	// Check framebuffer completion
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cerr << "Framebuffer error: geometry buffer failed to initialize correctly." << std::endl;
	}

	// Unbind
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
