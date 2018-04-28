#include "Include.h"
#include "ABClient.h"
#include "Util.h"
using namespace std;
using namespace asmjs;

ABClient abc;

namespace Process
{
	string Name = "TslGame.exe";
	string Module = "TslGame.exe";
}

void main()
{
	try
	{
		// bypass test

		printf("Testing...\n");
		printf("Connecting to server..\n");
		abc.Connect();
		printf("Connected. Pinging...\n");
		abc.Ping();
		abc.AccuireProcessHandle(Util::GetProcessIdsByName(Process::Name)[0], 0x1478 /* LSASS set */);
		auto base = abc.GetProcessModuleBase(Process::Module);								// getting module base
		printf("Module base: 0x%X\n", base);
		char textAtBase[3] = "  ";													// zero terminating the buffer
		printf("Reading text at base\n");
		abc.RpmRaw(base, textAtBase, 2);											// reading two characters from processbase (should be MZ)
		printf("TextAtBase: %s\n", textAtBase);										// displaying them
		auto oldProtection = abc.VirtualProtect(base, 2, PAGE_EXECUTE_READWRITE);	// unprotecting process base
		printf("OldProtection: 0x%X\n", oldProtection);								// displaying old protection						// writing a new value to module base
		abc.RpmRaw(base, textAtBase, 2);											// reading the process base to see if we wrote memory correctly last time
		printf("Reading text at base: %s\n", textAtBase);							// displaying em
		printf("Allocating some data..\n");								
		auto p = abc.VirtualAlloc(null, 0x1000000, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE); // allocating some memory
		printf("Allocated region: 0x%X", (unsigned int)p);
	}
	catch (exception e)
	{
		printf("Exception occured: %s (0x%X)\n", e.what(), GetLastError());
	}

	getchar();
	getchar();
	getchar();
	getchar();
}