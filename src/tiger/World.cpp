#include "World.h"

#include <gm/gmBind.h>
#include <gm/gmArrayLib.h>
#include <vm/VirtualMachine.h>

#include <gfx/Font.h>
#include <imgui/ImguiManager.h>

namespace tiger
{

using namespace funk;

World::World()
{
}

GM_REG_NAMESPACE(World)
{
	GM_MEMFUNC_DECL(CreateWorld)
	{
		GM_CHECK_NUM_PARAMS(0);
		GM_PUSH_USER_HANDLED( World, new World() );
		return GM_OK;
	}

	//GM_GEN_MEMFUNC_VOID_VOID( World, DrawTilesDebug )
}

GM_REG_MEM_BEGIN(World)
//GM_REG_MEMFUNC( World, DrawTilesDebug )
GM_REG_HANDLED_DESTRUCTORS(World)
GM_REG_MEM_END()
GM_BIND_DEFINE(World)

}
