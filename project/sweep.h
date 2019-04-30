#ifndef SWEEP_H_
#define SWEEP_H_

typedef struct object_information
{
    double distance;
    int degree;
    double width;
    double x;
    double y;
} object_info;

void sweep();
int get_num_objects();
void get_object_info(double *x, double *y, double *width, int index);
void add_object(double width);
int check_existing_object(double x, double y, double width);

#endif /* SWEEP_H_ */
