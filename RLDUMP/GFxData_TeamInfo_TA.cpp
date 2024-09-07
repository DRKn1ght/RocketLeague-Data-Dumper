/*
Written by: DRKnight
https://github.com/DRKn1ght
https://github.com/DRKn1ght/RocketLeague-Data-Dumper
*/


#include "GFxData_TeamInfo_TA.hpp"

std::wstring GFxData_TeamInfo_TA::GetTeamName()
{
	return std::wstring(this->TeamName->Name).c_str();
}

std::vector<GFxData_TeamInfo_TA*> GFxData_TeamInfo_TA::GetInstances()
{
	std::vector<GFxData_TeamInfo_TA*> TeamInstances;
	std::vector<UObject*> TeamInstancesObject = UObject::GetInstancesByClass(GFxData_TeamInfo_TA::StaticClass());
	for (int i = 1; i < TeamInstancesObject.size(); i++)
	{
		GFxData_TeamInfo_TA* TeamInfo = reinterpret_cast<GFxData_TeamInfo_TA*>(TeamInstancesObject[i]);
		TeamInstances.push_back(TeamInfo);
	}
	return TeamInstances;
}