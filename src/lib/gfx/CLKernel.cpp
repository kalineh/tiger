#include "CLKernel.h"

#include <gfx/CLManager.h>
#include <common/Util.h>

#define CHECK_CL(x) if((x)!=CL_SUCCESS) { printf("CL Failed: %s", #x); assert(false); }

namespace funk
{
CLKernel::CLKernel( const char * filename, const char* kernelName ): 
	m_filename(filename), 
	m_kernelName(kernelName), 
	m_clKernel(0), 
	m_clProgram(0)
{
	ASSERT(filename);
	ASSERT(kernelName);

	Init();
}

CLKernel::~CLKernel()
{
	Deinit();	
}

void CLKernel::Reload()
{
	Init();
}

void CLKernel::Init()
{
	Deinit();

	cl_context & ctx = CLManager::Get()->GetContext();
	const cl_device_id *device = &(CLManager::Get()->GetDeviceId());

	cl_int ciErrNum;	// Error code var

	printf("CL Compiling file '%s', kernel '%s'....\n", m_filename.c_str(), m_kernelName.c_str() );

	while (true)
	{
		int numBytes;          // Byte size of kernel code
		const char * cSourceCL = TextFileRead( m_filename.c_str(), &numBytes );

		// Create the program
		size_t programSize = numBytes;
		m_clProgram = clCreateProgramWithSource(ctx, 1, (const char **)&cSourceCL, &programSize, &ciErrNum);
		CHECK_CL(ciErrNum);

		delete[] cSourceCL;

		// build program
		ciErrNum = clBuildProgram(m_clProgram, 1, device, NULL, NULL, NULL);
		if (ciErrNum != CL_SUCCESS)
		{
			size_t len;
			char buffer[4096];
			clGetProgramBuildInfo(m_clProgram, CLManager::Get()->GetDeviceId(), CL_PROGRAM_BUILD_LOG, sizeof(buffer), buffer, &len);
			MESSAGE_BOX("CL Compile Error", "CL '%s'\nCompile Error: %s", m_filename.c_str(), buffer );
		}
		else
		{
			printf("CL Compiled file '%s', kernel '%s' successfully!\n", m_filename.c_str(), m_kernelName.c_str() );
			// compiled successfully, quit!
			break;
		}
	}

	// create kernel
	m_clKernel = clCreateKernel(m_clProgram, m_kernelName.c_str(), &ciErrNum);
	CHECK_CL(ciErrNum);
}

void CLKernel::Deinit()
{
	if(m_clKernel)	clReleaseKernel(m_clKernel);
	if(m_clProgram)	clReleaseProgram(m_clProgram);

	m_clKernel = 0;
	m_clProgram = 0;
}

void CLKernel::Dispatch1D( int cmdQueueIndex, size_t globalWorkSize, size_t localWorkSize, size_t globalWorkOffset )
{	
	// handle local work size
	size_t * localWorkSizePtr = NULL;
	if ( localWorkSize > 0 ) localWorkSizePtr = &localWorkSize;

	// run it!
	cl_command_queue cmdQueue = CLManager::Get()->GetCmdQueue(cmdQueueIndex);
	cl_int ciErrNum = clEnqueueNDRangeKernel( cmdQueue, m_clKernel, 1, &globalWorkOffset, &globalWorkSize, localWorkSizePtr, 0, 0, 0 );
	CHECK_CL(ciErrNum);
}

void CLKernel::DispatchND( int cmdQueueIndex, size_t dimens, const size_t *globalWorkSize, const size_t *localWorkSize, const size_t * globalWorkOffset )
{
	cl_int ciErrNum;
	ciErrNum = clEnqueueNDRangeKernel( CLManager::Get()->GetCmdQueue(cmdQueueIndex), m_clKernel, dimens, globalWorkOffset, globalWorkSize, localWorkSize, 0, 0, 0 );
	CHECK_CL(ciErrNum);
}

}

#undef CHECK_CL