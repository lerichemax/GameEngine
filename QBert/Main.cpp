#include "PCH.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "MainGame.h"

void StartHeapControl();
void DumpMemoryLeaks();

int main()
{
	StartHeapControl();
	MainGame engine;
	engine.Run();
	DumpMemoryLeaks();
	return 0;
}

void StartHeapControl()
{
#if defined(DEBUG) | defined(_DEBUG)
	// Notify user if heap is corrupt
	HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

	// Report detected leaks when the program exits
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Set a breakpoint on the specified object allocation order number
	//_CrtSetBreakAlloc(2442);
	
#endif
}

void DumpMemoryLeaks()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtDumpMemoryLeaks();
#endif
}