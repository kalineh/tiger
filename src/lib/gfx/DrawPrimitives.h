#ifndef _INCLUDE_DRAWPRIMITIVES_H_
#define _INCLUDE_DRAWPRIMITIVES_H_

#include <math/v2.h>
#include <math/v3.h>

#include <common/StrongHandle.h>
#include "Texture.h"
#include "Fbo.h"

namespace funk
{
	const float PI = 3.141592f;

	void SetColor( const v3 color, float alpha = 1.0f );

	void DrawLine( const v2 start, const v2 end );
	void DrawLine( const v3 start, const v3 end );
	void DrawSpline( const v2 v0, const v2 tan0, const v2 v1, const v2 tan1, const float t0 = 0.0f, const float t1 = 1.0f, int numSteps = 64 );
	void DrawSpline( const v3 v0, const v3 tan0, const v3 v1, const v3 tan1, const float t0 = 0.0f, const float t1 = 1.0f, int numSteps = 64 );
	
	void DrawTriangle(  const v2 v0, const v2 v1, const v2 v2 );
	void DrawTriangle(  const v3 v0, const v3 v1, const v3 v2 );
	void DrawTriangleWire(  const v2 v0, const v2 v1, const v2 v2 );
	void DrawTriangleWire(  const v3 v0, const v3 v1, const v3 v2 );

	void DrawRect( const v2 bottomLeft, const v2 dimen );
	void DrawRectRounded( const v2 bottomLeft, const v2 dimen, float cornerRadius );
	void DrawRectRoundedWire( const v2 bottomLeft, const v2 dimen, float cornerRadius );
	void DrawRectRoundedTop( const v2 bottomLeft, const v2 dimen, float cornerRadius );
	void DrawRectRoundedTopWire( const v2 bottomLeft, const v2 dimen, float cornerRadius );
	void DrawRectRoundedBot( const v2 bottomLeft, const v2 dimen, float cornerRadius );
	void DrawRectRoundedBotWire( const v2 bottomLeft, const v2 dimen, float cornerRadius );
	void DrawRectWire( const v2 bottomLeft, const v2 dimen );
	void DrawRectTexCoords( const v2 bottomLeft, const v2 dimen, v2 uv0 = v2(0.0f), v2 uv1=v2(1.0f) );
	void DrawTex2D( const v2 topLeft, const v2 dimen, StrongHandle<Texture> tex );

	void DrawCircle( const v2 center, float radius, int segments = 32 );
	void DrawCircleWire( const v2 center, float radius, int segments = 32 );
	void DrawDonut( const v2 center, float radius, float thickness, int segments = 32 );
	void DrawArcDonut( const v2 center, float radius, float thickness, float startAngle, float endAngle, int segments = 32 );
	void DrawArcLine( const v2 center, float radius, float startAngle, float endAngle, int segments = 32 );

	// 3d
	void DrawCube( const v3 dimen, const v3 center = v3(0.0f) );
	void DrawCubeWire( const v3 dimen, const v3 center = v3(0.0f) );
	void DrawMayaPlane( int gridsWidth = 10 );
	void DrawBasisVectors();
	void DrawArrow( const v3 start, const v3 end );
	void DrawSphere( const v3 center = v3(0.0f), float radius = 1.0f );
	void DrawDisk( const v3 center, const v3 dir, float radius, int numSteps = 32 );
	void DrawDiskWire( const v3 center, const v3 dir, float radius, int numSteps = 32 );

	void DrawScreenUV(  v2 topLeftScreenUV = v2(0,0),  // topleft
						v2 dimenScreenUV = v2(1,1) );
}

#endif