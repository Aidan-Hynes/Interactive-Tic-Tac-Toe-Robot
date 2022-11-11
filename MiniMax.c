void get_open_space(int *board, int *open_spaces)
{
	int array_index = 0;
	for(int i = 0; i <= 8; i++)
	{
		if(board[i] == 0)
		{
			open_spaces[array_index] = i;
			array_index++;
		}
	}
}

bool check_for_win(int *board, int player)
{
	for(int i = 0; i <= 2; i++)
	{
		if(board[i*3] == player && board[(i*3)+1] == player && board[(i*3)+2] == player)
			return true;
	}
	for(int i = 0; i <= 2; i++)
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

bool is_terminal(int *board, int *open_spaces)
{
	int non_zero_count = 0;

	for(int i = 0; i <= 8; i++)
	{
		if(open_spaces[i] != 0)
			non_zero_count++;
	}

	if(check_for_win(board,1) || check_for_win(board,2) || non_zero_count == 0)
		return true;

	return false;
}

int score_calc(int *window, int turn)
{
	int piece_count = 0;
	int open_count = 0;
	for(int i = 0; i <= 2; i++)
	{
		if(window[i] == turn)
			piece_count++;
		else if(window[i] == 0)
			open_count++;
	}

	int score = 0;

	if(piece_count == 3)
		score += 100;
	else if(piece_count == 2 && open_count == 1)
		score += 10;
	else if(piece_count == 1 && open_count == 2)
		score += 2;

	return score;
}

int position_score(int *board, int piece, int turn)
{
	int temp_board[9] = {0,0,0,0,0,0,0,0,0};
	int score = 0;
	int window[3] = {0,0,0};

	for(int i = 0; i <= 8; i++)
		temp_board[i] = board[i];

	temp_board[piece] = turn;
	writeDebugStreamLine("Piece: %d", piece);

	for(int i = 0; i <= 2; i++)
	{
		window[0] = temp_board[i*3];
		window[1] = temp_board[(i*3)+1];
		window[2] = temp_board[(i*3)+2];
		if(i*3 == piece || (i*3)+1 == piece || (i*3)+2 == piece)
				score += score_calc(window, turn);
	}

		for(int i = 0; i <= 2; i++)
	{
		window[0] = temp_board[i];
		window[1] = temp_board[i+3];
		window[2] = temp_board[i+6];
		if(i == piece || i+3 == piece || i+6 == piece)
				score += score_calc(window, turn);
	}

	window[0] = temp_board[0];
	window[1] = temp_board[4];
	window[2] = temp_board[8];
	if(0 == piece || 4 == piece || 8 == piece)
		score += score_calc(window, turn);

	window[0] = temp_board[2];
	window[1] = temp_board[4];
	window[2] = temp_board[6];
	if(2 == piece || 4 == piece || 6 == piece)
		score += score_calc(window, turn);

	return score;
}

int minimax(*board, depth, isMaximizingPlayer)
{
	if(is_ter
}


task main()
{

	SensorType[S1] = sensorEV3_Color;
  wait1Msec(50);
  SensorMode[S1] = modeEV3Color_Reflected;
  wait1Msec(100);

	int board[9] = {1,1,0,0,0,0,0,0,0};
	int open_spaces[9] = {-1,-1,-1,-1,-1,-1,-1,-1,-1};

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

	get_open_space(board, open_spaces);

	for(int i = 0; i <= 8; i++)
		writeDebugStreamLine("%d", board[i]);


	writeDebugStreamLine("--------------");
	for(int i = 0; i <= 8; i++)
		if(open_spaces[i] != -1)
			writeDebugStreamLine("Score: %d", position_score(board, open_spaces[i], 1));

	writeDebugStreamLine("--------------");

	for(int i = 0; i <= 8; i++)
	{
		if(open_spaces[i] != -1)
			writeDebugStreamLine("%d", open_spaces[i]);
	}

	writeDebugStreamLine(is_terminal(board, open_spaces) ? "True" : "False");
	writeDebugStreamLine(check_for_win(board,1) ? "True" : "False");
	writeDebugStreamLine(check_for_win(board,2) ? "True" : "False");

}
