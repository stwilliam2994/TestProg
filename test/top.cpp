#include "base.h"

// Top Menu

// The top menu looks like this (not including the first two columns
// 1:Top
// 2: Analog
// 3: Digital
// 4: Solenoid
// 5: Reset
// 6:

TopMenu::TopMenu()
{
	index_m = 2;
	maxIndex_m = 5;
}

menuType TopMenu::HandleSelectLeft ()
{
	//return HandleSelect();
	// We've decided that selecting a submenu should be right
	// button, exiting a submenu (returning to the parent menu
	// via the back button) should be left button only
	return TOP;
}

menuType TopMenu::HandleSelectRight ()
{
	return HandleSelect();
}

menuType TopMenu::HandleSelect ()
{
	switch (index_m) 
	{
	case 2:
		return ANALOG;
		break;
	case 3:
		return DIGITAL_TOP;
		break;
	case 4:
		return SOLENOID;
		break;
	case 5:
		break;
	default:
		break;
	}
	return TOP;
}


void TopMenu::UpdateDisplay ()
{
	dsLCD->PrintfLine(LCD1, "Top");
	dsLCD->PrintfLine(LCD2, " Analog");
	dsLCD->PrintfLine(LCD3, " Digital");
	dsLCD->PrintfLine(LCD4, " Solenoid");
	dsLCD->PrintfLine(LCD5, " Reset");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
