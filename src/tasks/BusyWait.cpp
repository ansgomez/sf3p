#include "tasks/BusyWait.h"

#include "core/Dispatcher.h"
#include "core/Simulation.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///The constructor requires a pointer to the simulation, its own dispatcher, and the WCET
BusyWait::BusyWait(struct timespec wcet) : Task(busy_wait) {
  WCET = wcet;
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM TASK ****/

void BusyWait::fire() {
  unsigned int start, end, count=0, wcet_us = TimeUtil::convert_us(WCET);
  
  start = TimeUtil::convert_us(TimeUtil::getTime());

  //BUSY WAIT LOOP
  do {
    end = TimeUtil::convert_us(TimeUtil::getTime());

    if( (end - start) > 100) {
      count += 100;
      start = end;
    }
  } while ( count < wcet_us && Simulation::isSimulating()); 
}
