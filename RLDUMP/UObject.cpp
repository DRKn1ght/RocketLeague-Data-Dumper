#include "UObject.hpp"
#include "UClass.hpp"

static uintptr_t BaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
static TUObjectArray* GObjects;
static FNamePool* GNames;

struct Offsets
{
	static const uintptr_t GObjects = 0x24BF4D8;
	static const uintptr_t GNames = 0x24BF490;
};

void UObject::InitGObjects()
{
	GObjects = reinterpret_cast<TUObjectArray*>((uintptr_t*)(BaseAddress + Offsets::GObjects));
	GNames = *(FNamePool**)(BaseAddress + Offsets::GNames);
}

UObject* UObject::GetObjectByIndex(int index)
{
	return GObjects->Objects[index].Object;
}

uint32_t UObject::GetObjectCount()
{
	return GObjects->Num();
}

std::wstring UObject::GetName()
{
	return std::wstring(GNames->FNamesArray[NameIndex]->Name);
}

std::wstring FNamePool::GetNameByIndex(int32_t NameIdx)
{
	return std::wstring(GNames->FNamesArray[NameIdx]->Name);
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

UObject* UObject::GetObjectByName(const wchar_t* ClassName)
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

void UClass::PrintMembers()
{
	for (UClass::UProperty* Member = this->Members; Member; Member = Member->Next)
	{
		wprintf(L"%ls - 0x%08X\n", Member->GetFullName().c_str(), Member->PropOffset);
	}
}

bool UObject::HasOuter()
{
	return Outer != this;
}