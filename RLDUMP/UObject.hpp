/*
Written by: DRKnight
https://github.com/DRKn1ght
https://github.com/DRKn1ght/RocketLeague-Data-Dumper
*/


#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// FPointer
// (0x0000 - 0x0008)
struct FPointer
{
	uintptr_t Dummy;
};

class FNameEntry
{
public:
	int64_t Flags; //0x0000
	int32_t Index; //0x0008
	char pad_000C[12]; //0x000C
	wchar_t Name[0x400]; //0x0018
}; //Size: 0x0338

class FNamePool
{
public:
	class FNameEntry* FNamesArray[165366]; //0x0000

	static std::wstring GetNameByIndex(int32_t NameIdx);
}; //Size: 0x30D400


class UClass;

class UObject
{
public:
	struct FPointer VfTable; //0x0000
	struct FPointer* HashNext; //0x0008
	uint64_t UObjectFlags; //0x0010
	struct FPointer* HashOuterNext; //0x0018
	struct FPointer* StateFrame; //0x0020
	class UObject* Linker; //0x0028
	int64_t LinkerIndex; //0x0030
	uint32_t ObjectInternalInteger; //0x0038
	uint32_t NetIndex; //0x003C
	class UObject* Outer; //0x0040
	uint32_t NameIndex; //0x0048
	char pad_004C[4]; //0x004C
	class UClass* Class; //0x0050
	class UObject* ObjectArchtype; //0x0058


	bool HasOuter();
	std::wstring GetName();
	std::wstring GetFullName();
	bool IsA(UClass* Class);

	static uint32_t GetObjectCount();

	static std::vector<UObject*> GetInstancesByClass(UClass* uClass);

	static UObject* GetObjectByIndex(int index);

	static UObject* GetObjectByName(const wchar_t* ClassName);

	static void InitGObjects();

}; //Size: 0x0060

struct FUObjectItem
{
	class UObject* Object;
};

class TUObjectArray {
public:
	FUObjectItem* Objects;     // Ponteiro para o array de pointeiros para os UObject
	int32_t NumElements;		// N�mero de elementos atuais no array
public:
	inline int32_t Num() const
	{
		return NumElements;
	}
};