
#include <stdbool.h>
#include "geometry.h"
#include "movement.h"
#include "robot_position.h"

/*
* I am going to make all the return types for driving functions a 
* Boolean, this is to show whether each driving function has been 
* completed succesfully.
*/

/*
* drives through two given objects in a safe way
* still need to have the function return 
* false if not completed all the way
* @ Author: Brandon Johnson
* @ Date: 4/28/2019
*/
bool drive_through_objects(double x1, double y1, double x2, double y2){
		
	double mid_x, mid_y, point_x, point_y;
	double drive_angle, distance1, distance2;
	int direction;
	
	midpoint(&mid_x, &mid_y, x1, y1, x2, y2);
	get_triangle(&point_x, &point_y, x1, y1, x2, y2);
	get_commands(&direction, &drive_angle, &distance1 ,&distance2, mid_x, mid_y, point_x, point_y);
	
	if(direction){
	
		turn_right(/*sensor data*/, drive_angle);
		move_forward(/*sensor data*/, distance1);
		turn_left(/*sensor data*/, 90);	
		//moving an additional 5 so objects arent scanned again
		move_forward(/*sensor data*/, distance1+5);
	}
	else{
		turn_right(/*sensor data*/, drive_angle);
		move_forward(/*sensor data*/, distance1);
		turn_left(/*sensor data*/, 90);	
		//moving an additional 5 so objects arent scanned again
		move_forward(/*sensor data*/, distance1+5);
	}
	
	return true;
}


