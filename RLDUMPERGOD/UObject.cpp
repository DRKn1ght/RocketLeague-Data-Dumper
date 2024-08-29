#include "UObject.hpp"
#include "UClass.hpp"

static uintptr_t BaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));

struct Offsets
{
	static const uintptr_t GObjects = 0x24BF4D8;
	static const uintptr_t ObjectCount = 0x24BF4E0;
	static const uintptr_t GNames = 0x24BF490;
};

UObject* UObject::GetObjectByIndex(int index)
{
	static uintptr_t GObjectsAddr = *(uintptr_t*)(BaseAddress + Offsets::GObjects);
	return *(UObject**)(GObjectsAddr + (0x8 * index));
}

uint32_t UObject::GetObjectCount()
{
	uintptr_t objectCountAddress = BaseAddress + Offsets::ObjectCount;
	return *reinterpret_cast<int32_t*>(objectCountAddress);
}

GNames* GNames::GetFNamePool()
{
	// offset: 0x24BF490
	return *(GNames**)(BaseAddress + Offsets::GNames);
}

std::wstring UObject::GetName()
{
	GNames* FNamePool = GNames::GetFNamePool();
	return std::wstring(FNamePool->FNamesArray[NameIndex]->Name);
}

std::wstring GNames::GetNameByIndex(int32_t NameIdx)
{
	GNames* FNamePool = GNames::GetFNamePool();
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
	for (UClass* uSuperClass = this->Class; uSuperClass; uSuperClass = uSuperClass = static_cast<UClass*>(uSuperClass->SuperField))
	{
		if (uSuperClass && uSuperClass == uClass)
		{
			return true;
		}
	}
	return false;
}

std::vector<UObject*> UObject::GetInstancesByClass(UClass* uClass)
{
	std::vector<UObject*> instances;

	for (int i = 0; i < UObject::GetObjectCount(); i++)
	{
		UObject* currObject = UObject::GetObjectByIndex(i);

		if (currObject && currObject->IsA(uClass))
		{
			instances.push_back(currObject);
		}
	}
	return instances;
}

UObject* UObject::GetObjectByName(const std::wstring& ClassName)
{
	uint32_t currentIndex = 0;
	while (currentIndex < GetObjectCount())
	{
		UObject* Object = GetObjectByIndex(currentIndex);
		if (Object && Object->GetFullName().find(ClassName) != std::string::npos)
		{
			return Object;
		}
		currentIndex += 1;
	}
	return nullptr;
}

bool UObject::HasOuter()
{
	return Outer != this;
}