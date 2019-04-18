void sweep(double x, double y, struct object){
	int degrees, start, finish, flag = 0;
    double ir, sonar;
	double *x_obj, *y_obj
    int count = 0;
    int i, mid_angle;
	for(i = 0; i < 181; i++){
        degrees = i;
        sonar = ping_getDistance();
        ir = pow(adc_avg_distance(), -1.855) * 2E+07;

        // if there is something within range begin keeping track of degrees
        if(abs(sonar-ir) < 20 && flag == 0 && sonar < 60){
            flag = 1;
            start = degrees;
        }
        // if the object is no longer detected calculate range and size, add to struct array
        if(abs(sonar-ir) > 20 && flag == 1 && sonar <60 && ir>sonar){
            finish = degrees;
            flag = 0;
			// put in the struct given
			mid_angle = (start + finish) / 2;
			object_position(x, y, mid_angle, sonar, *x_obj, *y_obj;
            object[count].distance = sonar;
            object[count].width = sqrt(2*pow(sonar,2)-2*pow(sonar,2)*cos((finish-start)*(M_PI/180))) - 3;
			object[count].x = *x_obj;
			object[count].y = y_obj
            count++;
        }
        servo_move(i);

    }
}