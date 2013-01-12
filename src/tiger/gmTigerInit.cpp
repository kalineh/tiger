#include "gmTigerInit.h"

#include <gm/gmMachine.h>
#include <gm/gmBind.h>

#include "World.h"

namespace tiger
{
	void InitGameLibs( gmMachine * a_machine )
	{
		GM_BIND_INIT( World, a_machine );
	}
}