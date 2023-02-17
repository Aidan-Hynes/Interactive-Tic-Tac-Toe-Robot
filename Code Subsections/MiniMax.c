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

task main()
{

	SensorType[S1] = sensorEV3_Color;
  wait1Msec(50);
  SensorMode[S1] = modeEV3Color_Reflected;
  wait1Msec(100);

	int best_move = -1;
	int best_score = -100000;

	int board[9] = {0,0,0,0,0,0,0,0,0};

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
	writeDebugStreamLine("%d", best_move);
}
