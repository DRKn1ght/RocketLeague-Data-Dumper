#include <windows.h>
#include <iostream>
#include "RLSDK/SdkHeaders.hpp"

void Setup()
{
    //TODO: Pattern scanning para Steam e Epic
	uintptr_t BaseAddress = reinterpret_cast<uintptr_t>(GetModuleHandle(NULL));
	GObjects = reinterpret_cast<TArray<UObject*>*>(BaseAddress + 0x353EF4C8); // steam
	GNames = reinterpret_cast<TArray<FNameEntry*>*>(BaseAddress + 0x353EF480); // steam
	UGFxData_LocalPlayer_TA* reinterpret_cast<TArray<FNameEntry*>*>(0x1992CA42800); // steam
	TArray<UObject*> pTeamInfo = reinterpret_cast<TArray<FNameEntry*>*>(0x19932438C00); // steam
	TArray<UObject*> pPRI_TA = reinterpret_cast<TArray<FNameEntry*>*>(0x1992C9F7000); // steam

	if (pULocalPlayer_TA) {
		std::cout << "Player: " << pULocalPlayer_TA->PlayerName.ToString() << std::endl;
	}

	while (true) {
		if (GetAsyncKeyState(VK_F1) & 1) {
			for (int i = 1; i < pTeamInfo.size(); i++) {
				if (pTeamInfo[i]) {
					UGFxData_TeamInfo_TA* myInstance = (UGFxData_TeamInfo_TA*)pTeamInfo[i];
					std::cout << "-=-=-=-=-=-=-=" << std::endl;
					std::cout << " - TeamName: " << myInstance->TeamName.ToString() << std::endl;
					std::cout << " - TeamScore: " << myInstance->Score << std::endl;
					std::cout << "-=-=-=-=-=-=-=" << std::endl;
				}
			}
		}
		if (GetAsyncKeyState(VK_F2) & 1) {
			for (int i = 0; i < pPRI_TA.size(); i++) {
				if (pPRI_TA[i]) {
					UGFxData_PRI_TA* myInstance = (UGFxData_PRI_TA*)pPRI_TA[i];
					if (myInstance->PlayerName.ToString().size() > 2 && myInstance->Team >= 0) {
						std::cout << "-=-=-=-=-=-=-=" << std::endl;
						std::cout << " - PlayerName: " << myInstance->PlayerName.ToString() << std::endl;
						std::cout << " - PlayerTeam: " << myInstance->Team << std::endl;
						std::cout << " - PlayerPing: " << myInstance->Ping << std::endl;
						std::cout << "-=-=-=-=-=-=-=" << std::endl;
					}
				}
			}
		}
		Sleep(100);
	}
}

BOOL WINAPI DllMain(HMODULE hMod, DWORD dwReason, LPVOID)
{
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		AllocConsole();
		freopen("CONOUT$", "w", stdout);
		SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		ShowWindow(GetConsoleWindow(), SW_NORMAL);
		DisableThreadLibraryCalls(hMod);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Setup, NULL, 0, 0);
	}

	return TRUE;
}