#pragma once
#include <iostream>
#include <string>

class UObject;
class UClass;

class UField
{
public:
	char pad_0000[8]; //0x0000
	class UClass* SuperField; //0x0008
	class UClass* Children; //0x0010
	uint32_t PropSize; //0x0018
	char pad_001C[4]; //0x001C
};

class UStruct
{
public:
	char pad_0000[64]; //0x0000
	class UObject* Outer; //0x0040
	uint32_t NameIdx; //0x0048
	char pad_004C[76]; //0x004C
	uint32_t Offset; //0x0098
	char pad_009C[4]; //0x009C
	class UStruct* Next; //0x00A0
	char pad_00A8[32]; //0x00A8
};

class UClass : public UObject
{
public:
	char pad_0008[0x20]; //0x0008
	class UField SuperField; //0x0078
	char pad_0098[0x78]; //0x0098
	class UStruct* Members; //0x0110
	char pad_0118[128]; //0x0118
};