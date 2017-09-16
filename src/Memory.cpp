#include "Memory.h"
#include <Psapi.h>

DWORD Memory::WaitOnModuleHandle(std::string moduleName)
{
	DWORD ModuleHandle = NULL;
	while (!ModuleHandle)
	{
		ModuleHandle = (DWORD)GetModuleHandle(moduleName.c_str());
		if (!ModuleHandle)
			Sleep(100);
	}

	return ModuleHandle;
}

// God knows who made these ancient peices of shit, but they get the job done
bool bCompare(const BYTE* Data, const BYTE* Mask, const char* szMask)
{
	for (; *szMask; ++szMask, ++Mask, ++Data)
	{
		if (*szMask == 'x' && *Mask != *Data)
		{
			return false;
		}
	}
	return (*szMask) == 0;
}

DWORD Memory::FindPattern(std::string moduleName, BYTE* Mask, char* szMask)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), Mask, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

DWORD Memory::FindTextPattern(std::string moduleName, char* string)
{
	DWORD Address = WaitOnModuleHandle(moduleName.c_str());
	MODULEINFO ModInfo; GetModuleInformation(GetCurrentProcess(), (HMODULE)Address, &ModInfo, sizeof(MODULEINFO));
	DWORD Length = ModInfo.SizeOfImage;

	// GHETTO AS FUCK LOL
	int len = strlen(string);
	char* szMask = new char[len + 1];
	for (int i = 0; i < len; i++)
	{
		szMask[i] = 'x';
	}
	szMask[len] = '\0';

	for (DWORD c = 0; c < Length; c += 1)
	{
		if (bCompare((BYTE*)(Address + c), (BYTE*)string, szMask))
		{
			return (DWORD)(Address + c);
		}
	}
	return 0;
}

int Memory::VMTManager::MethodCount()
{
	size_t Index = 0;

	while (!IsBadCodePtr((FARPROC)OriginalTable[Index]))
	{
		if (!IsBadCodePtr((FARPROC)OriginalTable[Index]))
		{
			Index++;
		}
	}
	return Index;
}

bool Memory::VMTManager::Initialise(DWORD* InstancePointer)
{
	Instance = InstancePointer;
	OriginalTable = (DWORD*)*InstancePointer;

	size_t Index = MethodCount() * 4;

	CustomTable = new DWORD[Index];
	if (!CustomTable) return false;
	memcpy((void*)CustomTable, (void*)OriginalTable, Index);

	*InstancePointer = (DWORD)CustomTable;
	return true;
}

Memory::VMTManager::~VMTManager()
{
	if (CustomTable)
	{
		free(CustomTable);
	}
}

DWORD	Memory::VMTManager::HookMethod(DWORD NewFunction, int Index)
{
	if (CustomTable)
	{
		CustomTable[Index] = NewFunction;
		return OriginalTable[Index];
	}
	else
		return NULL;
}

DWORD	Memory::VMTManager::GetOriginalFunction(int Index)
{
	return OriginalTable[Index];
}

void	Memory::VMTManager::Restore()
{
	*Instance = (DWORD)OriginalTable;
}