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


int minimax(int *board, int depth, bool maxing, int &best_move)
{
	int best_score = 0;
	if(is_terminal(board))
	{
		if(check_for_win(board, 1))
			return 1;
		else if(check_for_win(board, 2))
			return -1;
		else
			return 0;
	}

	if(maxing)
	{
		best_score = -100000000;
		for(int i = 0; i < 9; i++)
		{
			if(board[i] == 0)
			{
				board[i] = 1;
				int score = minimax(board, depth+1, false, best_move);
				board[i] = 0;
				if(score-depth > best_score)
				{
					best_score = score;
					best_move = i;
				}
			}
		}
		return best_score;
	}
	else
	{
		best_score = 100000000;
		for(int i = 0; i < 9; i++)
		{
			if(board[i] == 0)
			{
				board[i] = 2;
				int score = minimax(board, depth+1, true, best_move);
				board[i] = 0;
				if(score-depth < best_score)
				{
					best_score = score;
					best_move = i;
				}
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
	int board[9] = {2,2,0,0,0,0,0,1,1};
	writeDebugStreamLine("%d",minimax(board, 0, true, best_move));
	writeDebugStreamLine("%d", best_move);
}
