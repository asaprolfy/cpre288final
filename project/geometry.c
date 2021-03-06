#include <stdio.h>
#include <math.h>
#include "geometry.h"
#include "robot_position.h"

//Global variables for robot_position
double robot_x;
double robot_y;

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
void point_from_robot(double slope, double intercept, double *x, double *y){

    get_robot_position(&robot_x, &robot_y);

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
void object_position(double angle, double distance, double *x_obj, double *y_obj){
    get_robot_position(&robot_x, &robot_y);

    *x_obj = distance*cos(angle*M_PI/180)+ robot_x;
    *y_obj = distance*sin(angle*M_PI/180)+ robot_y;
    return;
}
/*
* Given an robots starting position, and midpoint of objects return the final point of the triangle
* combines line from mid and point from robot function.
* @Author: Brandon Johnson
* @Date: 4/14/2019
*/
void get_triangle(double *point_x, double *point_y, double x1, double y1, double x2, double y2){
    get_robot_position(&robot_x, &robot_y);

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
void get_commands(int *direction, double *angle, double *a_distance, double *b_distance, double mid_x, double mid_y, double x, double y){
    //get robot info
    double robot_angle = get_robot_angle();
    get_robot_position(&robot_x, &robot_y);

    //finds the missing sides of the triangle
    *a_distance = distance_between_points(robot_x, robot_y, x, y);
    *b_distance = distance_between_points(mid_x, mid_y, x, y);

    //find points x,y position relative to the robot
    x = x-robot_x;
    y = y-robot_y;

    //find angle of the point
    double other_angle = atan2(y,x)*180/M_PI;
    //quad1
    /*
    if(x > 0 && y > 0){
        other_angle += 0;
    }
    //quad2
    else if(x < 0 && y > 0){
        other_angle += 180;
    }
    //quad3
    else if(x < 0 && y < 0){
        other_angle += 180;
    }
    //quad4
    else if(x > 0 && y < 0){
        other_angle += 360;
    }
    */
    //atan returns negatives, negatives are bad as we only want positive angles
   /*
    if(other_angle < 0){
        other_angle += 360;
    }
*/
    //determine if the robot needs to turn right or left and calculate angle
    //absolute value because direction takes care of negatives.
    //other_angle = ((int)other_angle + 360) % 360;

    if(other_angle < 0){
        other_angle = abs(other_angle)+180;
    }

    if(robot_angle >= other_angle){
        *direction = 1;
        *angle = abs(robot_angle-other_angle);
    }
    else{
        *direction = 0;
        *angle = abs(other_angle-robot_angle);
    }

}

/*
* Given the robots distance to an object and its width, this function
* returns the minimum angle the robot needs to turn to avoid it.
* @Author: Brandon Johnson
* @Date: 4/18/2019
*/
double min_angle(double object_width, double object_distance){
    //object width/2 = radius, adding the robot radius +5 cm to be safe
	double angle = atan2((object_width/2+5+16.5),object_distance)*180/M_PI;
	//mapping atan2 to be 0-180 degrees.
    angle = ((int)angle + 360) % 360;
	return angle;
}

/*
* Finds a the nearest point to an object that we would like the robot to drive around
* this will be used in our drive around functio.
* side = 1 means drive to the right, side = 0 means drive to the left
* @Author: Brandon Johnson
* @Date: 5/2/2019
*/
void point_near_object(double *x, double *y, double x_obj, double y_obj, double width, int side){
	//find the angle of the object on our coord plane
	double obj_angle = atan2(y_obj, x_obj)*180/M_PI;
	obj_angle = (int)(obj_angle + 360) % 360;
	printf("%.1lf\n", obj_angle);
	double distance = distance_between_points(x_obj, y_obj, 0, 0);
	
	//find how much we should turn relative to the obj angle
	double turn_angle = min_angle(width, distance);
	
	if(side == 1){
		obj_angle = obj_angle - turn_angle; 
	}
	else{
		obj_angle = turn_angle + obj_angle;		
	}
	printf("obj_angle: %.1lf\n", obj_angle);
	//find coord of the point.
	*x = distance*cos(obj_angle*M_PI/180);
	*y = distance*sin(obj_angle*M_PI/180);
	
	return;
}
/*
* gets the commands needed to drive around an object.
* returns 1 if you need to turn right, returns 0 if left.
* @Author: Brandon Johnson
* @Date: 5/2/2019
*/
void drive_to_point(int *direction, double *length, double *angle, double x, double y){
	get_robot_position(&robot_x, &robot_y);
	double robot_angle = get_robot_angle();
	
	//finding the distance the robot needs to drive
	*length = distance_between_points(robot_x, robot_y, x, y);
	
	//finding the position relative to the robot
	//x = x-robot_x;
    //y = y-robot_y;
	
	//finding the angle and mapping it 0-360 degrees.
	*angle = atan2(y,x)*180/M_PI;
	*angle = ((int)*angle + 360) % 360;
	
	//direction is 1 if it needs to turn right
	//if 0 it needs to turn left
	printf("%.1lf\n", *angle);
	
	if(robot_angle > *angle){
		*direction = 1;
		*angle = robot_angle - *angle;
	}
	else{
		*direction = 0;
		*angle = *angle - robot_angle;
		
	}	
	
	return;
}


/*
* Given an objects x and y positions and the angle the robot is facing, returns wheter the object can be seen by the robot
* @Author: Nicholas Wolf
* @Date: 4/19/2019
*/
int in_view(double x, double y){
    get_robot_position(&robot_x, &robot_y);
    double angle = get_robot_angle();
    int seen;
    double side;
    double x_mag, x1, x2;
    double y_mag, y1, y2;

    //calculates the magnitude of the x and y distance between a object and the robot
    x_mag = abs(x-robot_x + 10);
    y_mag = abs(y-robot_y + 10);

    //calculates the (x,y) positions of two points that are perpendicular to the current angle of the robot
    x1 = x_mag*cos((angle+90)*M_PI/180)+robot_x;
    x2 = x_mag*cos((angle-90)*M_PI/180)+robot_x;
    y1 = y_mag*sin((angle+90)*M_PI/180)+robot_y;
    y2 = y_mag*sin((angle-90)*M_PI/180)+robot_y;

    //calculates the value side, if side>0, then object is not detected and vice versa
    side = (x-x1)*(y2-y1)-(y-y1)*(x2-x1);

    //sets whether the object is seen or not
    if(side<0){
        seen = 1;
    }
    else{
        seen = 0;
    }

    return seen;
}
