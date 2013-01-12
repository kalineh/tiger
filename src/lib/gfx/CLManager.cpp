#include "CLManager.h"

#include <common/Debug.h>

#define CHECK_CL(x) if((x)!=CL_SUCCESS) { printf("CL Failed: %s", #x); ASSERT(false); }

namespace funk
{
CLManager::CLManager()
{
	cl_int ciErrNum;

	CHECK_CL( clGetPlatformIDs(1, &m_clPlatform, NULL) );
	CHECK_CL( clGetDeviceIDs( m_clPlatform, CL_DEVICE_TYPE_GPU, 1, &m_clDevice, NULL ) );

#if 1
	// create context
	cl_context_properties props[] = 
	{
		CL_GL_CONTEXT_KHR, 
		(cl_context_properties) wglGetCurrentContext(),  // HGLRC handle
		CL_WGL_HDC_KHR,
		(cl_context_properties) wglGetCurrentDC(),  // HDC handle
		CL_CONTEXT_PLATFORM, 
		(cl_context_properties)m_clPlatform, 0
	};
	m_clContextId = clCreateContext(props, 1, &m_clDevice, NULL, NULL, &ciErrNum);
#else
	// context
	m_clContextId = clCreateContext(0, 1, &m_clDevice, NULL, NULL, &ciErrNum);
#endif

	CHECK_CL(ciErrNum);

	// command queue
	int numCmdQueues = 8;
	for( int i = 0; i < numCmdQueues; ++i )
	{
		cl_command_queue cmdQ = clCreateCommandQueue( m_clContextId, m_clDevice, 0, &ciErrNum );
		CHECK_CL(ciErrNum);
		m_clCmdQueues.push_back(cmdQ);
	}
}

CLManager::~CLManager()
{
	for( size_t i = 0; i < m_clCmdQueues.size(); ++i )
	{
		clReleaseCommandQueue(m_clCmdQueues[i]);
	}
	m_clCmdQueues.clear();


	clReleaseContext(m_clContextId);;
}

void CLManager::WaitFinishCmdQueue( int index )
{
	ASSERT(index >= 0 && index < (int)m_clCmdQueues.size());

	cl_int ciErrNum = clFinish(m_clCmdQueues[index]);
	CHECK_CL(ciErrNum);
}

}

#undef CHECK_CL