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

int minimax(int *board, int depth, bool isMaximizingPlayer)
{
	int bestVal = -100;
	int value = 0;

	if(is_terminal(board))
	{
		if(check_for_win(board, 1))
			return 10;
		else if(check_for_win(board, 2))
			return -10;
		else
			return 0;
	}
	if(depth == 0)
		return bestVal;

	if(isMaximizingPlayer)
	{
		bestVal = -1000000;
		for(int i = 0; i <= 8; i++)
		{
			value = minimax(board, depth-1, false);
			if(value > bestVal)
				bestVal = value;
			return bestVal;
		}
	}
  else
  {
		bestVal = 1000000;
		for(int i = 0; i <= 8; i++)
		{
			value = minimax(board, depth-1, true);
			if(value < bestVal)
				bestVal = value;
			return bestVal;
		}
	}
	return bestVal;
}

int findBestMove(int *board, int player)
{
    int bestVal = -1000;
    int bestMove = -1;

    for (int i = 0; i <= 8; i++)
    {
    	if (board[i] == 0)
    	{
    		board[i] = player;
    		int moveVal = minimax(board, player, false);
    		board[i] = 0;
				if (moveVal > bestVal)
				{
					bestMove = i;
					bestVal = moveVal;
				}
			}
		}
	return bestMove;
}


task main()
{

	SensorType[S1] = sensorEV3_Color;
  wait1Msec(50);
  SensorMode[S1] = modeEV3Color_Reflected;
  wait1Msec(100);

	int board[9] = {0,0,0,0,0,0,0,2,2};
/*
	int check_board = 0;

	while(check_board <= 8)
	{
		while(!getButtonPress(buttonEnter))
		{}
		while(getButtonPress(buttonEnter))
		{}
		if(SensorValue[S1] < 65)
			board[check_board] = 1;
		check_board++;
	}
*/

	writeDebugStreamLine("%d", findBestMove(board, 1));
}
