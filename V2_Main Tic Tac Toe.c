#include "PC_FileIO.c"

/*Global variables
This variable is used to return whether the Player won or not
If a global variable was not used many functions would need to be changed and it would become very complicated
Nassar said it is ok to use global variable for this
*/

//ALGORITHM FUNCTIONS

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
int TTT_GUI ()
{
	int loop_value = 0;
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
			loop_value = 0;
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
			loop_value = 0;
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
			loop_value = 0;
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
			loop_value = 1;
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
	while(loop_value > 0);
	return -1;
}

//CODE FOR CHECKING FOR A WIN CONDITION
bool check_for_win(int *board, int player)
{
	for(int i = 0; i <= 2; i++)
	{
		if(board[i*3] == player && board[(i*3)+1] == player && board[(i*3)+2] == player)
			return true;
	}
	for(int i = 0; i < 3; i++)
	{
		if(board[i] == player && board[i+3] == player && board[i+6] == player)
			return true;
	}

	if(board[0] == player && board[4] == player && board[8] == player)
		return true;

	else if(board[2] == player && board[4] == player && board[6] == player)
		return true;

	return false;
}

bool is_terminal(int *board)
{
	int open_count = 0;

	for(int i = 0; i < 9; i++)
	{
		if(board[i] == 0)
			open_count++;
	}

	if(check_for_win(board, 1) || check_for_win(board, 2) || open_count == 0)
		return true;

	return false;
}

int minimax(int *board, int depth, long alpha, long beta, bool maxing)
{
	int best_score = 0;

	if(is_terminal(board))
	{
		if(check_for_win(board, 1))
			return 100-depth;
		else if(check_for_win(board, 2))
			return -100+depth;
		else
			return 0;
	}

	if(maxing)
	{
		best_score = -1000;
		for(int i = 0; i < 9; i++)
		{
			if(board[i] == 0)
			{
				board[i] = 1;
				int score = minimax(board, depth+1, alpha, beta, false);
				board[i] = 0;
				if(score > best_score)
					best_score = score;
			}
			if(best_score >= beta)
				break;
			if(best_score > alpha)
				alpha = best_score;
		}
		return best_score;
	}
	else
	{
		best_score = 1000;
		for(int i = 0; i < 9; i++)
		{
			if(board[i] == 0)
			{
				board[i] = 2;
				int score = minimax(board, depth+1, alpha, beta, true);
				board[i] = 0;
				if(score < best_score)
					best_score = score;
			}
			if(best_score <= alpha)
				break;
			if(best_score < beta)
				beta = best_score;
		}
		return best_score;
	}

	return 0;
}

int best_robot_move(int *board)
{
	int best_move = -1;
	int best_score = -100000;
	for(int i = 0; i < 9; i++)
	{
		if(board[i] == 0)
		{
			board[i] = 1;
			int score = minimax(board, 0, -1000000000, 1000000000, false);
			board[i] = 0;
			if(score > best_score)
			{
				best_score = score;
				best_move = i;
			}
		}
	}
	return best_move;
}

int robot_move(int *board, int diff)
{
	int move_selection = -1;
	int open_spots[9];
	int open_counter = 0;
	for(int i = 0; i < 9; i++)
	{
		if(board[i] == 0)
		{
			open_spots[open_counter] = i;
			open_counter++;
		}
	}
	if(diff == 3)
	{
		move_selection = best_robot_move(board);
	}
	else if(diff == 2)
	{
		if(random(1))
			move_selection = best_robot_move(board);
		else
		{
			move_selection = open_spots[random(open_counter-1)];

		}
	}
	else if(diff == 1)
	{
		move_selection = open_spots[random(open_counter-1)];
	}

	return move_selection;
}

//ROBOT MOVE FUNCTIONS
void track_reset()
{
	bool x_reset = true;
	bool y_reset = true;
	nMotorEncoder(motorA) = 0;
	nMotorEncoder(motorC) = 0;
	motor[motorA] = 25;
	motor[motorC] = 20;
	time1[T1] = 0;
	wait1Msec(200);

	while(x_reset || y_reset)
	{
		if(abs(nMotorEncoder(motorA)/(time1(T1)/100)) < 9)
		{
			x_reset = false;
			motor[motorA] = 0;
		}

		if(abs(nMotorEncoder(motorC)/(time1(T1)/100)) < 14)
		{
			y_reset = false;
			motor[motorC] = 0;
		}
		time1[T1] = 0;
		nMotorEncoder(motorA) = 0;
		nMotorEncoder(motorC) = 0;
		wait1Msec(100);
	}
	nMotorEncoder(motorA) = 0;
	nMotorEncoder(motorC) = 0;
}

void pen_reset()
{
	nMotorEncoder(motorB) = 0;
	motor[motorB] = -30;
	time1[T2] = 0;
	wait1Msec(200);
	while(abs(nMotorEncoder(motorB)/(time1(T2)/100)) > 7)
	{}
	motor[motorB] = 0;
	nMotorEncoder(motorB) = 0;
}

void move_motor(tMotor motor_port, int distance, int speed)
{
	motor[motor_port] = speed;
	while(abs(nMotorEncoder[motor_port]) < distance)
	{}
	motor[motor_port] = 0;
}


