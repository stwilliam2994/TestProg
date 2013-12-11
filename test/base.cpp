#include "base.h"

BaseMenu::BaseMenu()
{
	dsLCD = DriverStationLCD::GetInstance();
	index_m = 2;
	maxIndex_m = 2;
	minIndex_m = 2;
	callingMenu_m = TOP;
}

BaseMenu::~BaseMenu()
{
	
}

void BaseMenu::HandleIndexDown ()
{
	index_m++;
	if (index_m > maxIndex_m)
	{
		index_m = minIndex_m;
	}
	//UpdateDisplay();
}

void BaseMenu::HandleIndexUp ()
{
	index_m--;
	if (index_m < minIndex_m)
	{
		index_m = maxIndex_m;
	}
	//UpdateDisplay();
}
	
menuType BaseMenu::HandleSelectLeft ()
{
	return TOP;
}

menuType BaseMenu::HandleSelectRight ()
{
	return TOP;
}

void BaseMenu::UpdateDisplay ()
{
	dsLCD->Clear();
	dsLCD->PrintfLine(LCD1,  "Base Class!");
	dsLCD->PrintfLine(LCD1,  "*Back");
	dsLCD->UpdateLCD();
}

void BaseMenu::SetCallingMenu (menuType callingMenu)
{
	callingMenu_m = callingMenu;
}

DriverStationLCD::Line BaseMenu::IndexToLCDLine (int line)
{
	return static_cast<DriverStationLCD::Line>(line - 1);
}
	
	
