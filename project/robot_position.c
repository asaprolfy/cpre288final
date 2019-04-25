#include <stdio.h>
#include <math.h>
#include "robot_position.h"

//initializing initial angle as 90 because a move forward will
//move the robot along the y-axis.
robot_info robot = {.x = 0, .y = 0, .angle = 90}; 

/*
* Using the robots current info and a distance, this function updates the robots position
* @Author: Brandon Johnson
* @Date: 4/18/2019
*/
void update_robot_position(double distance){	
	
	robot.x = distance*cos(robot.angle*M_PI/180)+ robot.x;
	robot.y = distance*sin(robot.angle*M_PI/180)+ robot.y;
	return;	
}

/*
* updates the robots current orientation given an angle to turn.
* @Author: Brandon Johnson
* @Date: 4/18/2019
*/
void update_robot_angle(double angle){
	robot.angle = robot.angle + angle;
	//keeping robot between 0 and 360 degrees
	if(robot.angle >= 360){
		robot.angle -= 360;
	}
	if(robot.angle < 0){
		robot.angle += 360;
	}
	
	return;
}

/*
* returns the objects current x,y position.
* @Author: Brandon Johnson
* @Date: 4/18/2019
*/
void get_robot_position(double *x, double *y){
	*x = robot.x;
	*y = robot.y;
	return;	
}

/*
* returns the robots current angle relative to its initial angle
* @Author: Brandon Johnson
* @Date: 4/18/2019
*/
double get_robot_angle(){
	return robot.angle; 	
}
