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
		const int PLAYING_LENGTH = 390;
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
		const int ADJUSTMENT = 690;
		writeDebugStreamLine("spot %d has column dist: %d", spot, moveValuesColumn[spot]);
		writeDebugStreamLine("spot %d has row dist: %d", spot, moveValuesRow[spot]);
		if(isPen)
		{
			//Move to column first
			move_motor(motorC, moveValuesColumn[spot], -20);
			//Move to row second
			move_motor(motorA, moveValuesRow[spot], 20);
		}
		else
		{
			move_motor(motorC, moveValuesColumn[spot] + ADJUSTMENT, -20);
			move_motor(motorA, moveValuesRow[spot], 20);
		}
}

void track_reset()
{
	bool x_reset = true;
	bool y_reset = true;
	nMotorEncoder(motorA) = 0;
	nMotorEncoder(motorC) = 0;
	motor[motorA] = -10;
	motor[motorC] = 10;
	time1[T1] = 0;
	wait1Msec(500);
	while(x_reset || y_reset)
	{
		writeDebugStreamLine("%f", abs(nMotorEncoder(motorA)/(time1[T1]/100)));
		writeDebugStreamLine("%f", abs(nMotorEncoder(motorC)/(time1[T1]/100)));

		if(abs(nMotorEncoder(motorA)/(time1(T1)/100)) < 7)
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

task main()
{
		track_reset();
		movePenSense(0, true);
}
