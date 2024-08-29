#pragma once
#include <iostream>
#include <string>
#include "UObject.hpp"

class UGFxData_PRI_TA : public UObject
{
public:
	char pad_0000[0x38]; // 0x60
	wchar_t* PlayerName; // 0x98
	char pad_0001[0x30]; // 0xA0
	uint32_t Team; // 0xD0

public:
	static UClass* StaticClass()
	{
		static UClass* uClassPointer = nullptr;
		if (!uClassPointer)
		{
			return reinterpret_cast<UClass*>(UObject::GetObjectByName(L"Class TAGame.GFxData_PRI_TA"));
		}
		return uClassPointer;
	}

	std::wstring GetPlayerName();

	static std::vector<UGFxData_PRI_TA*> GetInstances();
};


/*

StrProperty GFxData_PRI_TA.PlayerName - (0x00000098)
StrProperty GFxData_PRI_TA.PlayerIDString - (0x000000A8)
StrProperty GFxData_PRI_TA.Platform - (0x000000B8)
ObjectProperty GFxData_PRI_TA.Avatar - (0x000000C8)
IntProperty GFxData_PRI_TA.Team - (0x000000D0)
IntProperty GFxData_PRI_TA.Score - (0x000000D4)
IntProperty GFxData_PRI_TA.Goals - (0x000000D8)
IntProperty GFxData_PRI_TA.Assists - (0x000000DC)
IntProperty GFxData_PRI_TA.Saves - (0x000000E0)
IntProperty GFxData_PRI_TA.Shots - (0x000000E4)
IntProperty GFxData_PRI_TA.Ping - (0x000000E8)
IntProperty GFxData_PRI_TA.BreakoutDamage - (0x000000EC)
BoolProperty GFxData_PRI_TA.bMvp - (0x000000F0)
BoolProperty GFxData_PRI_TA.bSpeaking - (0x000000F0)
BoolProperty GFxData_PRI_TA.bMuted - (0x000000F0)
BoolProperty GFxData_PRI_TA.bBlocked - (0x000000F0)
BoolProperty GFxData_PRI_TA.bReady - (0x000000F0)
BoolProperty GFxData_PRI_TA.bBotPlayer - (0x000000F0)
BoolProperty GFxData_PRI_TA.bLocalPlayer - (0x000000F0)
BoolProperty GFxData_PRI_TA.bDisconnected - (0x000000F0)
BoolProperty GFxData_PRI_TA.bStayAsPartyActive - (0x000000F0)
BoolProperty GFxData_PRI_TA.bSpectator - (0x000000F0)
BoolProperty GFxData_PRI_TA.bCompletedPlacementMatches - (0x000000F0)
BoolProperty GFxData_PRI_TA.bSelected - (0x000000F0)
BoolProperty GFxData_PRI_TA.bHighlighted - (0x000000F0)
BoolProperty GFxData_PRI_TA.bGameEnded - (0x000000F0)
BoolProperty GFxData_PRI_TA.bOnPodium - (0x000000F0)
BoolProperty GFxData_PRI_TA.bWantsToHonorDuel - (0x000000F0)
BoolProperty GFxData_PRI_TA.bChallengedToHonorDuel - (0x000000F0)
BoolProperty GFxData_PRI_TA.bCanChallengeToHonorDuel - (0x000000F0)
StrProperty GFxData_PRI_TA.ConnectionStatus - (0x000000F8)
StrProperty GFxData_PRI_TA.PartyID - (0x00000108)
StrProperty GFxData_PRI_TA.XPTitle - (0x00000118)
StructProperty GFxData_PRI_TA.TitleColor - (0x00000128)
StructProperty GFxData_PRI_TA.TitleGlowColor - (0x0000012C)
IntProperty GFxData_PRI_TA.SkillTier - (0x00000130)
IntProperty GFxData_PRI_TA.Boost - (0x00000134)
ObjectProperty GFxData_PRI_TA.PRI - (0x00000138)
StructProperty GFxData_PRI_TA.PlayerID - (0x00000140)
FloatProperty GFxData_PRI_TA.PodiumSpotlightScreenPosX - (0x00000188)
FloatProperty GFxData_PRI_TA.PodiumSpotlightScreenPosY - (0x0000018C)
StrProperty GFxData_PRI_TA.PrimaryTitle - (0x00000190)
StrProperty GFxData_PRI_TA.SecondaryTitle - (0x000001A0)
StrProperty GFxData_PRI_TA.PrimaryTitleStat - (0x000001B0)
StrProperty GFxData_PRI_TA.SecondaryTitleStat - (0x000001C0)
IntProperty GFxData_PRI_TA.PrimaryStat - (0x000001D0)
IntProperty GFxData_PRI_TA.SecondaryStat - (0x000001D4)
QWordProperty GFxData_PRI_TA.ClubID - (0x000001D8)
ByteProperty GFxData_PRI_TA.WorstConnectionQualityStateBeyondLatency - (0x000001E0)
ByteProperty GFxData_PRI_TA.QuitSeverity - (0x000001E1)
IntProperty GFxData_PRI_TA.SpectatorShortcut - (0x000001E4)
ObjectProperty GFxData_PRI_TA.GFxPlayerBanner - (0x000001E8)
ObjectProperty GFxData_PRI_TA.GFxPlayerAvatar - (0x000001F0)
ObjectProperty GFxData_PRI_TA.GFxPlayerBorder - (0x000001F8)
ObjectProperty GFxData_PRI_TA.Persona - (0x00000200)
ObjectProperty GFxData_PRI_TA.PrimaryPersona - (0x00000208)
ObjectProperty GFxData_PRI_TA.Personas - (0x00000210)
ObjectProperty GFxData_PRI_TA.ScoreboardStats - (0x00000218)
ObjectProperty GFxData_PRI_TA.SpectatorStats - (0x00000220)
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