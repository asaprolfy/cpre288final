#include <stdbool.h>
#include "lcd.h"
#include "geometry.h"
#include "movement.h"
#include "robot_position.h"
#include "open_interface.h"
#include "sweep.h"
#include "servo.h"
#include "driving.h"
#include "timer.h"
#include "cliffSensor.h"
/*
 * I am going to make all the return types for driving functions a
 * Boolean, this is to show whether each driving function has been
 * completed succesfully.
 */
volatile int sense;
volatile object_info object[50];
//info object[50];
info objects_in_view[20];
//Global variables for robot_position
double robot_x;
double robot_y;

int indices_in_view[50];


/*
 * Made by Asa, Ege, and crack cocaine
 */
void driver_main(oi_t *sensor)
{
    double small; //works as a temp variable
    double smallest = 100; //Actuall smallest value from get_indices
    int smallindex; //Would be equal to the smallest objects index value in the struct array
    int index = 0;
    int j = 0;
    int i;
    double x, y;
    move_forward(sensor, 10); //moves forward a first time, gets intial data, Do sweep functions need to be here?
    sweep();
    index = get_indices(); //Maximum iteration size needed to find the smallest object in view
    for (i = 0; i < index; ++i)
    {
        j = indices_in_view[i]; //Records the current index equivalent to the orginal struct array of object data
        get_object_info(&x, &y, &small, j); //temp variable stores the current width value
        if (smallest > small)
        {
            smallest = small; //Replaces smallest with the actual smallest value seen
            smallindex = j; //Records the index of the original struct array for use in goal
        }
    }
    goal(smallest, smallindex); //Decides if a possible goal has been found, if so becomes 'primary' logic
    int index1, index2;

    if (sense != 1 && sense != 2 && sense != 3 && sense != 4) //Checks to see if a boundary has been found, if not this happens
    {
        if (j == 0) //If no objects are found nothing specially happens and the bot gets ready to move forward and scan again
        {
            return;
        }
        else if (j == 1) //If 1 object is found the bot moves around the object and gets ready to move forward and scan agin
        {
           drive_around_object(x,y,smallest);
        }
        else //If 2 or more objects are found, the bot finds the 2 closest objects and moves between them and then prepares to move forward and scan again
        {
            two_closest_objects(&index1, &index2);
            drive_through_objects(index1, index2);
        }

    }
    else //Happens if a boundary has been found
    {
        int direction;

        if (sense == 1 || sense == 2) //Checks to see if the left side sensors are the detectors
        {
            direction = 1; //Records for future use as the bot would permanently turn this direction in corners
            turn_right(sensor, 5); //Turns right as turning left would shove it into a wall more
            move_forward(sensor, 10); //Moves forward

        }
        else if (sense == 3 || sense == 4) //Same as above except for right sensors and turns left instead
        {
            direction = 2;
            turn_left(sensor, 5);
            move_forward(sensor, 10);
        }

        while (!completed()) //If the robot still can not complete the commands above, the robot is trapped in the loop until it is able to finish
        {
            if (direction == 1) //Direction allows us to see what direction the robot needs to turn
            {
                turn_right(sensor, 5);
                move_forward(sensor, 10);
            }
            else
            {
                turn_left(sensor, 5);
                move_forward(sensor, 10);
            }
        }
        sense = 0; //Resets sense so the robot can reset its logic

    }

}
/*
 * Made by Asa, Ege, and crack cocaine
 */
void goal(double goalwidth, int index)
{
    return; //TODO
    if (goalwidth < 15)
    {
        //Dont ever leave me

    }
}

/*
 * returns the indices of the 2 closest objects to the robot
 * @ Author: Brandon Johnson
 * @ Date: 4/28/2019
 */
void two_closest_objects(int *index1, int *index2)
{
    get_robot_position(&robot_x, &robot_y);
    int size = get_num_objects();
    int i;
    double distance, first, second, x, y, width;

    //first and second as really big
    first = 1000;
    second = 1000;

    //safecheck to make sure there is at least 2 objects
    if (size < 2)
    {
        return;
    }

    //finds indices of two smallest objeccts
    for (i = 0; i < size; i++)
    {
        get_object_info(&x, &y, &width, i);

        if ((width < .1) && (width > -.1))
        {
            continue;
        }
        distance = distance_between_points(robot_x, robot_y, x, y);

        if (distance < first)
        {
            //second = first;
            first = distance;
            *index1 = i;
        }

        else if (distance < second && distance != first)
        {
            second = distance;
            *index2 = i;
        }

    }

}

