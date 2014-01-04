#include "base.h"

// Digital IO Menu


DigitalIOMenu::DigitalIOMenu()
{
	// The Digital IO menu looks like this (not including the 1st 2 columns):
	// 1:Digital I/O
	// 2: State
	// 3: Clock
	// 4: Encoder
	// 5: Back
	// 6:

	index_m = 2;
	maxIndex_m = 5;
}

DigitalIOMenu::~DigitalIOMenu()
{

}

menuType DigitalIOMenu::HandleSelectLeft ()
{
	switch (index_m) 
	{
		case 5:
			return callingMenu_m;
			break;
		default:
			break;
	}
	
	return DIGITAL_IO;
}

menuType DigitalIOMenu::HandleSelectRight ()
{
	switch (index_m) 
	{
		case 2:
			return DIGITAL_IO_STATE;
			break;
		case 3:
			break;
		case 4:
			return DIGITAL_IO_ENCODER;
			break;
		default:
			break;
	}
	
	return DIGITAL_IO;
}

void DigitalIOMenu::UpdateDisplay ()
{
	dsLCD->PrintfLine(LCD1, "Digital I/O");
	dsLCD->PrintfLine(LCD2, " State");
	dsLCD->PrintfLine(LCD3, " Clock");
	dsLCD->PrintfLine(LCD4, " Encoder");
	dsLCD->PrintfLine(LCD5, " Back");
	dsLCD->Printf(IndexToLCDLine(index_m), 1, "*");

	dsLCD->UpdateLCD();
}
