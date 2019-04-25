#ifndef ROBOT_POSITION_H
#define ROBOT_POSITION_H

typedef struct robot_struct{	

	double x;
	double y;
	double angle;
	
}robot_info;

//function to be used in movement functions
void update_robot_position(double distance);
//function to be used in turnleft and turn right function
//for turn left, the angle inputted should be positive
//in turn right, the angle inputted should be negative
void update_robot_angle(double angle);
//to be used in object detection function
void get_robot_position(double *x, double *y);
//to be used where robots current angle is needed.
double get_robot_angle();

#endif
