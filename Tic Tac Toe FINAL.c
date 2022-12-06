#include "PC_FileIO.c"

const int ROBOT = 1;
const int PLAYER = 2;

//CODE FOR DISPLAY SCORES
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
	displayString(0, "Easy High Score: %d", easyHS);
	displayString(1, "Medium High Score: %d", mediumHS);
	displayString(2, "Hard High Score: %d", hardHS);
	displayString(3, "Current Score: %d", currentHS);
}

//CODE TO WRITE TO FILE
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

//WRITES TO TTT_High_Scores.txt AFTER SCORE CALCULATIONS
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
		} else {
	}
	wait1Msec(200);
}

//CODE FOR GUI
int GUI()
{
	int loopValue = 0;
	do {
		displayTextLine(2, "Welcome to your personal");
		displayTextLine(3, "Tic-Tic-Toe Friend!");
		displayTextLine(5, "Select Difficulty:");
		displayTextLine(7, "Easy   (left button)");
		displayTextLine(8, "Medium (enter button)");
		displayTextLine(9, "Hard   (right button)");
		displayTextLine(10, "Display Scores (down button)");

		while (!getButtonPress(buttonAny))
		{}


		if(getButtonPress(buttonLeft))
		{
			loopValue = 0;
			eraseDisplay();
			displayTextLine(8, "Difficulty level: EASY");
			wait1Msec(2000);
			eraseDisplay();
			displayTextLine(8, "Good Luck!");
			wait1Msec(2000);
			eraseDisplay();
			wait1Msec(2000);
			eraseDisplay();
			return 1;
		}else if(getButtonPress(buttonEnter))
		{
			loopValue = 0;
			eraseDisplay();
			displayTextLine(8, "Difficulty level: MEDIUM");
			wait1Msec(2000);
			eraseDisplay();
			displayTextLine(8, "Good Luck!");
			wait1Msec(2000);
			eraseDisplay();
			return 2;
		}else if (getButtonPress(buttonRight))
		{
			loopValue = 0;
			eraseDisplay();
			displayTextLine(8, "Difficulty level: HARD");
			wait1Msec(2000);
			eraseDisplay();
			displayTextLine(8, "Good Luck!");
			wait1Msec(2000);
			eraseDisplay();
			wait1Msec(2000);
			eraseDisplay();
			return 3;
		}else if (getButtonPress(buttonDown))
		{
			loopValue = 1;
			eraseDisplay();
			displayScores();
			wait1Msec(6000);
			eraseDisplay();
		}
		else if (getButtonPress(buttonUp))
		{
			eraseDisplay();
			return -1;
		}
	}
	while(loopValue > 0);
	return -1;
}

//Checks if a given player has won a game (any 3 in a row possibility)
bool checkForWin(int *board, int player)
{
	for(int i = 0; i < 3; i++)
	{
		if(board[i*3] == player && board[(i*3)+1] == player && board[(i*3)+2] == player)
			return true;
		if(board[i] == player && board[i+3] == player && board[i+6] == player)
			return true;
	}

	if(board[0] == player && board[4] == player && board[8] == player)
		return true;
	if(board[2] == player && board[4] == player && board[6] == player)
		return true;

	return false;
}

//CODE FOR ALGORITHM

//checks if the board is in a terminal state (a player has won/no more valid moves)
bool isTerminal(int *board)
{
	int openCount = 0;

	for(int i = 0; i < 9; i++)
	{
		if(board[i] == 0)
			openCount++;
	}

	if(checkForWin(board, ROBOT) || checkForWin(board, PLAYER) || openCount == 0)
		return true;

	return false;
}

//gets the scores of each possible move and returns the best
int minimax(int *board, int depth, long alpha, long beta, bool maxing)
{
	int bestScore = 0;

	//checks if there is a terminal state and returns the board value
	if(isTerminal(board))
	{
		if(checkForWin(board, ROBOT))
			return 100-depth;
		else if(checkForWin(board, PLAYER))
			return -100+depth;
		else
			return 0;
	}

	//for each valid move on the board find the score and returns the highest possible score
	if(maxing)
	{
		bestScore = -1000;
		for(int i = 0; i < 9; i++)
		{
			if(board[i] == 0)
			{
				board[i] = ROBOT;
				//calls itself with one higher depth and minimizing
				int score = minimax(board, depth+1, alpha, beta, false);
				board[i] = 0;
				if(score > bestScore)
					bestScore = score;
			}
			//break if the maximum score is greater than or equal to the minimized score
			if(bestScore >= beta)
				i = 10;
			if(bestScore > alpha)
				alpha = bestScore;
		}
		return bestScore;
	}
	//for each valid move on the board find the score and returns the lowest possible score
	else
	{
		bestScore = 1000;
		for(int i = 0; i < 9; i++)
		{
			if(board[i] == 0)
			{
				board[i] = PLAYER;
				//calls itself with one higher depth and maximizing
				int score = minimax(board, depth+1, alpha, beta, true);
				board[i] = 0;
				if(score < bestScore)
					bestScore = score;
			}
			//break if the minimum score is less than or equal to the maximized score
			if(bestScore <= alpha)
				i = 10;
			if(bestScore < beta)
				beta = bestScore;
		}
		return bestScore;
	}

	return 0;
}

