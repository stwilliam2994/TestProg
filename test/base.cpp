#include "base.h"

BaseMenu::BaseMenu()
{
	// The Base class menu looks like this (not including the 1st 2 columns):
	// 1:Base Class!
	// 2: Back
	// 3: 
	// 4: 
	// 5:
	// 6:
	
	// You should never see this class unless there is a bug in this program

	dsLCD = DriverStationLCD::GetInstance();
	
	index_m       = 2;
	maxIndex_m    = 2;
	minIndex_m    = 2;
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
}

void BaseMenu::HandleIndexUp ()
{
	index_m--;
	if (index_m < minIndex_m)
	{
		index_m = maxIndex_m;
	}
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
	dsLCD->PrintfLine(LCD2,  "*Back");
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
	
void BaseMenu::SetSpeed (float speed)
{
	
}

void BaseMenu::SetTableEntry (int index, Jaguar * pointer)
{
	
}

