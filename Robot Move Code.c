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
	for(int i = 0; i < 9; i++)
	{
		track_reset();
		move_spot(i);
	}
	/*
	for(int i = 1; i <=3; i++)
	{
		move_motor(motorA, 160*i, 30);
		for(int j = 1; j <= 3; j++)
		{
			move_motor(motorC, 130, -15);
			nMotorEncoder[motorC] = 0;
		}
		track_reset();
		nMotorEncoder[motorA] = 0;
	}
	*/
}
