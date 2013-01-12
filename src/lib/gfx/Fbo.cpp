#include "Fbo.h"

#include <common/Debug.h>
#include <common/Window.h>

#include <gm/gmBind.h>
#include <gm/gmBindFuncGen.h>

#include "Texture.h"

#include <gl/glew.h>

namespace funk
{
Fbo::Fbo( int width, int height ) 
: m_dimen(width,height), m_fboId(0), 
m_fboDepthId(0),  m_boundAsTarget(false)
{
	m_rtTextures.resize(MAX_NUM_SLOTS);

	glGenFramebuffersEXT(1, &m_fboId); // Generate one frame buffer and store the ID in fbo
	ASSERT( m_fboId != 0 );

	GLint err = glGetError();
	CHECK( err == GL_NO_ERROR, "Creating FBO texture failed, GLerror: %d!", err );
}

Fbo::~Fbo()
{
	CHECK( !m_boundAsTarget, "Deleting FBO that is currently bound!" );

	if ( m_fboId ) glDeleteFramebuffers( 1, &m_fboId );
	if ( m_fboDepthId ) glDeleteRenderbuffers( 1, &m_fboDepthId );
}

void Fbo::GenerateDepthBuffer()
{
	glGenRenderbuffersEXT(1, &m_fboDepthId); // Generate one render buffer and store the ID in fbo_depth  
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, m_fboDepthId); // Bind the fbo_depth render buffer  
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, m_dimen.x, m_dimen.y); // Set the render buffer storage to be a depth component, with a width and height of the window
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0); // Unbind the render buffer 

	GLint err = glGetError();
	CHECK( err == GL_NO_ERROR, "Creating FBO depth buffer failed, GLerror: 0x%X!", err );

	// attach depth to fbo
	BindFrameBuffer();

	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_fboDepthId); // Attach the depth buffer fbo_depth to our frame buffer
	GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT); // Check that status of our generated frame buffer
	CHECK(status == GL_FRAMEBUFFER_COMPLETE_EXT, "Unable to create FBO!"); // If the frame buff
	
	UnbindFrameBuffer();
}

void Fbo::RemoveRenderTargetTex( int slot )
{
	ASSERT(slot >= 0 && slot < MAX_NUM_SLOTS);

	// unbind from fbo
	BindFrameBuffer();
	unsigned int GLcolorAttachmentSlot = GL_COLOR_ATTACHMENT0_EXT + slot;
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GLcolorAttachmentSlot, GL_TEXTURE_2D, 0, 0);
	UnbindFrameBuffer();

	m_rtTextures[slot] = StrongHandle<Texture>();
}

void Fbo::GenerateRenderTargetTex( int slot, TexParams texParams )
{
	ASSERT(slot >= 0 && slot < MAX_NUM_SLOTS);

	StrongHandle<Texture> tex = new Texture(m_dimen.x, m_dimen.y, texParams);
	SetRenderTargetTex(tex, slot);
}

void Fbo::SetRenderTargetTex( StrongHandle<Texture> tex, int slot )
{
	CHECK( tex != NULL, "FBO trying to set NULL texure as color buffer!");
	ASSERT(slot >= 0 && slot < MAX_NUM_SLOTS);
	CHECK( tex->Sizei() == Dimen(), "Texture dimensions must match FBO dimen" );

	m_rtTextures[slot] = tex;

	CHECK( !m_boundAsTarget, "Cannot bind next texture. Fbo currently bound!");

	unsigned int GLcolorAttachmentSlot = GL_COLOR_ATTACHMENT0_EXT + slot;

	BindFrameBuffer();
	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GLcolorAttachmentSlot, GL_TEXTURE_2D, tex->Id(), 0);
	UnbindFrameBuffer();
}

void Fbo::BindFrameBuffer()
{
	CHECK( !m_boundAsTarget, "Fbo currently bound!");
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, m_fboId);
	m_boundAsTarget = true;
}

