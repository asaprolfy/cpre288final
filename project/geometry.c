#include <stdio.h>
#include <math.h>
#include "geometry.h"


/*
* Given two points x and y, this function returns the midpoint (x,y)
* @Author: Brandon Johnson
* @Date: 4/13/2019
*/
void midpoint(double *x, double *y, double x1, double y1, double x2, double y2){
	*x = (x1+x2)/2;
	*y = (y1+y2)/2;
}

/*
* Given a line and a point, this function finds the point closest to the object.
* @Author: Brandon Johnson
* @Date: 4/13/2019
*/
void point_from_robot(double slope, double intercept, double robot_x, double robot_y, double *x, double *y){
	
	double m, b;
	
	if(slope == 0){
		m = 0;
	}
	else{
		m = -1*(1/slope);
	}
	
	b = robot_y - m*robot_x;
	
	*x = (b-intercept)/(m-slope);
	
	*y = m*(*x)-b;	
	
	*x*=-1;
	*y*=-1;
}

/*
* Given two points x and y, this function creates a line perpindicular from the midpoint.
* @Author: Brandon Johnson
* @Date: 4/13/2019
*/
void line_from_midpoint(double *slope, double *intercept, double x1, double y1, double x2, double y2){
	double x_mid, y_mid;
	double m = (y2-y1)/(x2-x1);

	if(m == 0){
		*slope = 0;
	}
	else{
		*slope = -1*(1/m);
	}
	
	midpoint(&x_mid, &y_mid, x1, y1, x2, y2);
	
	*intercept = y_mid - *slope*x_mid;
}	

/*
* Given an robots starting position, distance and angle to an object, return the new  position.
* the first 2 arguments are the robots xy position.
* @Author: Brandon Johnson
* @Date: 4/13/2019
*/
void object_position(double x, double y, double angle, double distance, double *x_obj, double *y_obj){	
	*x_obj = distance*cos(angle*M_PI/180)+ x;
	*y_obj = distance*sin(angle*M_PI/180)+ y;
	return;	
}
/*
* Given an robots starting position, and midpoint of objects return the final point of the triangle
* combines line from mid and point from robot function.
* @Author: Brandon Johnson
* @Date: 4/14/2019
*/
void get_triangle(double *point_x, double *point_y, double robot_x, double robot_y, double x1, double y1, double x2, double y2){
	double slope1, intercept1, slope2, intercept2, x_mid, y_mid;
	//slope of line between both objects
	slope1 = (y2-y1)/(x2-x1);
	//finding the slope of perpindicular to that line
	if(slope1 == 0){
		//making sure slope isn't infinite
		slope2 = 10000;
	}
	else{
		slope2 = -1*(1/slope1);
	}
	
	midpoint(&x_mid, &y_mid, x1, y1, x2, y2);
	
	intercept1 = robot_y - slope1*robot_x;
	
	intercept2 = y_mid - slope2*x_mid;
	
	*point_x = (intercept1-intercept2)/(slope1-slope2);
	
	*point_y = slope1*(*point_x)-intercept1;	
			
	*point_x*=-1;
	*point_y*=-1;
}


/*
* Given two points x and y, this function returns the midpoint (x,y)
* @Author: Brandon Johnson
* @Date: 4/12/2019
*/
double distance_between_points(double x1, double y1, double x2, double y2){
	return sqrt(pow(x2-x1,2)+pow(y2-y1,2));
}

/*
* Given the midpoint position, robot position, and calculated positon,
* get the commands for angle to turn, A distance, and B distance.
* A distance is the small side of the triangle
* B distance is the larger side of the triangle
* @Author: Brandon Johnson
* @Date: 4/12/2019
*/
void get_commands(double *angle, double *a_distance, double *b_distance, double robot_x, double robot_y, double mid_x, double mid_y, double x, double y){
	//finds the missing sides of the triangle
	*a_distance = distance_between_points(robot_x, robot_y, x, y);
	*b_distance = distance_between_points(mid_x, mid_y, x, y);
	//finds angle from midpoint to object
	*angle = atan((*b_distance/(*a_distance)))*180/M_PI;
}
