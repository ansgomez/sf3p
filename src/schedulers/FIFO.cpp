#include "schedulers/FIFO.h"

#include "core/Criteria.h"
#include "queues/FIFOQueue.h"
#include "util/Operators.h"

#include <iostream>

#define _INFO 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation
FIFO::FIFO(unsigned int _id, int level) : EventBased(_id, level) {
  #if _INFO==1
  cout << "Creating RateMonotonic with ID: " << id << endl;
  #endif

  //Create activeQueue object
  activeQueue = (RunnableQueue*) new FIFOQueue();
}
