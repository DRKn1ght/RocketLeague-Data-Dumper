#pragma once
#define _CRT_SECURE_NO_WARNINGS
#include <windows.h>
#include <vector>
#include "UObject.hpp"
#include "UClass.hpp"
#include "GFxData_TeamInfo_TA.hpp"
#include "UGFxData_PRI.hpp"
#include <chrono>
#include <thread>

// Function to center text within a given width
std::wstring centerText(const std::wstring& text, size_t width) {
	size_t padding = width > text.length() ? (width - text.length()) / 2 : 0;
	std::wstring centeredText = std::wstring(padding, L' ') + text + std::wstring(padding, L' ');

	// If there's an odd amount of padding, adjust to ensure it's fully centered
	if (centeredText.length() < width) {
		centeredText += L' ';
	}

	return centeredText;
}

void PrintTeamTable(std::vector<GFxData_TeamInfo_TA*>& Teams, std::vector<UGFxData_PRI_TA*>& Players)
{
	system("cls");
	// Determine the width of the columns
	size_t columnWidth = 20;

	// Print the top border
	std::wcout << L"|" << std::wstring(columnWidth + 1, L'=') << L"="
		<< std::wstring(columnWidth + 1, L'=') << L"|" << std::endl;

	// Print the header with team names
	std::wcout << L"| " << centerText(Teams[0]->GetTeamName(), columnWidth)
		<< L"| " << centerText(Teams[1]->GetTeamName(), columnWidth) << L"|" << std::endl;

	std::wcout << L"| " << centerText(std::to_wstring(Teams[0]->Score), columnWidth)
		<< L"| " << centerText(std::to_wstring(Teams[1]->Score), columnWidth) << L"|" << std::endl;

	// Print the separator
	std::wcout << L"|" << std::wstring(columnWidth + 1, L'=') << L"|" 
               << std::wstring(columnWidth + 1, L'=') << L"|" << std::endl;

	std::vector<UGFxData_PRI_TA*> Team1Players;
	std::vector<UGFxData_PRI_TA*> Team2Players;

	for (auto player : Players)
	{
		if (player->Team == Teams[0]->Team)
			Team1Players.push_back(player);
		else
			Team2Players.push_back(player);
	}

	size_t maxPlayers = (((Team1Players.size()) > (Team2Players.size())) ? (Team1Players.size()) : (Team2Players.size()));

	for (size_t i = 0; i < maxPlayers; ++i)
	{
		std::wcout << L"| " << centerText((i < Team1Players.size() ? Team1Players[i]->GetPlayerName() : L""), columnWidth)
			<< L"| " << centerText((i < Team2Players.size() ? Team2Players[i]->GetPlayerName() : L""), columnWidth)
			<< L"|" << std::endl;
	}
	// Print the bottom border
	std::wcout << L"|" << std::wstring(columnWidth + 1, L'=') << L"="
		<< std::wstring(columnWidth + 1, L'=') << L"|" << std::endl;
}

void Main()
{
	UObject::InitGObjects();
	UClass* GFxData_TeamInfo = nullptr;
	UClass* UGFxData_PRI = nullptr;

	std::vector<GFxData_TeamInfo_TA*> TeamInstances = GFxData_TeamInfo_TA::GetInstances();
	std::vector<UGFxData_PRI_TA*> PlayerInstances = UGFxData_PRI_TA::GetInstances();
		
	while (true)
	{
		PrintTeamTable(TeamInstances, PlayerInstances);
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Main, 0, 0, 0);
		AllocConsole();
		SetWindowPos(GetConsoleWindow(), HWND_TOPMOST, 0, 0, 0, 0, SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW);
		ShowWindow(GetConsoleWindow(), SW_NORMAL);
		freopen("CONOUT$", "w", stdout);
		freopen("CONOUT$", "w", stderr);
		freopen("CONIN$", "r", stdin);
		std::cout << "DLL Injected!" << std::endl;
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}