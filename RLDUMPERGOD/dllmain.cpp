#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "UObject.hpp"
#include "UClass.hpp"

static uintptr_t BaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

GNames* GetFNamePool()
{
	// offset: 0x24BF490
	return *(GNames**)(BaseAddress + 0x24BF490);
}

std::wstring UObject::GetName()
{
	GNames* FNamePool = GetFNamePool();
	return std::wstring(FNamePool->FNamesArray[NameIdx]->Name);
}

std::wstring GetNameByIndex(int32_t NameIdx) 
{
	GNames* FNamePool = GetFNamePool();
	return std::wstring(FNamePool->FNamesArray[NameIdx]->Name);
}

std::wstring UObject::GetFullName()
{
	std::wstring fullName = this->GetName();

	for (UObject* uOuter = this->Outer; uOuter; uOuter = uOuter->Outer)
	{
		fullName = (uOuter->GetName() + L"." + fullName);
	}

	fullName = this->Class->GetName() + L" " + fullName;

	return fullName;
}

bool UObject::IsA(UClass* uClass)
{
	for (UClass* uSuperClass = this->Class; uSuperClass; uSuperClass = static_cast<UClass*>(uSuperClass->SuperField.SuperField))
	{
		if (uSuperClass && uSuperClass == uClass)
		{
			return true;
		}
	}
	return false;
}

std::vector<UObject*> GetInstancesByClass(UClass* uClass)
{
	uintptr_t objectCountAddress = BaseAddress + 0x24BF4E0;
	int32_t objectNumber = *reinterpret_cast<int32_t*>(objectCountAddress);
	std::vector<UObject*> instances;

	for (int i = 0; i < objectNumber; i++)
	{
		UObject* currObject = GetObjectByIndex(i);

		if (currObject && currObject->IsA(uClass))
		{
			instances.push_back(currObject);
		}
	}
	return instances;
}


bool UObject::HasOuter()
{
	return Outer != this;
}

void Main()
{
	GNames* NamePool = GetFNamePool();
	UObject* Object = GetObjectByIndex(0);
	int currentIndex = 0;
	FILE* file;
	_wfopen_s(&file, L"addresses.txt", L"w+");
	uintptr_t objectCountAddress = BaseAddress + 0x24BF4E0;
	int32_t objectNumber = *reinterpret_cast<int32_t*>(objectCountAddress);
	uintptr_t LocalPlayerAddress = 0x0;
	UClass* LocalPlayer = nullptr;
	UObject* LocalPlayerObject = nullptr;
	while (currentIndex < objectNumber)
	{
		if (Object) {
			fwprintf(file, L"Name: %ls\n, address: %p\n", Object->GetFullName().c_str(), Object);
			if (Object->GetFullName().find(L"GFxData_LocalPlayer_TA Transient.GFxData_LocalPlayer_TA") != std::string::npos) {
				LocalPlayer = reinterpret_cast<UClass*>(Object->Class);
				LocalPlayerObject = reinterpret_cast<UObject*>(Object);
				wprintf(L"LocalPlayerObject: 0x%p\nLocalPlayerAddress: 0x%p\n", Object, LocalPlayer);
			}
		}

		currentIndex++;
		if (currentIndex >= objectNumber)
			break;
		Object = GetObjectByIndex(currentIndex);
	}
	fclose(file);
	UStruct* LocalPlayerMembers = LocalPlayer->Members;
	for (int i = 0; i < 1000; i++) {
		if (LocalPlayerMembers)
		{
			wprintf(L"%ls.%ls 0x%08X\n", LocalPlayerMembers->Outer->GetName().c_str(), GetNameByIndex(LocalPlayerMembers->NameIdx).c_str(), LocalPlayerMembers->Offset);
			LocalPlayerMembers = LocalPlayerMembers->Next;
		}
		else {
			break;
		}
	}

	std::vector<UObject*> instances = GetInstancesByClass(LocalPlayer);
	for (int i = 0; i < instances.size(); i++)
	{
		UObject* currObject = instances[i];
		if (currObject)
		{
			wprintf(L"Classe %ls; instance: %ls %p\n", LocalPlayer->GetName().c_str(), currObject->GetFullName().c_str(), currObject);
		}
	}
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		freopen("CONIN$", "r", stdin);
		std::cout << "DLL Injected!" << std::endl;
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}
