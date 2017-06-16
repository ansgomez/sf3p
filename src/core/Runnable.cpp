#include "core/Runnable.h"

#include "core/Criteria.h"
#include "core/Task.h"
#include "criteria/InclusiveCriteria.h"
#include "pthread/Priorities.h"
#include "results/Trace.h"

#include <iostream>

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

Runnable::Runnable(unsigned int _id) : Thread(_id) {
  //Default initial values
  state = deactivated;
  criteria = NULL;
  current_runnable = this;
  current_task = NULL;
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD ****/

///Calling this function will block the calling thread until this thread exits
void Runnable::join() {
  join2();
}

///This function will be executed by the pthread
void Runnable::wrapper() {
  //empty
  cout << "Runnable::wrapper - This should not print!\n";
}

/**** FROM RUNNABLE ****/

///This function set the current runnable to active, meaning that it has control of the CPU and should 'run'
void Runnable::activate() {
  cout << "Runnable::activate - This should not print!\n";
}

///This function set the current runnable to inactive, meaning that it has lost control of the CPU and has to stop running
void Runnable::deactivate() {
  cout << "Runnable::deactivate - This should not print!\n";
}

/*********** GETTERS AND SETTERS ***********/

///This function return the runnable's schedulable criteria
Criteria* Runnable::getCriteria() {
  return criteria;
}

///This function sets the runnable's parent
void Runnable::setParent(ResourceAllocator* p) {
  parent = p;
}
