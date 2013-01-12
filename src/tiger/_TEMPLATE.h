#ifndef _INCLUDE_STELLA_TEMPLATE_H_
#define _INCLUDE_STELLA_TEMPLATE_H_

#include <gm/gmBindHeader.h>
#include <common/HandledObj.h>

namespace stella
{
	class TEMPLATE : public funk::HandledObj<TEMPLATE>
	{
	public:
		TEMPLATE();
		~TEMPLATE();

		GM_BIND_TYPEID(TEMPLATE);

	private:
		
	};

	GM_BIND_DECL( TEMPLATE );
}

#endif