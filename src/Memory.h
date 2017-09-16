#pragma once

#include <Windows.h>
#include <string>

// Memory utils
// Purpose: Provides memeory related functionality (pattern scanning ect)
namespace Memory
{
	// Waits for a module to be available, before returning it's base address
	DWORD WaitOnModuleHandle(std::string moduleName);

	// Waits for a pointer to point to a non-zero value
	void WaitOnValidPointer(void* Pointer);

	// Attempts to locate the given signature and mask in a memory range
	// Returns the address at which it has been found
	DWORD FindPattern(std::string moduleName, BYTE* Mask, char* szMask);

	// Attempts to locate the given text in a memory range
	// Returns the address at which it has been found
	DWORD FindTextPattern(std::string moduleName, char* string);

	class VMTManager
	{
	private:
		DWORD* OriginalTable;
		DWORD* CustomTable;
		DWORD* Instance;

		int MethodCount();
	public:
		~VMTManager();

		bool	Initialise(DWORD* InstancePointer);
		DWORD	HookMethod(DWORD NewFunction, int Index);
		DWORD	GetOriginalFunction(int Index);
		void	Restore();
	};
};