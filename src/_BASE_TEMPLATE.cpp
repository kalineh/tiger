#include "TEMPLATE.h"

#include "GuiManager.h"

namespace e3d
{
TEMPLATE::TEMPLATE() :  m_totalTime(0)
{;}

TEMPLATE::~TEMPLATE()
{;}

void TEMPLATE::Init()
{
	m_gui = GuiManager::Get()->CreateGui("TEMPLATE");
	
	// cam
	m_cam = new CamDemo;
	m_cam->Init();
}

void TEMPLATE::Deinit()
{

}

void TEMPLATE::Update( float dt )
{
	m_totalTime += dt;
	m_cam->Update(dt);
}

void TEMPLATE::Render()
{

}
}
