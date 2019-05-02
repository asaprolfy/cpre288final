/*
 * sweep.h
 *
 *
 * Created:  April 17, 2019
 *   Author: Rolf Anderson
 */

#ifndef SWEEP_H_
#define SWEEP_H_


typedef struct object_information{
    double distance;
    int degree;
    double width;
    double x;
    double y;
}object_info;


volatile extern object_info object[50];

void sweep();
int get_num_objects();
void get_object_info(double *x, double *y, double *width, int index);
int check_existing_object(double x, double y);

#endif /* SWEEP_H_ */