void Fbo::UnbindFrameBuffer()
{
	CHECK( m_boundAsTarget, "Fbo not bound!");
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	m_boundAsTarget = false;
}

void Fbo::Begin()
{
	BindFrameBuffer();

	// set viewport as FBO tex size
	glViewport( 0, 0, Width(), Height() );
}

void Fbo::End()
{
	UnbindFrameBuffer();

	// set viewport to screen
	v2i dimen =  Window::Get()->Sizei();
	glViewport( 0, 0, dimen.x, dimen.y );
}

GM_REG_NAMESPACE(Fbo)
{
	GM_MEMFUNC_CONSTRUCTOR(Fbo)
	{
		GM_CHECK_NUM_PARAMS(2);
		GM_CHECK_FLOAT_OR_INT_PARAM(width, 0);
		GM_CHECK_FLOAT_OR_INT_PARAM(height, 1);

		StrongHandle<Fbo> p = new Fbo((int)width, (int)height);
		GM_PUSH_USER_HANDLED( Fbo, p.Get() );
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GetTex)
	{
		GM_INT_PARAM( slot, 0, 0 );
		GM_GET_THIS_PTR(Fbo, ptr);

		// return texture if exists
		if ( ptr->IsSlotValid(slot) ) a_thread->PushNull();	
		else GM_PUSH_USER_HANDLED( Texture, ptr->GetTex(slot).Get() );

		return GM_OK;
	}

	GM_MEMFUNC_DECL(SetRenderTargetTex)
	{
		GM_CHECK_USER_PARAM_PTR( Texture, tex, 0 );
		GM_INT_PARAM( slot, 1, 0 );
		GM_GET_THIS_PTR(Fbo, ptr);

		ptr->SetRenderTargetTex(tex, slot);
		
		return GM_OK;
	}

	GM_MEMFUNC_DECL(GenerateRenderTargetTex)
	{
		// args
		GM_INT_PARAM( slot, 0, 0 );
		GM_INT_PARAM( internalFormat, 1, RGBA8 );
		GM_INT_PARAM( format, 2, RGBA );
		GM_INT_PARAM( dataType, 3, UNSIGNED_BYTE );

		// create texture and bind to slot
		GM_GET_THIS_PTR(Fbo, ptr);
		TexParams params = TexParams((GLInternalFormat)internalFormat, (GLFormat)format, (GLDataType)dataType);
		ptr->GenerateRenderTargetTex( slot, params );
		
		return GM_OK;
	}

	GM_GEN_MEMFUNC_VOID_VOID( Fbo, Begin )
	GM_GEN_MEMFUNC_VOID_VOID( Fbo, End )
	GM_GEN_MEMFUNC_INT_VOID( Fbo, Width )
	GM_GEN_MEMFUNC_INT_VOID( Fbo, Height)
	GM_GEN_MEMFUNC_VOID_INT( Fbo, RemoveRenderTargetTex )
	GM_GEN_MEMFUNC_VOID_VOID( Fbo, GenerateDepthBuffer )
}

GM_REG_MEM_BEGIN(Fbo)
GM_REG_MEMFUNC( Fbo, Begin )
GM_REG_MEMFUNC( Fbo, End )
GM_REG_MEMFUNC( Fbo, Width )
GM_REG_MEMFUNC( Fbo, Height )
GM_REG_MEMFUNC( Fbo, GetTex )
GM_REG_MEMFUNC( Fbo, SetRenderTargetTex )
GM_REG_MEMFUNC( Fbo, GenerateRenderTargetTex )
GM_REG_MEMFUNC( Fbo, GenerateDepthBuffer )
GM_REG_MEMFUNC( Fbo, RemoveRenderTargetTex )
GM_REG_HANDLED_DESTRUCTORS(Fbo)
GM_REG_MEM_END()
GM_BIND_DEFINE(Fbo)

}