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

// PWM ports for robotDrive
#define LEFT_DRIVE_PWM  1
#define RIGHT_DRIVE_PWM 2

class RobotDemo : public SimpleRobot
{
	Jaguar     leftMotor;
	Jaguar     rightMotor;
	RobotDrive myRobot;   // robot drive system
	Joystick   stick;     // only joystick
	EGamepad   gamepad;   // for test mode
	
	DriverStationLCD *dsLCD;

public:
	RobotDemo(void):
		leftMotor (LEFT_DRIVE_PWM),
		rightMotor(RIGHT_DRIVE_PWM),
		myRobot(&leftMotor, &rightMotor),	// Using this constructor to allow us
		stick(1),							// to have pointers to the motor controllers
		gamepad(3)							
	{
		myRobot.SetExpiration(0.1);
		//myRobot.SetSafetyEnabled(false);
		
		dsLCD = DriverStationLCD::GetInstance();
		
		// Output the program name and build date/time in the hope that this will help
		// us catch cases where we are downloading a program other than the one
		// we think we are downloading. Keep in mind that if this source file
		// does not change (and you don't do a complete rebuild) the timestamp
		// will not change.
		
		dsLCD->Clear();
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line1, "2013 Test");
		dsLCD->PrintfLine(DriverStationLCD::kUser_Line2, __DATE__ " "__TIME__);
		dsLCD->UpdateLCD();
	}

	/**
	 * Drive left & right motors for 2 seconds then stop
	 */
	
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
		myRobot.Drive(-0.5, 0.0); 	// drive forwards half speed
		Wait(2.0); 					// for 2 seconds
		myRobot.Drive(0.0, 0.0); 	// stop robot
	}

	/**
	 * Runs the motors with arcade steering. 
	 */
	
	void OperatorControl(void)
	{
		myRobot.SetSafetyEnabled(true);
		while (IsOperatorControl())
		{
			myRobot.ArcadeDrive(stick); // drive with arcade style (use right stick)
			Wait(0.005);				// wait for a motor update time
		}
	}
	
	/**
	 * Run the test program
	 */
	
	// The test program is organized as a set of hierarchical menus that are
	// displayed on the LCD on the driver station. Each menu is either a set
	// of submenus or is a menu controlling the use of a port (or ports) on 
	// one of the IO modules plugged into the cRIO. The gamepad D-Pad is used
	// to navigate the menus:
	// Up    - move up on menu item
	// Down  - move down one menu iteme
	// Left  - decrease a menu item value or return to previous menu
	//         if at the "back" menu item
	// Right - increase a menu item or enter the selected submenu
	// See base.h for a description of the test menu hierarchy

	void Test() 
	{
		menuType currentMenu = TOP;
		menuType newMenu     = TOP;
		
		// We use a table of pointers to menu objects - one for each menu.
		BaseMenu * menus[NUM_MENU_TYPE];
		
		menus[TOP] = new TopMenu;
		menus[ANALOG] = new AnalogMenu;
		menus[DIGITAL_TOP] = new DigitalMenu;
		menus[SOLENOID] = new SolenoidMenu;
		menus[DIGITAL_PWM] = new PWMMenu;
		menus[DIGITAL_IO] = new DigitalIOMenu;
		menus[DIGITAL_RELAY] = new RelayMenu;
		menus[DIGITAL_IO_STATE] = new DigitalIOStateMenu;
		menus[DIGITAL_IO_CLOCK] = new BaseMenu;
		menus[DIGITAL_IO_ENCODER] = new DigitalIOEncoderMenu;

		// Inform appropriate menus of already allocated ports
		menus[DIGITAL_PWM]->SetPWMTableEntry (LEFT_DRIVE_PWM, &leftMotor);
		menus[DIGITAL_PWM]->SetPWMTableEntry (RIGHT_DRIVE_PWM, &rightMotor);

		// Write out the TOP menu for the first time
		menus[currentMenu]->UpdateDisplay();
		
		// Initialize the button states on the gamepad
		gamepad.Update();
		
		// Loop counter to ensure that the program us running (debug helper
		// that can be removed when things get more stable)
		int sanity = 0;
	
		while (IsTest())
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
			dsLCD->PrintfLine(DriverStationLCD::kUser_Line6, "Sanity: %d", sanity);
			dsLCD->UpdateLCD();
			
			sanity++;
			
			// Run the loop every 50 msec (20 times per second)
			Wait(0.050);
		}
	}
};

START_ROBOT_CLASS(RobotDemo);

