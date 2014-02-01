#include "WPILib.h"
#include "EGamepad.h"
#include "test/base.h"
/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 

// This demo progam has been modified to include code that runs in test mode. The intent of this
// code is to enable the manipulation of individual channels on any of the modules using a menu-
// driven interface which uses a few gamepad buttons and the LCD screen as its interface

class RobotDemo : public SimpleRobot
{
	EGamepad          gamepad;   // for test mode
	DriverStationLCD *dsLCD;

public:
	RobotDemo(void):
		gamepad(3)
	{
		dsLCD = DriverStationLCD::GetInstance();
		
		// Output the program name and build date/time in the hope that this will help
		// us catch cases where we are downloading a program other than the one
		// we think we are downloading. Keep in mind that if this source file
		// does not change (and you don't do a complete rebuild) the timestamp
		// will not change.
		
		dsLCD->Clear();
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "2013 Test Menu");
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, __DATE__ " "__TIME__);
		dsLCD->UpdateLCD();
	}

	/**
	 * Output a message to the LCD to let users know they have strayed
	 * into a useless mode
	 */
	
	void Autonomous(void)
	{
		dsLCD->Clear();
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "2013 Test Fix");
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Autonomous Mode");
		dsLCD->UpdateLCD();
	}

	/**
	 * Output a message to the LCD to let users know they have strayed
	 * into a useless mode
	 */
	
	void OperatorControl(void)
	{
		// Loop counter to ensure that the program us running (debug helper
		// that can be removed when things get more stable)
		int sanity, bigSanity = 0;

		while (IsOperatorControl() &&IsEnabled())
		{
			dsLCD->Clear();
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "2013 Test Fix");
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, "Teleop Mode");
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line6, "bigSanity: %d", sanity);
			dsLCD->UpdateLCD();
			sanity++;
			if (0 == sanity % 20)
			{
				bigSanity++;
			}
			Wait(1.0);				// wait for a motor update time
		}
	}
	
	/**
	 * Run the test program
	 */
	// The test program is organized as a set of hierarchical menus that are
	// displayed on the LCD on the driver station. Each menu is either a set
	// of submenus or is a menu controlling the use of a port (or ports) on 
	// one of the IO modules plugged into the cRIO. 
	// See base.h for a description of the test menu hierarchy

	// Simplified User's guide:
	// dpad up   : move the cursor up one menu item
	// dpad down : move the cursor down one menu item
	// dpad left : decrement the value in the selected menu item or
	//             return to the previous menu if the menu item is "Back"
	// dpad right: increment the value in the selected menu item or
	//             enter a submenu (as appropriate)
	// gamepad right joystick: control the configured and enabled PWM ports
	//               (Top->Digital->PWM)
	
	void Test() 
	{
		menuType currentMenu = TOP;
		menuType newMenu     = TOP;
		
		BaseMenu * menus[NUM_MENU_TYPE];
		
		menus[TOP] = new TopMenu;
		menus[ANALOG] = new AnalogMenu;
		menus[DIGITAL_TOP] = new DigitalMenu;
		menus[SOLENOID] = new SolenoidMenu;
		menus[DIGITAL_PWM] = new PWMMenu;
		menus[DIGITAL_IO] = new DigitalIOMenu;
		menus[DIGITAL_RELAY] = new RelayMenu;
		menus[DIGITAL_IO_STATE] = new DigitalIOStateMenu;
		menus[DIGITAL_IO_CLOCK] = new DigitalIOClockMenu;
		menus[DIGITAL_IO_ENCODER] = new DigitalIOEncoderMenu;

		// Write out the TOP menu for the first time
		menus[currentMenu]->UpdateDisplay();
		
		// Initialize the button states on the gamepad
		gamepad.Update();
		
		// Loop counter to ensure that the program us running (debug helper
		// that can be removed when things get more stable)
		int sanity = 0;
	
		while (IsTest() && IsEnabled())
		{
			// The dpad "up" button is used to move the menu pointer up one line
			// on the LCD display
			if (kEventClosed == gamepad.GetDPadEvent(Gamepad::kUp))
			{
				menus[currentMenu]->HandleIndexUp();
			}
			
			// The dpad "down" button is used to move the menu pointer down one line
			// on the LCD display
			if (kEventClosed == gamepad.GetDPadEvent(Gamepad::kDown))
			{
				menus[currentMenu]->HandleIndexDown();
			} 
			
			// The dpad left button is used to exit a submenu when the menu pointer
			// points to the "back" menu item and to decrease a value (where 
			// appropriate) on any other menu item.
			if (kEventClosed == gamepad.GetDPadEvent(Gamepad::kLeft))
			{
				newMenu = menus[currentMenu]->HandleSelectLeft();
			}
			
			// Theoretically, both the select buttons could be pressed in the 
			// same 10 msec window. However, if using the dpad on the game 
			// game controller this is physically impossible so we don't
			// need to worry about a previous value of newMenu being 
			// overwritten in the next bit of code.
			
			// The dpad right button is used to enter a submenu when the menu pointer
			// points to a submenu item and to increase a value (where  appropriate) 
			// on any other menu item.
			if (kEventClosed == gamepad.GetDPadEvent(Gamepad::kRight))
			{
				newMenu = menus[currentMenu]->HandleSelectRight();
				
				// Handle change from one menu to a sub menu
				if (newMenu != currentMenu)
				{
					// When we enter a menu we need to set the record the
					// menu to return to. We do *not* want to do this when
					// returning from a menu to its calling menu.
					menus[newMenu]->SetCallingMenu(currentMenu);
				}
			}
			
			// Handle change from one menu to another
			if (newMenu != currentMenu)
			{
				menus[newMenu]->UpdateDisplay();
				currentMenu = newMenu;
			}

			// Set the motor speed(s) (if any have been enabled via the Digital PWM menu)
			menus[DIGITAL_PWM]->SetSpeed(-1.0 * gamepad.GetRightY());
			
			// Update gamepad button states
			gamepad.Update();
			
			// Update the display (we do this on every loop pass because some menus
			// (analog, for example) need to have values updated even when there are
			// no dpad events to handle)
			menus[currentMenu]->UpdateDisplay();
			
			// Dump the sanity time value to the LCD
			
//			dsLCD->PrintfLine(DriverStationLCD::kUser_Line6, "Sanity: %d", bigSanity);
			dsLCD->UpdateLCD();
			
			sanity++;
			if (0 == sanity % 20)
			{
				printf ("sanity = %d\r", sanity/20);
			}
			
			// Run the loop every 50 msec (20 times per second)
			Wait(0.050);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