int get_indices()
{

    int i;
    double x, y, width;

    //clear the index info
    for (i = 0; i < 50; i++)
    {
        indices_in_view[i] = 420;
    }

    int count = 0;
    //add index if in view;
    for (i = 0; i < get_num_objects(); i++)
    {

        get_object_info(&x, &y, &width, i);

        if (in_view(x, y))
        {
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
void fill_object_struct()
{
    int count = get_num_objects();
    int i;
    double x, y, width;

    for (i = 0; i <= count; ++i)
    {
        get_object_info(&x, &y, &width, i);
        object[i].x = x;
        object[i].y = y;
        object[i].width = width;
    }
}

void find_objects_in_view(int *number)
{
    int count = get_num_objects();
    int i;
    double x, y, width;

    for (i = 0; i <= count; ++i)
    {
        get_object_info(&x, &y, &width, i);
        if (!in_view(x, y))
        {
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
bool drive_through_objects(int index1, int index2)
{
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
    timer_waitMillis(5000);
    lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", index2, x2, y2, width2);
    timer_waitMillis(5000);
//
//    }

// get postion of 2 closest objects
// double x1 = //object[index1].x;
//double y1 = //object[index1].y;
//double x2 = //object[index2].x;
//double y2 = //object[index2].y;

    midpoint(&mid_x, &mid_y, x1, y1, x2, y2);
    get_triangle(&point_x, &point_y, x1, y1, x2, y2);
    get_commands(&direction, &drive_angle, &distance1, &distance2, mid_x, mid_y,
                 point_x, point_y);

    //lcd_printf("angle1: %.1lf\ndistance1: %.1lf\ndistance2: %.1lf", drive_angle, distance1, distance2);

    //if direction is 1, the objects are on the left side of the robot
    //else the objects are on the right side
    if (direction)
    {
        turn_right(sensor_data, drive_angle);

        move_forward(sensor_data, distance1 + 10);
        //check to see if move forward succesfully
        if (!completed())
        {
            return false;
        }
        turn_left(sensor_data, 90);
        //moving an additional 5 so objects arent scanned again
        move_forward(sensor_data, distance1 + 15);
        //check to see if move forward succesfully
        if (!completed())
        {
            return false;
        }
    }
    else
    {
        turn_left(sensor_data, drive_angle);
        move_forward(sensor_data, distance1 + 10);
        //check to see if move forward succesfully
        if (!completed())
        {
            return false;
        }
        turn_right(sensor_data, 90);
        //moving an additional 5 so objects arent scanned again
        move_forward(sensor_data, distance1 + 15);
        //check to see if move forward succesfully
        if (!completed())
        {
            return false;
        }
    }

    oi_free(sensor_data);

    double x, y, width = 0;

    double x10, y10, width10 = 0;
    int number_objects = get_num_objects();
    get_object_info(&x, &y, &width, index1);

    get_object_info(&x10, &y10, &width10, index2);

    while (1)
    {
        lcd_printf("angle1: %.1lf\ndistance1: %.1lf\ndistance2: %.1lf",
                   drive_angle, distance1, distance2);
        timer_waitMillis(5000);
        lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", number_objects, x, y, width);
        timer_waitMillis(5000);
        lcd_printf("%d: (%.1lf , %.1lf)\n%.1lf", number_objects - 1, x10, y10,
                   width10);
        timer_waitMillis(5000);
    }

    return true;
}
/*
 * drives around an object in a safe way
 * the inputs are the objects inputs
 * @ Author: Brandon Johnson
 * @ Date: 4/28/2019
 */
bool drive_around_object(double x, double y, double width)
{
    get_robot_position(&robot_x, &robot_y);
    double robot_angle = get_robot_angle();

    oi_t *sensor_data = oi_alloc();
    oi_init(sensor_data);
    //hard coded to turn left

    double turn_angle, distance, x1, y1;
    int direction, side = 1;

    //point_near_object(&x1, &y1, x, y, width, side);

    //drive_to_point(&direction, &distance, &turn_angle, x, y);

    lcd_printf("turn %d\n %.1lf\n %.1lf\n", direction, distance, turn_angle);

    timer_waitMillis(200);

    //first try the right side
    if (direction)
    {
        turn_right(sensor_data, turn_angle);
        move_forward(sensor_data, distance + 10);
        if (completed())
        {
            return true;
        }
        turn_left(sensor_data, turn_angle);
    }
    else
    {
        turn_left(sensor_data, turn_angle);
        move_forward(sensor_data, distance + 10);
        if (completed())
        {
            return true;
        }
        turn_right(sensor_data, turn_angle);
    }

    //if this fails we will try to turn left
    //point_near_object(&x1, &y1, x, y, width, 0);
    //drive_to_point(&direction, &distance, &turn_angle, x, y);

    if (direction)
    {
        turn_right(sensor_data, turn_angle);
        move_forward(sensor_data, distance + 10);
        if (completed())
        {
            return true;
        }
        turn_left(sensor_data, turn_angle);
    }
    else
    {
        turn_left(sensor_data, turn_angle);
        move_forward(sensor_data, distance + 10);
        if (completed())
        {
            return true;
        }
        turn_right(sensor_data, turn_angle);
    }

    turn_right(sensor_data, 180);

    oi_free(sensor_data);

    return false;

}
