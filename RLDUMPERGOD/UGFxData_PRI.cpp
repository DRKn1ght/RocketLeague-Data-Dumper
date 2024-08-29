#include "UGFxData_PRI.hpp"

std::wstring UGFxData_PRI_TA::GetPlayerName()
{
	return std::wstring(this->PlayerName).c_str();
}

std::vector<UGFxData_PRI_TA*> UGFxData_PRI_TA::GetInstances()
{
	std::vector<UGFxData_PRI_TA*> TeamInstances;
	std::vector<UObject*> TeamInstancesObject = UObject::GetInstancesByClass(UGFxData_PRI_TA::StaticClass());
	for (int i = 1; i < TeamInstancesObject.size(); i++)
	{
		UGFxData_PRI_TA* TeamInfo = reinterpret_cast<UGFxData_PRI_TA*>(TeamInstancesObject[i]);
		TeamInstances.push_back(TeamInfo);
	}
	return TeamInstances;
}