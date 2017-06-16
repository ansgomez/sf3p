#include "schedulers/FixedPriority.h"

#include "core/Criteria.h"
#include "queues/PriorityQueue.h"
#include "util/Operators.h"

#include <iostream>

#define _INFO 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation
FixedPriority::FixedPriority(unsigned int _id, int level) : EventBased(_id, level) {
  #if _INFO==1
  cout << "Creating FixedPriority with ID: " << id << endl;
  #endif

  //Create activeQueue object
  activeQueue = (RunnableQueue*) new PriorityQueue();
}
