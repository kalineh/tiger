#ifndef _INCLUDE_CL_KERNEL_H_
#define _INCLUDE_CL_KERNEL_H_

#include <common/Debug.h>
#include <common/HandledObj.h>

#include <oclUtils.h>
#include <string>

namespace funk
{
	class CLKernel : public HandledObj<CLKernel>
	{
	public:

		CLKernel( const char * filename, const char* kernelName );
		~CLKernel();

		cl_kernel& Get() { return m_clKernel; }

		void Reload();
		void Dispatch1D( int cmdQueueIndex, size_t globalWorkSize, size_t localWorkSize = 0, size_t globalWorkOffset = 0 );
		void DispatchND( int cmdQueueIndex, size_t dimens, const size_t *globalWorkSize, const size_t *localWorkSize, const size_t * globalWorkOffset );

		// set argument
		template<class T> 
		void SetArg( int arg, const T& value )
		{
			cl_int ciErr = clSetKernelArg( m_clKernel, arg, sizeof(T), (void*)&value );
			CHECK( ciErr == CL_SUCCESS );
		}

	private:

		void Init();
		void Deinit();

		std::string m_filename;
		std::string m_kernelName;

		cl_program m_clProgram;
		cl_kernel m_clKernel;

	};
}

#endif