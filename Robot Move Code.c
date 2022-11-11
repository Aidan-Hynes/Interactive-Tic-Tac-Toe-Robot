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
}
