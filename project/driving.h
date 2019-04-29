#ifndef DRIVING_H_
#define DRIVING_H_



typedef struct driving_objects{
    double x;
    double y;
    double width;
}info;



void driver_main();

bool drive_through_objects(int index1, int index2);

void fill_object_struct();

void two_closest_objects(int *index1, int *index2);

void find_objects_in_view(int *number);

bool drive_around_object(double x, double y, double width);



#endif /* DRIVING_H_ */
