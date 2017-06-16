#include "core/Worker.h"

#include "core/Criteria.h"
#include "core/ResourceAllocator.h"
#include "core/Simulation.h"
#include "core/Task.h"
#include "criteria/InclusiveCriteria.h"
#include "pthread/Priorities.h"
#include "results/Statistics.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>

#define _DEBUG 0
#define _INFO 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

Worker::Worker(ResourceAllocator *p, unsigned int _id, _task_load tl) : Runnable(_id) {
  #if _INFO == 1
  cout << "++New Worker \t - " << _id << endl;
  #endif

  parent = p;
  thread_type = worker;
  task_load = tl;
  Statistics::addWorkerId(_id);

  criteria = new InclusiveCriteria();
  //crtieria->setRelativeDeadline();

  //Default value for relativeDeadline
  relativeDeadline = TimeUtil::Millis(20);

  //Semaphore initialization
  sem_init(&activation_sem, 0, 1); //mutex semaphore
  sem_init(&arrival_sem, 0, 1); //mutex semaphore
  sem_init(&worker_sem, 0, 1); //mutex semaphore
  sem_init(&wrapper_sem, 0, 0); //signal semaphore
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD ****/

///This join function takes into account the worker's unblocking mechanism
void Worker::join() {

  //Post to sems in case worker is blocked
  sem_post(&wrapper_sem);
  sem_post(&worker_sem);
  sem_post(&worker_sem);
  sem_post(&activation_sem);
  sem_post(&activation_sem);
  sem_post(&arrival_sem);
  sem_post(&arrival_sem);

  if(parent!=NULL) {
    parent->join();
  }

  join2();
}

///This inherited function will be executed by the worker thread
void Worker::wrapper() {
  struct timespec now, deadline, arrival;

  //Wait until the simulation is initialized
  while( !Simulation::isInitialized() );

  while( Simulation::isSimulating() ) {

    //Wait for new job
    sem_wait(&wrapper_sem);

    #if _INFO == 1
    cout << "Worker: " << id << " began executing a new job\n";
    #endif

    if( Simulation::isSimulating() ) {

      Statistics::addTrace(worker, id, task_start);

      if(task != NULL) {
	task->fire();    
      }
      else {
	cout << "Worker error: task is null!\n";
      }

      //Add the task end to the statistics
      Statistics::addTrace(worker, id, task_end);

      now = TimeUtil::getTime();
      if(criteria!=NULL) {
	deadline = criteria->getArrivalTime() + relativeDeadline;
      }
      else {
	cout << "Worker::wrapper() - criteria is null!\n";
      }

      //If deadline was missed, add to statistics
      if(now > deadline) {
	arrival = TimeUtil::relative(criteria->getArrivalTime());
	Statistics::addMissedDeadline(id, arrival, TimeUtil::relative(deadline));
	Statistics::addTrace(worker, id, deadline_missed);
      }
      else {
	Statistics::addTrace(worker, id, deadline_met);
      }
    }

    //Handle the end of the current job (might register finishedJob or updateRunnable with parent)
    finishedJob();
  }
}

/**** FROM RUNNABLE ****/

///This function gives the worker thread the ACTIVE_PR priority
void Worker::activate() {
  if(state == activated) {
    cout << "Worker::activate error - already active!\n";
  }

  sem_wait(&activation_sem);
    Statistics::addTrace(worker, id, sched_start);
    setPriority(Priorities::get_active_pr());
    state = activated;
  sem_post(&activation_sem);
}

///This function give the worker thread the INACTIVE_PR priority
void Worker::deactivate() {
  if(state == deactivated ) {
    cout << "Worker::deactivate error - already deactivated!\n";
  }

  //sem_wait(&arrival_sem);
  sem_wait(&activation_sem);
    Statistics::addTrace(worker, id, sched_end);
    setPriority(Priorities::get_inactive_pr());    
    state = deactivated;
  sem_post(&activation_sem);
  //sem_post(&arrival_sem);
}

/*********** MEMBER FUNCTIONS ***********/

///This function erases the head of the active_queue, and updates any pending events
void Worker::finishedJob() {

  if( !Simulation::isSimulating() ) {
    return;
  }

  bool finished = false;
  bool updated = false;

  #if _DEBUG==1
  cout << "Worker::finishedJob() - registering with parent\n";
  #endif

  sem_wait(&arrival_sem);
    //If there are any jobs left on queue, register new head
    if(arrival_times.size() > 1) {
      //Update object's schedulable criteria
      if(criteria != NULL) {
        criteria->setArrivalTime(arrival_times[1]);
        criteria->setDeadline(arrival_times[1]+relativeDeadline);
      }
      else {
        cout << "Worker::finishedJob() - criteria is null! (1)\n";
      }
      updated=true;
    }
    //If no jobs are pending, remove from parent
    else {
      //Clear schedulable criteria
      if(criteria != NULL) {
        criteria->setArrivalTime(TimeUtil::Millis(0));
        criteria->setDeadline(TimeUtil::Millis(0));
      }
      else {
        cout << "Worker::finishedJob() - criteria is null! (2)\n";
      }
      finished = true;
    }
    arrival_times.pop_front(); //Erase old arrival time
  sem_post(&arrival_sem);

  //Register event with the parent
  if(parent!=NULL) {
    if(updated) {
      parent->updateRunnable(this);
    }
    else if(finished) {
      parent->finishedJob(id); 
    }
    else {
      cout << "Worker::finishedJob() - no event is being handled!\n";
    }
  }
  else {
    cout << "Worker::finishedJob() - parent is null!\n";
  }
}

///This function will be called by the dispatcher thread, and will post to the wrapper_sem
void Worker::newJob() {
  //add arrival time before critical section
  struct timespec aux = TimeUtil::getTime();
  bool reportNewJob = false;

  #if _DEBUG==1
  cout << "Worker::newjob() is waiting\n";
  #endif

  sem_wait(&arrival_sem);

  arrival_times.push_back(aux);

  #if _DEBUG==1
  cout << "Worker::newjob() is processing\n";
  #endif

  //If there were no active jobs before, register event
  if(arrival_times.size() == 1) {

    //Update schedulable criteria
    if( criteria != NULL ) {
      criteria->setArrivalTime(arrival_times.front());
      criteria->setDeadline(arrival_times.front());//Derived function adds its own relative deadline
    }
    else {
      cout << "Worker::newJob() - criteria is null!\n";
    }
      
    reportNewJob = true;
  }
  /*If there is an active job, finishedJob() will take care of 
    'registering' this new job with parent */

  //Signal the worker thread
  sem_post(&wrapper_sem);

  sem_post(&arrival_sem);

  if(reportNewJob) {
    //Notify parent of new arrival
    if (parent != NULL ) {
      parent->newJob(this);
    }
    else {
      cout << "Worker::newJob() - parent is null!\n";
    }
  }
}

/*********** GETTER AND SETTER FUNCTIONS ***********/
///This function gets the relative deadline
struct timespec Worker::getRelativeDeadline() {
  return relativeDeadline;
}

//This function sets the Criteria object (deleting the previous one..)
void Worker::setCriteria(Criteria* c) {
  if(criteria!=NULL) {
    delete(criteria);
  }
  criteria = c;
}

///This function sets the relative deadline
void Worker::setRelativeDeadline(struct timespec aux) {
  relativeDeadline = aux;
}

///This function sets the worker's load
void Worker::setTask(Task *t) {
  task = t;
}
