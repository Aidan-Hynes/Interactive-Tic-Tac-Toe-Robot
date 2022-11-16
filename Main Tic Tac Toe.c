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
		if(board[i] != 0)
			open_count++;
	}

	if(check_for_win(board, 1) || check_for_win(board, 2) || open_count == 0)
		return true;

	return false;
}

int minimax(int *board, int depth, bool maxing)
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
				int score = minimax(board, depth+1, false);
				board[i] = 0;
				if(score > best_score)
					best_score = score;
			}
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
				int score = minimax(board, depth+1, true);
				board[i] = 0;
				if(score < best_score)
					best_score = score;
			}
		}
		return best_score;
	}
	return 0;
}

void track_reset()
{
	bool x_reset = true;
	bool y_reset = true;
	nMotorEncoder(motorA) = 0;
	nMotorEncoder(motorC) = 0;
	motor[motorA] = -20;
	motor[motorC] = 15;
	time1[T1] = 0;
	wait1Msec(200);
	while(x_reset || y_reset)
	{
		writeDebugStreamLine("%f", abs(nMotorEncoder(motorA)/(time1[T1]/100)));
		writeDebugStreamLine("%f", abs(nMotorEncoder(motorC)/(time1[T1]/100)));

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

void move_motor(tMotor motor_port, int distance, int speed)
{
	motor[motor_port] = speed;
	while(abs(nMotorEncoder[motor_port]) < distance)
	{}
	motor[motor_port] = 0;
}

void move_spot(int index_num)
{
	const int move_array[9][2] = {{0,480},{195,480},{390,480},{0,240},{195,240},{390,240},{0,0},{195,0},{390,0}};
	move_motor(motorC, move_array[index_num][0], -20);
	move_motor(motorA, move_array[index_num][1], 30);
}

task main()
{

	SensorType[S1] = sensorEV3_Color;
  wait1Msec(50);
  SensorMode[S1] = modeEV3Color_Reflected;
  wait1Msec(100);

	int best_move = -1;
	int best_score = -100000;

	int board[9] = {0,1,0,
									0,0,0,
									2,2,0};

	for(int i = 0; i < 9; i++)
	{
		writeDebugStreamLine("MOVE CHECK %d", i);
		if(board[i] == 0)
		{
			writeDebugStreamLine("MOVE EX %d", i);
			board[i] = 1;
			int score = minimax(board, 0, false);
			board[i] = 0;
			writeDebugStreamLine("MOVE SCORE %d", score);
			writeDebugStreamLine("BEST SCORE %d", best_score);
			if(score > best_score)
			{
				best_score = score;
				best_move = i;
				writeDebugStreamLine("REPLACE %d", best_move);
			}
		}
	}
	track_reset();
	move_spot(best_move);
}