//gets the index of the best move using minimax
int bestRobotMove(int *board)
{
	int bestMove = -1;
	int bestScore = -100000;
	//gets the scores of each possible move and selects the highest
	for(int i = 0; i < 9; i++)
	{
		if(board[i] == 0)
		{
			board[i] = ROBOT;
			int score = minimax(board, 0, -1000000000, 1000000000, false);
			board[i] = 0;
			//selects the index with the highest score
			if(score > bestScore)
			{
				bestScore = score;
				bestMove = i;
			}
		}
	}
	return bestMove;
}

//determines the robots move given the difficulty
int robotMove(int *board, int diff)
{
	int moveSelection = -1;
	int openSpots[9];
	int openCounter = 0;

	//gets all valid moves
	for(int i = 0; i < 9; i++)
	{
		if(board[i] == 0)
		{
			openSpots[openCounter] = i;
			openCounter++;
		}
	}

	//always make the best move on hard mode
	if(diff == 3)
	{
		moveSelection = bestRobotMove(board);
	}
	//50/50 for best move or random move on medium mode
	else if(diff == 2)
	{
		if(random(1))
			moveSelection = bestRobotMove(board);
		else
			moveSelection = openSpots[random(openCounter-1)];
	}
	//always make a random move on easy mode
	else if(diff == 1)
	{
		moveSelection = openSpots[random(openCounter-1)];
	}

	return moveSelection;
}

//ROBOT MOVE FUNCTIONS

//brings the two arms to the bottom left most position relitive to the robot
void trackReset()
{
	bool xReset = true;
	bool yReset = true;
	nMotorEncoder(motorA) = 0;
	nMotorEncoder(motorC) = 0;
	motor[motorA] = 25;
	motor[motorC] = 20;
	time1[T1] = 0;
	wait1Msec(200);

	while(xReset || yReset)
	{
		//stop resetting the x-arm if there is resistance
		if(abs(nMotorEncoder(motorA)/(time1(T1)/100)) < 9)
		{
			xReset = false;
			motor[motorA] = 0;
		}
		//stop resetting the y-arm if there is resistance
		if(abs(nMotorEncoder(motorC)/(time1(T1)/100)) < 14)
		{
			yReset = false;
			motor[motorC] = 0;
		}
		time1[T1] = 0;
		nMotorEncoder(motorA) = 0;
		nMotorEncoder(motorC) = 0;
		wait1Msec(100);
	}

	//zero the x and y arm positions
	nMotorEncoder(motorA) = 0;
	nMotorEncoder(motorC) = 0;
}

//brings the pen to its highest position
void penReset()
{
	nMotorEncoder(motorB) = 0;
	motor[motorB] = -30;
	time1[T2] = 0;
	wait1Msec(200);
	//stop resetting the pen if there is resistance
	while(abs(nMotorEncoder(motorB)/(time1(T2)/100)) > 7)
	{}
	motor[motorB] = 0;
	//zero the pen position
	nMotorEncoder(motorB) = 0;
}

//moves a given motor to a given distance at a given speed
void moveMotor(tMotor motorPort, int distance, int speed)
{
	motor[motorPort] = speed;
	while(abs(nMotorEncoder[motorPort]) < distance)
	{}
	motor[motorPort] = 0;
}


