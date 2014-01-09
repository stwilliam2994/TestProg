#include "base.h"

// Digital Menu


DigitalMenu::DigitalMenu()
{
	// The Digital menu looks like this (not including the 1st 2 columns):
	// 1:Digital
	// 2: I/O
	// 3: PWM
	// 4: Relay
	// 5: Back
	// 6:

	index_m = 2;
	maxIndex_m = 5;
}

menuType DigitalMenu::HandleSelectLeft ()
{
	switch (index_m) 
	{
	// These cases can be removed because only back has meaning for a left select
	case 2:
		break;
	case 3:
		break;
	case 4:
		break;
	case 5:
		return callingMenu_m;
		break;
	default:
		break;
	}
	return DIGITAL_TOP;

}

menuType DigitalMenu::HandleSelectRight ()
{
	switch (index_m) 
	{
	case 2:
		return DIGITAL_IO;
		break;
	case 3:
		return DIGITAL_PWM;
		break;
	case 4:
		break;
	case 5:
		break;
	default:
		break;
	}
	return DIGITAL_TOP;
}

void DigitalMenu::UpdateDisplay ()
{
	dsLCD->PrintfLine(LCD1, "Digital");
	dsLCD->PrintfLine(LCD2, " I/O");
	dsLCD->PrintfLine(LCD3, " PWM");
	dsLCD->PrintfLine(LCD4, " Relay");
	dsLCD->PrintfLine(LCD5, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
