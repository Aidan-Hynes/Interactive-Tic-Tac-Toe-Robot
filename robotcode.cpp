void draw(int horiSpeed, double gridSize, int penDown)
{
	//figure out how much to draw
	double drawDist = (gridSize / 2) - (gridSize / 4);
	double const ENC_LIMIT = drawDist * 180 / (PI * 2.75);
	nMotorEncoder[motorA] = 0;
	
	/*horiSpeed is horizontal speed, penDown is sharpie going down speed
	assuming pen starts middle, moves right with pen down. Once it reaches
	calculated distance, pen up and horizontal speed to zero*/
	motor[motorA] = horiSpeed;
	motor[motorB] = penDown;
	while(nMotorEncoder[motorA] < ENC_LIMIT)
	{}
	//once reaches end, stops
	motor[motorA] = 0;
	motor[motorB] = -penDown;
	
	//need to move pen back to middle for the other side
	motor[motorA] = -horiSpeed;
	while(nMotorEncoder[motorA] > 0)
	{}
	motor[motorA] = 0;
	
	//draws other side of straight line
	motor[motorA] = -horiSpeed;
	motor[motorB] = penDown;
	while(nMotorEncoder[motorA] > -1 * ENC_LIMIT)
	{}
	//once reaches end, stops
	motor[motorA] = 0;
	motor[motorB] = -penDown;
	wait1Msec(3000);
	//return to start function goes here
}
