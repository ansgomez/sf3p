#include "schedulers/EDF.h"

#include "core/Criteria.h"
#include "queues/DeadlineQueue.h"
#include "util/Operators.h"

#include <iostream>

#define _INFO 0
#define _DEBUG 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation
EDF::EDF(unsigned int _id, int level) : EventBased(_id, level) {
  #if _INFO==1
  cout << "Creating EDF with ID: " << id << endl;
  #endif

  //Create activeQueue object
  activeQueue = (RunnableQueue*) new DeadlineQueue();
}

