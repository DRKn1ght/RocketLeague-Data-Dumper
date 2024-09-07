/*
Written by: DRKnight
https://github.com/DRKn1ght
https://github.com/DRKn1ght/RocketLeague-Data-Dumper
*/


#pragma once
#include <iostream>
#include <string>

class UObject;

class UStruct : public UObject
{
public:
	char pad_0000[0x20]; //0x0060
	class UStruct* SuperField; //0x0008
	class UStruct* Children; //0x0010
	uint32_t PropSize; //0x0018
	char pad_001C[4]; //0x001C
};

class UProperty : public UStruct
{
public:
	int32_t PropOffset; //0x0098
	char pad_009C[4]; //0x009C
	class UProperty* Next; //0x00A0
	char pad_00A8[32]; //0x00A8
};

class UClass : public UProperty
{
public:
	char pad_0000[0x48]; //0x0098
	class UProperty* Members; //0x110

	void PrintMembers();
}; //Size: 0x0120