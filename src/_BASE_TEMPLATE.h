#ifndef _INCLUDE_TEMPLATE_H
#define _INCLUDE_TEMPLATE_H

#include "BaseEntity.h"
#include "Gui.h"
#include "StrongPtr.h"
#include "CamDemo.h"

namespace e3d
{
	class TEMPLATE : public BaseEntity
	{
	public:
		TEMPLATE();
		~TEMPLATE();

		void Init();
		void Deinit();
		void Update( float dt );
		void Render();

	private:
		StrongPtr<CamDemo>	m_cam;
		StrongPtr<Gui> 		m_gui;
		
		float m_totalTime;
	};
}
#endif