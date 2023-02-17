
void TTT_GUI ()
{

	int difficulty = 0;
	displayTextLine(2, "Welcome to your personal");
	displayTextLine(3, "Tic-Tic-Toe Friend!");
	displayTextLine(5, "Select Difficulty:");
	displayTextLine(7, "Easy   (left button)");
	displayTextLine(8, "Medium (enter button)");
	displayTextLine(9, "Hard   (right button)");

	while (!getButtonPress(buttonAny))
	{}


	if(getButtonPress(buttonLeft))
	{
		difficulty = 1;
		eraseDisplay();
		displayTextLine(8, "Difficulty level: EASY");
		wait1Msec(2000);
		eraseDisplay();
		displayTextLine(8, "Good Luck!");
		wait1Msec(2000);
		eraseDisplay();
		wait1Msec(2000);
		eraseDisplay();
		displayTextLine(8, "Press enter when ready!");
		while (!getButtonPress(buttonEnter))
		{}
		while (getButtonPress(buttonEnter))
		{}
	}else if(getButtonPress(buttonEnter))
	{
		difficulty = 2;
		eraseDisplay();
		displayTextLine(8, "Difficulty level: MEDIUM");
		wait1Msec(2000);
		eraseDisplay();
		displayTextLine(8, "Good Luck!");
		wait1Msec(2000);
		eraseDisplay();
		displayTextLine(8, "Press enter when ready!");
		while (!getButtonPress(buttonEnter))
		{}
		while (getButtonPress(buttonEnter))
		{}
	}else if (getButtonPress(buttonRight))
	{
		difficulty = 3;
		eraseDisplay();
		displayTextLine(8, "Difficulty level: HARD");
		wait1Msec(2000);
		eraseDisplay();
		displayTextLine(8, "Good Luck!");
		wait1Msec(2000);
		eraseDisplay();
		wait1Msec(2000);
		eraseDisplay();
		displayTextLine(8, "Press enter when ready!");
		while (!getButtonPress(buttonEnter))
		{}
		while (getButtonPress(buttonEnter))
		{}
	}
}


task main()
{

	SensorType[S1] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Color_Color;
	wait1Msec(100);


	while (!getButtonPress(buttonUp))
	{
		TTT_GUI();
	}
	eraseDisplay();
	displayTextLine(7, "Thanks for playing!");
	displayTextLine(8, "See you next time!");
	wait1Msec(2000);

}