void movePenSense(int spot, bool isPen)
{
	//Configurable distances for rows/columns, we are using 390/390 encoder square
	const int PLAYING_LENGTH = 300;
	int dist_between_centers = 0;
	int dist_to_center = 0;
	int first_column_dist = 0;
	int second_column_dist = 0;
	int third_column_dist = 0;
	int first_row_dist = 0;
	int second_row_dist = 0;
	int third_row_dist = 0;

	//calculate the distance between the center of each spot
	dist_between_centers = PLAYING_LENGTH/3;
	//Calculate distance from an outside line to center of box
	dist_to_center = PLAYING_LENGTH/6;

	//Determine distances to each column/row starting with top left as 0
	first_column_dist = dist_to_center;
	second_column_dist = dist_to_center + dist_between_centers;
	third_column_dist = dist_to_center + 2*(dist_between_centers);
	first_row_dist = dist_to_center + 2*(dist_between_centers);
	second_row_dist = dist_to_center + dist_between_centers;
	third_row_dist = dist_to_center;

	/*
	Create two parallel arrays, for column dist/row dist
	ie spot 1 would have second_column_dist and first_row_dist
	*/
	int moveValuesColumn[9] = {
		first_column_dist,
		second_column_dist,
		third_column_dist,
		first_column_dist,
		second_column_dist,
		third_column_dist,
		first_column_dist,
		second_column_dist,
		third_column_dist
	};
	int moveValuesRow[9] = {
		first_row_dist,
		first_row_dist,
		first_row_dist,
		second_row_dist,
		second_row_dist,
		second_row_dist,
		third_row_dist,
		third_row_dist,
		third_row_dist
	};
	//Adjustment distance for the pen
	const int HORZ_ADJUSTMENT = 170;
	const int VERT_ADJUSTMENT = 0;

	if(isPen)
	{
		//Move to column first
		move_motor(motorC, moveValuesColumn[spot], -20);
		//Move to row second
		move_motor(motorA, moveValuesRow[spot], -20);
	}
	else
	{
		move_motor(motorC, moveValuesColumn[spot] + HORZ_ADJUSTMENT, -20);
		move_motor(motorA, moveValuesRow[spot] + VERT_ADJUSTMENT, -20);
	}
}

void pen_down()
{
	const int PEN_DIST = 30;
	move_motor(motorB, PEN_DIST, 40);
}

/*
This function draws the playing board for a tic-tac-toe game.
A constant playing length in motor encoder clicks is assumed
to be 390.
*/
void draw_board()
{
	pen_reset();
	track_reset();
	const int PLAYING_LENGTH = 300;
	int dist_between_lines = 0;
	dist_between_lines = PLAYING_LENGTH/3;

	// move to first column line and then draw upwards
	move_motor(motorC, dist_between_lines, -20);
	pen_down();
	wait1Msec(750);
	move_motor(motorA, PLAYING_LENGTH, -20);
	pen_reset();
	track_reset();

	//Move to second column and then draw upwards
	move_motor(motorC, 2*dist_between_lines, -20);
	pen_down();
	wait1Msec(750);
	move_motor(motorA, PLAYING_LENGTH, -20);
	pen_reset();
	track_reset();

	//Move to first row, draw across column
	move_motor(motorA, dist_between_lines, -20);
	pen_down();
	wait1Msec(750);
	move_motor(motorC, PLAYING_LENGTH, -20);
	pen_reset();
	track_reset();

	//Move to second row, draw across column
	move_motor(motorA, 2*dist_between_lines, -20);
	pen_down();
	wait1Msec(750);
	move_motor(motorC, PLAYING_LENGTH, -20);
	pen_reset();
	track_reset();
}

task main()
{
	SensorType[S1] = sensorEV3_Color;
	wait1Msec(50);
	SensorMode[S1] = modeEV3Color_Reflected;
	wait1Msec(100);
	SensorType[S4] = sensorEV3_Touch;
	wait1Msec(100);

 	bool loop_run = true;

	while(loop_run)
	{
		int board[9] = {0,0,0,
			0,0,0,
			0,0,0};
		int difficulty = TTT_GUI();

		if(difficulty == -1)
			loop_run = false;
		else
			draw_board();

		while(!is_terminal(board) && loop_run)
		{
			int change_count = 0;
			do
				{
				track_reset();
				displayTextLine(8, "Please Make Your Move");
				displayTextLine(9, "Then Press the Button");
				while(SensorValue[S4] == 0)
				{}
				while(SensorValue[S4] == 1)
				{}
				eraseDisplay();
				for(int i = 0; i < 9; i++)
				{
					track_reset();
					if(board[i] == 0)
					{
						movePenSense(i, false);
						wait1Msec(500);
						if(SensorValue[S1] < 25)
						{
							writeDebugStreamLine("Move Found At: %d", i);
							board[i] = 2;
							change_count++;
						}
					}
				}
			}
			while(change_count < 1 && loop_run);

			if(change_count > 1)
			{
				displayTextLine(8, "Too Many Moves!");
				displayTextLine(9, "The game will now restart");
				wait1Msec(5000);
				break;
			}

			track_reset();

			if(!is_terminal(board) && loop_run)
			{
				int best_move = robot_move(board, difficulty);
				writeDebugStreamLine("%d", best_move);
				movePenSense(best_move, true);
				board[best_move] = 1;
				pen_down();
				wait1Msec(1000);
				pen_reset();

			}
		}
		writeToHighscore(check_for_win(board,2), difficulty);
	}
}
