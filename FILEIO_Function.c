/*
Test Program for PC_FileIO.c

NOTE:
To be safe, maybe have 1 input/output object going at 1 time.
I think the EV3 can only handle 1 at a time.
*/

#include "PC_FileIO.c"

// prototypes
void createEmpty();
bool writeOutput(long easy, long medium, long hard, long current);
void writeToHighscore(bool gameResult, int difficulty);
void createTestFile();
void displayScores();

task main()
{
	//reateEmpty(); //Creates the empty base file
	createTestFile(); // test file
	displayScores(); // Displays scores onto ev3

	wait1Msec(5000);
}

// reads TTT_High_Scores.txt and displays the lines for easy, medium, hard, and current (0, 1, 2, 3)
void displayScores()
{
	TFileHandle input;
	bool isOpen = openReadPC(input, "TTT_High_Scores.txt");
	if (!isOpen)
	{
		playSound(soundException);
		displayString(5, "FAILED TO OPEN FILE");
		wait1Msec(1000);
	}
	int easyHS = 0, mediumHS = 0, hardHS = 0, currentHS = 0;
	// reading input
	readIntPC(input, easyHS);
	readIntPC(input, mediumHS);
	readIntPC(input, hardHS);
	readIntPC(input, currentHS);
	// displaying input
	displayString(0, "EASY: %d", easyHS);
	displayString(1, "MEDIUM: %d", mediumHS);
	displayString(2, "HARD: %d", hardHS);
	displayString(3, "CURRENT: %d", currentHS);
}

// writes things to the highscore
// (can be a little slow since we're opening and closing for each write)
// should not be an issue for only 4 values though
void createTestFile()
{
	writeToHighscore(true, 3);
	writeToHighscore(false, 3);
	writeToHighscore(true, 1);
	writeToHighscore(true, 1);
	writeToHighscore(true, 1);
	writeToHighscore(false, 1);
	writeToHighscore(true, 1);
	writeToHighscore(false, 1);
	writeToHighscore(true, 2);
	writeToHighscore(false, 2);
}

// writes to TTT_High_Scores.txt after calculating the new scores
void writeToHighscore(bool gameResult, int difficulty)
{
	TFileHandle input;
	bool isOpen = openReadPC(input, "TTT_High_Scores.txt");
	if (!isOpen)
	{
		playSound(soundException); // just plays a sound for failure
		displayString(5, "FAILED TO OPEN FILE");
		wait1Msec(1000);
	}
	int easyHS = 0, mediumHS = 0, hardHS = 0, currentHS = 0, lastHS;
	readIntPC(input, easyHS);
	readIntPC(input, mediumHS);
	readIntPC(input, hardHS);
	readIntPC(input, currentHS);
	if (gameResult)
	{
		currentHS+=difficulty;
	}
	if (difficulty == 1){
		if (currentHS > easyHS)
		{
			easyHS = currentHS;
		}
	}
	else if (difficulty == 2)
	{
		if (currentHS > mediumHS)
		{
			mediumHS = currentHS;
		}
	}
	else if (difficulty == 3)
	{
		if (currentHS > hardHS)
		{
			hardHS = currentHS;
		}
	}
	lastHS = currentHS;
	if (!gameResult) {
		currentHS = 0;
	}
	closeFilePC(input); // closes the input
	// writeOutput returns a true if it writes correctly
	if (writeOutput(easyHS, mediumHS, hardHS, currentHS))
	{
		// should hear a beep beep for everytime writeToHighscore is called

	} else {

	}
	wait1Msec(200);
}



// self explanatory
bool writeOutput(long easy, long medium, long hard, long current)
{
	TFileHandle output;
	bool isOpen = openWritePC(output, "TTT_High_Scores.txt");
	if (!isOpen)
	{
		playSound(soundException);
		displayString(5, "FAILED TO OPEN FILE");
		wait1Msec(1000);
		return false;
	}
	writeLongPC(output, easy);
	writeEndlPC(output);
	writeLongPC(output, medium);
	writeEndlPC(output);
	writeLongPC(output, hard);
	writeEndlPC(output);
	writeLongPC(output, current);
	writeEndlPC(output);

	closeFilePC(output);
	return true;
}

// creates and empty file, self explanatory
void createEmpty() {
	TFileHandle output;
	bool isOpen = openWritePC(output, "TTT_High_Scores.txt");
	if (!isOpen)
	{
		playSound(soundException);
		displayString(5, "FAILED TO OPEN FILE");
		wait1Msec(1000);
	}
	for (int i=0;i<4;i++)
	{
		writeLongPC(output, 0);
		writeEndlPC(output);
	}
	closeFilePC(output);
}
