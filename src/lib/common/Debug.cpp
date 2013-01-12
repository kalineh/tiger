#include "Debug.h"

#ifdef _WIN32
#include <Windows.h>
#endif

#include <stdio.h>
#include <stdarg.h>

#ifdef _DEBUG

void CHECK( bool cond, const char * format, ... )
{
	if ( cond ) return;

	if ( format )
	{
		char buffer[256];
		va_list args;
		va_start (args, format);
		vsnprintf_s(buffer, 255, 255, format, args);
		va_end (args);

		#ifdef _WIN32
		MessageBoxA(NULL, buffer, "Error", MB_OK );
		#endif
		
	}
	
	ASSERT(false);
}

void WARN( bool cond, const char * format, ... )
{
	if ( cond ) return;

	if ( format )
	{
		char buffer[256];
		va_list args;
		va_start (args, format);
		vsnprintf_s(buffer, 255, 255, format, args);
		va_end (args);

		printf("WARNING: %s\n", buffer );
	}
}

#endif

void MESSAGE_BOX(const char * title, const char * format, ...)
{
	if ( format )
	{
		char buffer[4096];
		va_list args;
		va_start (args, format);
		vsnprintf_s(buffer, sizeof(buffer), sizeof(buffer), format, args);
		va_end (args);

		#ifdef _WIN32
			MessageBoxA(NULL, buffer, title, MB_OK );
		#else
		printf("MESSAGE BOX (%s): %s\n", title, buffer );
		#endif
	}
}