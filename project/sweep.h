#ifndef SWEEP_H_
#define SWEEP_H_

typedef struct object_information{
    double distance;
    int degree;
    double width;
    double x;
    double y;
}object_info;

//void sweep_and_detect_objects();
//void checkpoint1_sweep();
void sweep();
int get_num_objects();
void get_object_position(double *x, double *y, int index);

#endif /* SWEEP_H_ */
