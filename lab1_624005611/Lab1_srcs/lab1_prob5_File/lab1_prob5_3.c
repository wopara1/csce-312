//CSCE 312: Lab-1 Problem-5 framework
// This is version-2; bugfix for timediff
/* ***   README   **** */
/*This file is a framework: i.e. there is no actual code whose execution time will be
measured. You need to populate the appropriate functions with the code that you wrote 
for the previous problems in order to get useful data. 

Turning in this file without your code will result in zero points being awarded for this problem.

run this file as: gcc FileName.c -o ExecutableName -lrt 

*/




#include <stdio.h>
#include <time.h>

// Macro definitions to ensure portablity between both sun.cs and linux.cs

#if defined(sun)
    #define CLOCKNAME CLOCK_HIGHRES
        
#else
    #define CLOCKNAME CLOCK_PROCESS_CPUTIME_ID
#endif

unsigned int driver_on_seat;
unsigned int doors_closed;
unsigned int bell;

unsigned int driver_seat_belt_fastened;
unsigned int engine_running;

unsigned int key_in_car;
unsigned int door_lock_actuator;

unsigned int brake_pedal;
unsigned int car_moving;
unsigned int brake_actuator;

inline void read_inputs_from_ip_if(){

	//place your input code here
	//to read the current state of the available sensors
	
	FILE* in = fopen("lab1_prob5_input3.txt","r");
   
    fscanf (in, "%i %i %i %i %i %i %i", &engine_running, &driver_seat_belt_fastened, &doors_closed, &driver_on_seat, &key_in_car, &brake_pedal, &car_moving);
	
	
	fclose(in);
	

}

inline void write_output_to_op_if(){

	//place your output code here
    //to display/print the state of the 3 actuators (DLA/BELL/BA)
    
    FILE* out = fopen("lab1_prob5_output3.txt", "w");
    
		fprintf(out, "*** ACTUATOR STATUS ***");
		fprintf(out, "\nBell = %i\nDoor Locked = %i\nBrake = %i", bell, door_lock_actuator, brake_actuator);
		fprintf(out, "\n\nengine_running = %i\ndriver_seat_belt_fastened = %i\ndoor_closed = %i \ndriver_on_seat = %i\nkey_in_car = %i \nbrake_pedal = %i \ncar_moving = %i", engine_running, driver_seat_belt_fastened, doors_closed, driver_on_seat, key_in_car, brake_pedal, car_moving);
    
    fclose(out);
	

}

//The code segment which implements the decision logic
inline void control_action(){

// Put your control/decision logic code segments inside this function
// This is the actual code whose execution time which is being measure
	
	if(engine_running == 1 && !driver_seat_belt_fastened == 1 && !doors_closed == 1) {
		bell = 1;
		//printf("Bell = 1");
	} else if (engine_running == 1 && !doors_closed == 1 && driver_seat_belt_fastened == 1) {
		bell = 1;
		//printf("Bell = 1");
	} else if (engine_running == 1 && doors_closed == 1 && !driver_seat_belt_fastened == 1) {
		bell = 1;
		//printf("Bell = 1");
	} else if (engine_running == 1 && !doors_closed == 1 && !driver_seat_belt_fastened == 1) {
		bell = 1;
		//printf("Bell = 1");
	} else if ((engine_running == 1 && driver_seat_belt_fastened == 1) && (engine_running == 1 && doors_closed == 1) ) {
		bell = 0;
		//printf("Bell = 0");
	} else {
		bell = 0;
		//printf("Bell = 0");
	}
	
	// conditional regarding door locked actuator
	if(!driver_on_seat == 1 && key_in_car == 1) {
		door_lock_actuator = 0;
		//printf(" Doors locked = 0");
	} else {
		door_lock_actuator = 1;
		//printf(" Doors locked = 1");
	}
	
	// conditional regarding brake actuator
	if(brake_pedal == 1 && car_moving == 1) {
		brake_actuator = 1;
		printf(" Brakes = 1");
	} else {
		brake_actuator = 0;	
		//printf(" Brakes = 0");
	}


}


/* ---     You should not have to modify anything below this line ---------*/

/*timespec diff from
http://www.guyrutenberg.com/2007/09/22/profiling-code-using-clock_gettime/
*/
struct timespec diff(struct timespec start, struct timespec end)
 {
    struct timespec temp;
    //the if condition handles time stamp end being smaller than than 
    //time stamp start which could lead to negative time.

     if ((end.tv_nsec-start.tv_nsec)<0) {
          temp.tv_sec = end.tv_sec-start.tv_sec-1;
          temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
      } else {
          temp.tv_sec = end.tv_sec-start.tv_sec;
          temp.tv_nsec = end.tv_nsec-start.tv_nsec;
      }
  return temp;
 }

int main(int argc, char *argv[])
{
	unsigned int cpu_mhz;
	unsigned long long int begin_time, end_time;
	struct timespec timeDiff,timeres;
    struct timespec time1, time2, calibrationTime;
	
    clock_gettime(CLOCKNAME, &time1);
	clock_gettime(CLOCKNAME, &time2);
	calibrationTime = diff(time1,time2); //calibration for overhead of the function calls
    clock_getres(CLOCKNAME, &timeres);  // get the clock resolution data
	
    read_inputs_from_ip_if(); // get the sensor inputs
	
	clock_gettime(CLOCKNAME, &time1); // get current time
	control_action();                 // process the sensors
	clock_gettime(CLOCKNAME, &time2);   // get current time

	write_output_to_op_if();    // output the values of the actuators
	
	timeDiff = diff(time1,time2); // compute the time difference

	printf("Timer Resolution = %u nanoseconds \n ",timeres.tv_nsec);
	printf("Calibrartion time = %u seconds and %u nanoseconds \n ", calibrationTime.tv_sec, calibrationTime.tv_nsec);
    printf("The measured code took %u seconds and ", timeDiff.tv_sec - calibrationTime.tv_sec);
	printf(" %u nano seconds to run \n", timeDiff.tv_nsec - calibrationTime.tv_nsec);
	
	return 0;
}
