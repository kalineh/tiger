#ifndef _INCLUDE_DEBUG_H_
#define _INCLUDE_DEBUG_H_

#include <assert.h>

#define ASSERT(A) assert(A)


#ifndef _DEBUG
#define CHECK(a,...) 0
#else
	void CHECK( bool cond, const char * format = 0, ... );
#endif // _DEBUG


#ifndef _DEBUG
#define WARN(a,...) 0
#else
	void WARN( bool cond, const char * format = 0, ... );
#endif // _DEBUG


void MESSAGE_BOX(const char * title, const char * format = 0, ... );


#endif