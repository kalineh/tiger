#include <gm/gmStreamBuffer.h>
#include <gm/gmMachine.h>
#include <gm/gmStreamBuffer.h>

#include <common/Util.h>
#include <fstream>

int main( int argc, char *argv[] )
{
	gmMachine gm;

	if ( argc < 3 )
	{
		printf("Error, Please use: gmByteCoder.exe [SRC] [DEST]\n");
		return 1;
	}

	char * fileSrc = argv[1];
	char * fileDst = argv[2];

	// output lib
	gmStreamBufferDynamic writeBuffer;

	// load soure code
	char * srcCode = TextFileRead(fileSrc);
	if ( !srcCode ) 
	{
		printf("Cannot find file: %s\n", fileSrc );
		return 1;
	}

	gm.CompileStringToLib( srcCode, writeBuffer );

	FILE * fh;
	fh = std::fopen(fileDst, "wb");

	// check error
	if ( !fh )
	{
		printf("Cannot open destinatin file: %s\n", fileDst );
		delete[] srcCode;
		return 1;
	}

	fwrite( writeBuffer.GetData(), writeBuffer.GetSize(), 1, fh );
	fclose(fh);

	delete[] srcCode;

	return 0;
}

