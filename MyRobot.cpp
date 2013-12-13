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
	RobotDrive myRobot;   // robot drive system
	Joystick   stick;     // only joystick
	EGamepad   gamepad;   // for test mode
	
	DriverStationLCD *dsLCD;

public:
	RobotDemo(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick(1),		// as they are declared above.
		gamepad(3)
	{
		myRobot.SetExpiration(0.1);
		
		dsLCD = DriverStationLCD::GetInstance();
		
		// Output the program name a build data in the hope that this will help
		// us catch cases where we are downloading a program other than the one
		// we think we are downloading. Keep in mind that if this source file
		// does not change (and you don't so a complete rebuild) the timestamp
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
	
	// <need to describe the menu hierarchy and user interface here>
	void Test() 
	{
		menuType currentMenu = TOP;
		menuType newMenu = TOP;
		BaseMenu * menus[NUM_MENU_TYPE];
		menus[TOP] = new TopMenu;
		menus[ANALOG] = new AnalogMenu;
		menus[DIGITAL_TOP] = new DigitalMenu;
		menus[SOLENOID] = new SolenoidMenu;
		menus[DIGITAL_PWM] = new PWMMenu;
		menus[DIGITAL_IO] = new BaseMenu;
		menus[DIGITAL_RELAY] = new BaseMenu;
		menus[DIGITAL_IO_STATE] = new BaseMenu;
		menus[DIGITAL_IO_CLOCK] = new BaseMenu;
		menus[DIGITAL_IO_ENCODER] = new BaseMenu;

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
			// overwritten here.
			
			// The dpad right button is used to enter a submenu when the menu pointer
			// points to a submenu item and to increase a value (where 
			// appropriate) on any other menu item.
			if (kEventClosed == gamepad.GetDPadEvent(Gamepad::kRight))
			{
				newMenu = menus[currentMenu]->HandleSelectRight();
			}
			
			// Handle change from one menu to another
			if (newMenu != currentMenu)
			{
				menus[newMenu]->SetCallingMenu(currentMenu);
				menus[newMenu]->UpdateDisplay();
				currentMenu = newMenu;
			}
			
			// Set the motor speed(s) (if any are enabled)
			menus[DIGITAL_PWM]->SetSpeed(gamepad.GetRightX());
			
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

