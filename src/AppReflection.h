#ifndef _INCLUDE_APPREFLECTION_H_
#define _INCLUDE_APPREFLECTION_H_

#include <math/v3.h>

#include <common/BaseEntityGroup.h>
#include <common/StrongHandle.h>

namespace funk
{
	class AppReflection : public BaseEntityGroup
	{
	public:
		void Init();
		void Deinit();
		void Update( float dt );
		void Render();

	private:
		
		float	m_totalTime;
		v3		m_clearColor;
	};}

#endif