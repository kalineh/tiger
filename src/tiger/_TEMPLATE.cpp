#include "TEMPLATE.h"

#include <gm/gmBind.h>

namespace stella
{

using namespace funk;

TEMPLATE::TEMPLATE()
{
}

TEMPLATE::~TEMPLATE()
{
}

GM_REG_NAMESPACE(TEMPLATE)
{
	GM_MEMFUNC_CONSTRUCTOR(TEMPLATE)
	{
		//GM_CHECK_NUM_PARAMS(1);
		//GM_CHECK_INT_PARAM(numTEMPLATE, 0);

		GM_PUSH_USER_HANDLED( TEMPLATE, new TEMPLATE);
		return GM_OK;
	}

	//GM_GEN_MEMFUNC_VOID_FLOAT_FLOAT( TEMPLATE, DrawRange )

}
GM_REG_MEM_BEGIN(TEMPLATE)
//GM_REG_MEMFUNC( TEMPLATE, DrawRange )
GM_REG_HANDLED_DESTRUCTORS(TEMPLATE)
GM_REG_MEM_END()
GM_BIND_DEFINE(TEMPLATE)


}