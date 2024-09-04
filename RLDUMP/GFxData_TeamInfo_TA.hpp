#pragma once
#include <iostream>
#include <string>
#include "UObject.hpp"

class FName
{
public:
	wchar_t Name[400];
};

class GFxData_TeamInfo_TA : public UObject
{
public:
	char pad_0000[0x30]; // 0x60
	uint32_t Team; // 0x90
	class FName* TeamName; // 0x98
	char pad_0001[0xC]; // 0xA0
	uint32_t Score; // 0xAC

public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;
		if (!uClassPointer)
		{
			return reinterpret_cast<UClass*>(UObject::GetObjectByName(L"Class TAGame.GFxData_TeamInfo_TA"));
		}
		return uClassPointer;
	}

	std::wstring GetTeamName();

	static std::vector<GFxData_TeamInfo_TA*> GetInstances();
};


/*

StrProperty GFxData_TeamInfo_TA.TeamName - (0x00000098)
StructProperty GFxData_TeamInfo_TA.TeamColor - (0x000000A8)
IntProperty GFxData_TeamInfo_TA.Score - (0x000000AC)
ObjectProperty GFxData_TeamInfo_TA.TeamLogo - (0x000000B0)
IntProperty GFxData_TeamInfo_TA.SeriesScore - (0x000000B8)
ObjectProperty GFxData_TeamInfo_TA.Team - (0x000000C0)
IntProperty GFxData_TeamInfo_TA.Difficulty - (0x000000C8)
NameProperty GFxDataRow_X.TableName - (0x00000060)
NameProperty GFxDataRow_X.PrimaryKeyName - (0x00000068)
ClassProperty GFxDataRow_X.ProxyClass - (0x00000070)
BoolProperty GFxDataRow_X.bLevelTransitionPersistent - (0x00000078)
ObjectProperty GFxDataRow_X.ProxyObject - (0x00000080)
ObjectProperty GFxDataRow_X.Shell - (0x00000088)
IntProperty GFxDataRow_X.RowIndex - (0x00000090)
StructProperty Object.VfTableObject - (0x00000000)
StructProperty Object.HashNext - (0x00000008)
QWordProperty Object.ObjectFlags - (0x00000010)
StructProperty Object.HashOuterNext - (0x00000018)
StructProperty Object.StateFrame - (0x00000020)
ObjectProperty Object.Linker - (0x00000028)
StructProperty Object.LinkerIndex - (0x00000030)
IntProperty Object.ObjectInternalInteger - (0x00000038)
IntProperty Object.NetIndex - (0x0000003C)
ObjectProperty Object.Outer - (0x00000040)
NameProperty Object.Name - (0x00000048)
ClassProperty Object.Class - (0x00000050)
ObjectProperty Object.ObjectArchetype - (0x00000058)

*/