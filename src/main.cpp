#include "Core.h"

// NOTE: you need these settings to run:
// Properties->Debugging->Working Directory: $(ProjectDir)..\..\resources\
// Properties->Debugging->Environment: PATH=$(SolutionDir)../resources/;($Path)

int main(int argc, char **argv)
{
	funk::Core app;

	app.HandleArgs(argc,argv);
	app.Init();
	app.Run();
	app.Deinit();

	return 0;
}