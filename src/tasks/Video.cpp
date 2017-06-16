#include "tasks/Video.h"

#include <iostream>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///The constructor requires a pointer to the simulation, its own dispatcher, and the WCET
Video::Video() : Task(video) {
  state = mjpeg_init();
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM TASK ****/

void Video::fire() {
  mjpeg_fire(state);
}
