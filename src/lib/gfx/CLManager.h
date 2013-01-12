#ifndef _INCLUDE_CL_MANAGER_H_
#define _INCLUDE_CL_MANAGER_H_

#include <oclUtils.h>
#include <common/Singleton.h>
#include <vector>

namespace funk
{
	class CLManager : public Singleton<CLManager>
	{
	public:
		
		cl_context&			GetContext() { return m_clContextId; }
		cl_device_id &		GetDeviceId() { return m_clDevice; }

		// cmd queue
		cl_command_queue&	GetCmdQueue( int index = 0 ) { return m_clCmdQueues[index]; }
		void WaitFinishCmdQueue( int index = 0 );

	private:

		cl_platform_id		m_clPlatform;      // OpenCL platform
		cl_device_id		m_clDevice;          // OpenCL device
		cl_context			m_clContextId;        // OpenCL context

		std::vector<cl_command_queue> m_clCmdQueues;

		friend Singleton<CLManager>;
		CLManager();
		~CLManager();
	};
}

#endif