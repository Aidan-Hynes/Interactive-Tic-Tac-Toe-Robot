//ALGORITHM FUNCTIONS

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
	motor[motorA] = 20;
	motor[motorC] = 15;
	time1[T1] = 0;
	wait1Msec(200);

	while(x_reset || y_reset)
	{
		if(abs(nMotorEncoder(motorA)/(time1(T1)/100)) < 10)
		{
			x_reset = false;
			motor[motorA] = 0;
		}

		if(abs(nMotorEncoder(motorC)/(time1(T1)/100)) < 15)
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

	int board[9] = {0,0,0,
									0,0,0,
									0,0,0};


	draw_board();

	while(!is_terminal(board))
	{
		int board_same = true;
		do
		{
			track_reset();
			displayTextLine(8, "Please Make Your Move and Press the Button");
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
						board_same = false;
					}
				}
			}
		}
		while(board_same);
		track_reset();
		if(!is_terminal(board))
		{
			int best_move = robot_move(board, 1);
			writeDebugStreamLine("%d", best_move);
			movePenSense(best_move, true);
			board[best_move] = 1;
			pen_down();
			wait1Msec(1000);
			pen_reset();
		}
	}
}
