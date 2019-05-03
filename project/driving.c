
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

int indices_in_view[50];
volatile int sense;

/*
* drives through two given objects in a safe way
* still need to have the function return
* false if not completed all the way
* @ Author: Brandon Johnson
* @ Date: 4/28/2019
*/

void driver_main(){

    int objects_in_view = get_indices();
    lcd_printf("%d", objects_in_view);

    //if zero, move forward
    if(objects_in_view == 0){
        oi_t *sensor_data = oi_alloc();
        oi_init(sensor_data);
       move_forward(sensor_data, 40);
        while (!completed()) //If the robot still can not complete the commands above, the robot is trapped in the loop until it is able to finish
        {
            lcd_printf("not completed");
            if (sense == 1 || sense == 2) //Direction allows us to see what direction the robot needs to turn
            {
                lcd_printf("turn_right");
                turn_right(sensor_data, 20);
                move_forward(sensor_data, 30);
            }
            else if(sense == 3 || sense == 4)
            {
                lcd_printf("turn_left");
                turn_left(sensor_data, 20);
                move_forward(sensor_data, 30);
            }
            else{
                turn_right(sensor_data, 20);
                move_forward(sensor_data, 30);
            }

        }
    }
    //if one drive around it
    if(objects_in_view == 1){
        double x_obj, y_obj, width;

        get_object_info(&x_obj, &y_obj, &width, indices_in_view[0] );
        lcd_printf("%d", indices_in_view[0]);
        if(!drive_around_object(x_obj, y_obj, width)){

            return;
        }
    }
    //if two or more, drive through the closest 2
    if(objects_in_view >= 2){
        int index1, index2;
        //get the two closest objects
        double x_obj, y_obj, width, x_obj1, y_obj1, width1;
        two_closest_objects(&index1, &index2);

        if(!drive_through_objects(index1, index2)){

            return;
        }

        get_object_info(&x_obj, &y_obj, &width, index1);
        get_object_info(&x_obj1, &y_obj1, &width1, index2);
        if(width1 < 7 && width < 7){
            lcd_printf("Epic Gamer Win");
            oi_init_noupdate();
            //play_song();
            while(1){
               play_song();
            }
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
    for(i=0; i<get_indices(); i++){
        get_object_info(&x, &y, &width, indices_in_view[i] ); //changed

        if((width < .1) && (width > -.1)){
            continue;
        }
        distance = distance_between_points(robot_x, robot_y, x, y);

        if(distance < first){
            //second = first;
            first = distance;
            *index1 = i;
        }

        else if(distance < second && distance != first){
            second = distance;
            *index2 = i;
        }


    }

}



void goal(double goalwidth, int index)
{
    return; //TODO
    if (goalwidth < 7)
    {
        lcd_printf("FOUND TARGET");
        //Dont ever leave me

    }
}




int get_indices(){

    int i;
    double x, y, width;

    //clear the index info
    for(i = 0; i<50; i++){
        indices_in_view[i] = 420;
    }


    int count = 0;
    //add index if in view;
    for(i=0; i<get_num_objects(); i++){

        get_object_info(&x, &y, &width, i);

        if(in_view(x,y)){
            indices_in_view[count] = i;
            count++;
        }
    }
    return count;
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
//    while(1){
//
        lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", index1, x1, y1, width1);
        timer_waitMillis(1000);
        lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", index2, x2, y2, width2);
        timer_waitMillis(1000);
//
//    }




    // get postion of 2 closest objects
   // double x1 = //object[index1].x;
    //double y1 = //object[index1].y;
    //double x2 = //object[index2].x;
    //double y2 = //object[index2].y;


    midpoint(&mid_x, &mid_y, x1, y1, x2, y2);
    get_triangle(&point_x, &point_y, x1, y1, x2, y2);
    get_commands(&direction, &drive_angle, &distance1 ,&distance2, mid_x, mid_y, point_x, point_y);

    //lcd_printf("angle1: %.1lf\ndistance1: %.1lf\ndistance2: %.1lf", drive_angle, distance1, distance2);

    //if direction is 1, the objects are on the left side of the robot
    if(drive_angle < 180){
        if(direction){
            turn_right(sensor_data, drive_angle);

            move_forward(sensor_data, distance1+5);
            //check to see if move forward succesfully
            if(!completed()){
                oi_free(sensor_data);
                return false;
            }
            turn_left(sensor_data, 90);
            //moving an additional 5 so objects arent scanned again
            move_forward(sensor_data, distance2+15);
            //check to see if move forward succesfully
            if(!completed()){
                oi_free(sensor_data);
                return false;
            }
        }
        else{
            turn_left(sensor_data, drive_angle);
            move_forward(sensor_data, distance1+5);
            //check to see if move forward succesfully
            if(!completed()){
                oi_free(sensor_data);
                return false;
            }
            turn_right(sensor_data, 90);
            //moving an additional 5 so objects arent scanned again
            move_forward(sensor_data, distance2+15);
            //check to see if move forward succesfully
            if(!completed()){
                oi_free(sensor_data);
                return false;
            }
        }
    }
    else{
        if(direction){
            turn_right(sensor_data, drive_angle);

            move_forward(sensor_data, distance1+5);
            //check to see if move forward succesfully
            if(!completed()){
                oi_free(sensor_data);
                return false;
            }
            turn_right(sensor_data, 90);
            //moving an additional 5 so objects arent scanned again
            move_forward(sensor_data, distance2+15);
            //check to see if move forward succesfully
            if(!completed()){
                oi_free(sensor_data);
                return false;
            }
        }
        else{
            turn_left(sensor_data, drive_angle);
            move_forward(sensor_data, distance1+8);
            //check to see if move forward succesfully
            if(!completed()){
                oi_free(sensor_data);
                return false;
            }
            turn_left(sensor_data, 95);
            //moving an additional 5 so objects arent scanned again
            move_forward(sensor_data, distance2+15);
            //check to see if move forward succesfully
            if(!completed()){
                oi_free(sensor_data);
                return false;
            }
        }



    }



    oi_free(sensor_data);

    double x, y, width = 0;

    double x10, y10, width10 = 0;
    int number_objects = get_num_objects();
    get_object_info(&x, &y, &width, index1);

    get_object_info(&x10, &y10, &width10, index2);

   // while(1){
      //  lcd_printf("angle1: %.1lf\ndistance1: %.1lf\ndistance2: %.1lf", drive_angle, distance1, distance2);
       // timer_waitMillis(5000);
      //  lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", number_objects, x, y, width);
        //timer_waitMillis(5000);
      //  lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", number_objects-1, x10, y10, width10);
       // timer_waitMillis(5000);
   // }


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



    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    //hard coded to turn left


    double turn_angle, distance, x1, y1;
    int direction, side = 1;


    point_near_object(&x1, &y1, x, y, width, side);

    drive_to_point(&direction, &distance, &turn_angle, x, y);

    lcd_printf("turn %d\n %.1lf\n %.1lf\n", direction, distance, turn_angle);

    timer_waitMillis(200);

    //first try the right side
    if(direction){
        turn_right(sensor_data, turn_angle);
        move_forward(sensor_data, distance + 10);
        if(completed()){
            return true;
        }
       // turn_left(sensor_data, turn_angle);
    }
    else{
        turn_left(sensor_data, turn_angle);
        move_forward(sensor_data, distance + 10);
        if(completed()){
            return true;
        }
       // turn_right(sensor_data, turn_angle);
    }

    //if this fails we will try to turn left
    point_near_object(&x1, &y1, x, y, width, 0);
    drive_to_point(&direction, &distance, &turn_angle, x, y);

    if(direction){
        turn_right(sensor_data, turn_angle);
        move_forward(sensor_data, distance + 10);
        if(completed()){
            oi_free(sensor_data);
            return true;
        }
        //turn_left(sensor_data, turn_angle);
    }
    else{
        turn_left(sensor_data, turn_angle);
        move_forward(sensor_data, distance + 10);
        if(completed()){
            oi_free(sensor_data);
            return true;
        }
        //turn_right(sensor_data, turn_angle);
    }

    turn_right(sensor_data, 180);



    oi_free(sensor_data);

    return false;

}