void movePenSense(int spot, bool isPen)
{
	//Configurable distances for rows/columns, we are using 390/390 encoder square
	const int PLAYING_LENGTH = 300;
	int distBetweenCenters = 0;
	int distToCenter = 0;
	int firstColumnDist = 0;
	int secondColumnDist = 0;
	int thirdColumnDist = 0;
	int firstRowDist = 0;
	int secondRowDist = 0;
	int thirdRowDist = 0;

	//calculate the distance between the center of each spot
	distBetweenCenters = PLAYING_LENGTH / 3;
	//Calculate distance from an outside line to center of box
	distToCenter = PLAYING_LENGTH / 6;

	//Determine distances to each column/row starting with top left as 0
	firstColumnDist = distToCenter;
	secondColumnDist = distToCenter + distBetweenCenters;
	thirdColumnDist = distToCenter + 2*(distBetweenCenters);
	firstRowDist = distToCenter + 2*(distBetweenCenters);
	secondRowDist = distToCenter + distBetweenCenters;
	thirdRowDist = distToCenter;

	/*
	Create two parallel arrays, for column dist/row dist
	ie spot 1 would have second_column_dist and first_row_dist
	*/
	int moveValuesColumn[9] = {
		firstColumnDist,
		secondColumnDist,
		thirdColumnDist,
		firstColumnDist,
		secondColumnDist,
		thirdColumnDist,
		firstColumnDist,
		secondColumnDist,
		thirdColumnDist
	};
	int moveValuesRow[9] = {
		firstRowDist,
		firstRowDist,
		firstRowDist,
		secondRowDist,
		secondRowDist,
		secondRowDist,
		thirdRowDist,
		thirdRowDist,
		thirdRowDist
	};
	//Adjustment distance for the pen
	const int HORZ_ADJUSTMENT = 170;
	const int VERT_ADJUSTMENT = 0;

	if(isPen)
	{
		//Move to column first
		moveMotor(motorC, moveValuesColumn[spot], -20);
		//Move to row second
		moveMotor(motorA, moveValuesRow[spot], -20);
	}
	else
	{
		moveMotor(motorC, moveValuesColumn[spot] + HORZ_ADJUSTMENT, -20);
		moveMotor(motorA, moveValuesRow[spot] + VERT_ADJUSTMENT, -20);
	}
}

void penDown()
{
	const int PEN_DIST = 30;
	moveMotor(motorB, PEN_DIST, 40);
}

/*
This function draws the playing board for a tic-tac-toe game.
A constant playing length in motor encoder clicks is assumed
to be 390.
*/
void drawBoard()
{
	penReset();
	trackReset();
	const int PLAYING_LENGTH = 300;
	int distBetweenLines = 0;
	distBetweenLines = PLAYING_LENGTH / 3;

	// move to first column line and then draw upwards
	moveMotor(motorC, distBetweenLines, -20);
	penDown();
	wait1Msec(750);
	moveMotor(motorA, PLAYING_LENGTH, -20);
	penReset();
	trackReset();

	//Move to second column and then draw upwards
	moveMotor(motorC, 2*distBetweenLines, -20);
	penDown();
	wait1Msec(750);
	moveMotor(motorA, PLAYING_LENGTH, -20);
	penReset();
	trackReset();

	//Move to first row, draw across column
	moveMotor(motorA, distBetweenLines, -20);
	penDown();
	wait1Msec(750);
	moveMotor(motorC, PLAYING_LENGTH, -20);
	penReset();
	trackReset();

	//Move to second row, draw across column
	moveMotor(motorA, 2*distBetweenLines, -20);
	penDown();
	wait1Msec(750);
	moveMotor(motorC, PLAYING_LENGTH, -20);
	penReset();
	trackReset();
}

task main()
{
	//initilizes sensors
	SensorType[S1] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Color_Reflected;
	wait1Msec(100);
	SensorType[S4] = sensorEV3_Touch;
	wait1Msec(100);


 	bool loopRun = true;

	while(loopRun)
	{
		//initilizes the board
		int board[9] = {0,0,0,
										0,0,0,
										0,0,0};

		int difficulty = GUI();
		if(difficulty == -1)
			loopRun = false;
		else
			drawBoard();

		//loops through the turns
		while(!isTerminal(board) && loopRun)
		{
			int changeCount = 0;
			//player move
			do
				{
				trackReset();
				displayTextLine(8, "Please Make Your Move");
				displayTextLine(9, "Then Press the Button");
				while(SensorValue[S4] == 0)
				{}
				while(SensorValue[S4] == 1)
				{}
				eraseDisplay();
				for(int i = 0; i < 9; i++)
				{
					trackReset();
					if(board[i] == 0)
					{
						movePenSense(i, false);
						wait1Msec(500);
						if(SensorValue[S1] < 25)
						{
							writeDebugStreamLine("Move Found At: %d", i);
							board[i] = PLAYER;
							changeCount++;
						}
					}
				}
			}
			while(changeCount < 1 && loopRun);

			//exits if the player has made too many moves
			if(changeCount > 1)
			{
				displayTextLine(8, "Too Many Moves!");
				displayTextLine(9, "The game will now restart");
				wait1Msec(5000);
				loopRun = false;
			}

			//robot move
			trackReset();
			if(!isTerminal(board) && loopRun)
			{
				int bestMove = robotMove(board, difficulty);
				writeDebugStreamLine("%d", bestMove);
				movePenSense(bestMove, true);
				board[bestMove] = ROBOT;
				penDown();
				wait1Msec(1000);
				penReset();

			}
		}
		//updates highscore file
		writeToHighscore(checkForWin(board, PLAYER), difficulty);
	}
}
