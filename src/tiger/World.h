#ifndef _INCLUDE_TIGER_WORLD_H_
#define _INCLUDE_TIGER_WORLD_H_

#include <vector>

#include <gm/gmTableObject.h>
#include <gm/gmBindHeader.h>
#include <common/HandledObj.h>
#include <common/StrongHandle.h>
#include <math/v2i.h>
#include <gfx/Vbo.h>


namespace tiger
{
	struct EntityData
	{
		int type;
		gmTableObject* gmobj;
	};

	class World : public funk::HandledObj<World>
	{
	public:
		World();

		GM_BIND_TYPEID(World);

	private:
	};

	GM_BIND_DECL(World);
}

#endif