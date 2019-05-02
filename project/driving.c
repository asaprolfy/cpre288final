
#include <stdbool.h>
#include "lcd.h"
#include "geometry.h"
#include "movement.h"
#include "robot_position.h"
#include "open_interface.h"
#include "sweep.h"
#include "driving.h"
#include "timer.h"
/*
* I am going to make all the return types for driving functions a
* Boolean, this is to show whether each driving function has been
* completed succesfully.
*/

volatile object_info object[50];
//info object[50];
info objects_in_view[20];
//Global variables for robot_position
double robot_x;
double robot_y;


/*
* drives through two given objects in a safe way
* still need to have the function return
* false if not completed all the way
* @ Author: Brandon Johnson
* @ Date: 4/28/2019
*/

void driver_main(){
   //fill_object_struct();

    int step = 3;

    //step to be chosen if need to drive between two objects
    if(step == 1){
        int index1, index2;
        //get the two closest objects
        two_closest_objects(&index1, &index2);

        if(!drive_through_objects(index1, index2)){
            return;
        }
    }
    if(step == 2){
        int num_objects_in_view = 0;
        find_objects_in_view(&num_objects_in_view);
    }
    if(step == 3){
        double x_obj, y_obj, width;

        int size = get_num_objects();
        get_object_info(&x_obj, &y_obj, &width, size-1);
        if(!drive_around_object(x_obj, y_obj, width)){
            return;
        }
    }




}
/*
* returns the indices of the 2 closest objects to the robot
* @ Author: Brandon Johnson
* @ Date: 4/28/2019
*/
void two_closest_objects(int *index1, int *index2){
    get_robot_position(&robot_x, &robot_y);
    int size = get_num_objects();
    int i;
    double distance, first, second, x, y, width;


    //first and second as really big
    first = 1000;
    second = 1000;


    //safecheck to make sure there is at least 2 objects
    if(size < 2){
        return;
    }


    //finds indices of two smallest objeccts
    for(i=0; i<size; i++){
        get_object_info(&x, &y, &width, i);

        if((width < .1) && (width > -.1)){
            continue;
        }
        distance = distance_between_points(robot_x, robot_y, x, y);

        if(distance < first){
            second = first;
            first = distance;
            *index1 = i;
        }

        else if(distance < second && distance != first){
            second = distance;
            *index2 = i;
        }


    }

}

/*
* fills up drives struct with necessary
* object info from sweeps/
* @ Author: Brandon Johnson
* @ Date: 4/28/2019
*/
void fill_object_struct(){
    int count = get_num_objects();
    int i;
    double x, y, width;

    for(i=0; i<=count; ++i){
        get_object_info(&x, &y, &width, i);
        object[i].x = x;
        object[i].y = y;
        object[i].width = width;
    }
}


void find_objects_in_view(int *number){
    int count = get_num_objects();
    int i;
    double x, y, width;

    for(i=0; i<=count; ++i){
        get_object_info(&x, &y, &width, i);
        if(!in_view(x, y)){
            objects_in_view[i].x = x;
            objects_in_view[i].y = y;
            objects_in_view[i].width = width;
            *number++;
        }
    }
}


/*
* drives through two given objects in a safe way
* still need to have the function return
* false if not completed all the way
* @ Author: Brandon Johnson
* @ Date: 4/28/2019
*/
bool drive_through_objects(int index1, int index2){
    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);

    double mid_x, mid_y, point_x, point_y;
    double drive_angle, distance1, distance2;
    int direction;
    double x1, x2, y1, y2, width1, width2;
    get_object_info(&x1, &y1, &width1, index1);
    get_object_info(&x2, &y2, &width2, index2);
    // get postion of 2 closest objects
   // double x1 = //object[index1].x;
    //double y1 = //object[index1].y;
    //double x2 = //object[index2].x;
    //double y2 = //object[index2].y;


    midpoint(&mid_x, &mid_y, x1, y1, x2, y2);
    get_triangle(&point_x, &point_y, x1, y1, x2, y2);
    get_commands(&direction, &drive_angle, &distance1 ,&distance2, mid_x, mid_y, point_x, point_y);


    //if direction is 1, the objects are on the left side of the robot
    //else the objects are on the right side
    if(direction){
        turn_right(sensor_data, drive_angle);
        move_forward(sensor_data, distance1+10);
        //check to see if move forward succesfully
        if(!completed()){
            return false;
        }
        turn_left(sensor_data, 90);
        //moving an additional 5 so objects arent scanned again
        move_forward(sensor_data, distance1+15);
        //check to see if move forward succesfully
        if(!completed()){
            return false;
        }
    }
    else{

        turn_left(sensor_data, drive_angle);
        move_forward(sensor_data, distance1+10);
        //check to see if move forward succesfully
        if(!completed()){
            return false;
        }
        turn_right(sensor_data, 90);
        //moving an additional 5 so objects arent scanned again
        move_forward(sensor_data, distance1+15);
        //check to see if move forward succesfully
        if(!completed()){
            return false;
        }
    }


    oi_free(sensor_data);
    /*
    double x, y, width = 0;

    double x10, y10, width10 = 0;
    int number_objects = get_num_objects();
    get_object_info(&x, &y, &width, index1);

    get_object_info(&x10, &y10, &width10, index2);

    while(1){
        lcd_printf("angle1: %.1lf\ndistance1: %.1lf\ndistance2: %.1lf", drive_angle, distance1, distance2);
        timer_waitMillis(5000);
        lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", number_objects, x, y, width);
        timer_waitMillis(5000);
        lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", number_objects-1, x10, y10, width10);
        timer_waitMillis(5000);
    }

    */
    return true;
}
/*
* drives around an object in a safe way
* the inputs are the objects inputs
* @ Author: Brandon Johnson
* @ Date: 4/28/2019
*/
bool drive_around_object(double x, double y, double width){
    get_robot_position(&robot_x, &robot_y);
    double robot_angle = get_robot_angle();

    double distance = distance_between_points(x, y, robot_x, robot_y);
    double angle = min_angle(distance, width);

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    //hard coded to turn left

    double turn_angle = robot_angle - angle;





    turn_right(sensor_data, turn_angle);
    move_forward(sensor_data, distance + 10);
    if(!completed()){
        turn_angle = angle - robot_angle;
        turn_left(sensor_data, turn_angle);
        move_forward(sensor_data, distance + 10);


    }

    oi_free(sensor_data);

    return true;

}


