#include "AppReflection.h"

#include <math/v2.h>
#include <gl/glew.h>

#include <gfx/TextureManager.h>
#include <common/Window.h>
#include <gfx/DrawPrimitives.h>
#include <common/Debug.h>
#include <math/Util.h>

//#include "CircuitBoard.h"

namespace funk
{

void AppReflection::Init()
{
	//AddChild( new CircuitBoard );
	BaseEntityGroup::Init();

	m_totalTime = 0.0f;

	glDisable( GL_TEXTURE_2D );
	//glDisable( GL_CULL_FACE );
	glDisable( GL_DEPTH_TEST );
	//glEnable( GL_POINT_SMOOTH );

	m_clearColor = v3( 0.185f, 0.51f, 0.475f );
}

void AppReflection::Deinit()
{
	BaseEntityGroup::Deinit();
}

void AppReflection::Update( float dt )
{
	BaseEntityGroup::Update(dt);
	m_totalTime += dt;

	//GLenum errCode =  glGetError();
	//CHECK( errCode == GL_NO_ERROR, (char*)glewGetErrorString( errCode ) );
}

void AppReflection::Render()
{
	glClearColor( m_clearColor.x, m_clearColor.y, m_clearColor.z, 1.0f );
	glClearDepth(1.0);
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	BaseEntityGroup::Render();
}

}