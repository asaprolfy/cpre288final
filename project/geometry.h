#ifndef GEOMETRY_H
#define GEOMETRY_H


//finds midpoint between 2 points
void midpoint(double *x, double *y, double x1, double y1, double x2, double y2);
//finds a line perpindicular between 2 points
void line_from_midpoint(double *slope, double *intercept, double x1, double y1, double x2, double y2);
//finds the closest point from the robots starting position to the robot
void point_from_robot(double slope, double intercept, double *x, double *y);
//finds the position of an object using the angle and distance 
void object_position(double angle, double distance, double *x_obj, double *y_obj);
//return the distance between two points
double distance_between_points(double x1, double y1, double x2, double y2);
//gets the commands for the robot once everything has been calculated
void get_commands(int *direction, double *angle, double *a_distance, double *b_distance, double mid_x, double mid_y, double x, double y);
//combines functions line from mid and point from robot
void get_triangle(double *point_x, double *point_y, double x1, double y1, double x2, double y2);
//returns the minimum angle the robot would have to turn to avoid an object;
double min_angle(double object_width, double object_distance);
//returns 1 if the robot sees the object, 0 if it doesnt.
int in_view(double x, double y);
//drives the robot to a given x,y point.
void drive_to_point(int *direction, double *length, double *angle, double x, double y);
//finds a point near the object, used to drive around an object
void point_near_object(double *x, double *y, double x_obj, double y_obj, double width, int side);

#endif
