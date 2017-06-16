#include "schedulers/Scheduler.h"

#include "core/Simulation.h"

#include <iostream>

#define _INFO 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation as well as the scheduler's id and hierarchical level
Scheduler::Scheduler(unsigned int _id, int _level) : ResourceAllocator(_id){
  #if _INFO == 1
  cout << "++New Scheduler - " << _id << "\n";
  #endif

  thread_type = scheduler;
  level = _level;
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD ****/

///Calling this function will block the calling thread until this thread exits
void Scheduler::join() {
  join2();
}

///This is the pthread's wrapper function
void Scheduler::wrapper() {

  #if _INFO == 1
  cout << "Sched: " << id << " waiting for initialization\n";
  #endif

  //Wait until the simulation is initialized
  while( ! Simulation::isInitialized());

  #if _INFO == 1
  cout << "Sched: " << id << " begining execution\n";
  #endif

  schedule();
}

/**** FROM RUNNABLE ****/

///This function rewrites the activate method to activate both the scheduler as well as its load
void Scheduler::activate() {
  cout << "Scheduler::activate() - This should not print!\n";
}

///This function rewrites the deactivate method to deactivate both the scheduler as well as its load
void Scheduler::deactivate() {
  cout << "Scheduler::deactivate() - This should not print!\n";
}

/**** FROM RESOURCEALLOCATOR ****/

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void Scheduler::finishedJob(unsigned int id) {
  cout << "Scheduler::finishedJob - This should not print!\n";
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void Scheduler::newJob(Runnable *r) {
  cout << "Scheduler::newJob - This should not print!\n";
}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void Scheduler::updateRunnable(Runnable* r) {
  cout << "Scheduler::updateRunnable - This should not print!\n";
}

/*********** MEMBER FUNCTIONS ***********/

///This function performs the actual scheduling (figuring out the order of execution for its load)
void Scheduler::schedule() {
  cout << "Scheduler::schedule - This should not print!\n";
}
