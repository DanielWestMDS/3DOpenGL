#include "CShadowMap.h"

CShadowMap::CShadowMap(int _windowWidth, int _windowHeight)
{
	// create the texture
	glGenTextures(1, &m_ShadowTexture);
	glBindTexture(GL_TEXTURE_2D, m_ShadowTexture);

	// data is initialised as null. Filled each frame
	// gl_depth_component used to store a single component
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, _windowWidth, _windowHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_BYTE, NULL);

	// mipmap linear
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	// create the framebuffer
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// set the texture as the framebuffer output
	glFramebufferTexture2D( GL_FRAMEBUFFER,			// target 
							GL_DEPTH_ATTACHMENT,	// attachment type
							GL_TEXTURE_2D,			// texture target type
							m_ShadowTexture,		// texture id
							0);						// mipmap level

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Error: framebuffer failed to initialize correctly" << std::endl;
	}

	glDrawBuffer(GL_NONE); // specifies there is no colour buffer drawn to
	glReadBuffer(GL_NONE); // specifies no colour buffer read from
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

CShadowMap::~CShadowMap()
{
}

void CShadowMap::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void CShadowMap::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}
