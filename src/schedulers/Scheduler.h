#ifndef _SCHEDULER_H
#define _SCHEDULER_H

#include "core/ResourceAllocator.h"
#include "core/Worker.h"
#include "util/Enumerations.h"

#include <vector>

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class Scheduler : public ResourceAllocator {

 protected:
 
  /*********** VARIABLES ***********/

  ///This variable holds the level of the scheduler (helps assign correct hierarchical priority)
  int level;

  ///This variable stores the type of scheduler
  _sched_type sched_type;

 public:
 
  /*********** CONSTRUCTOR ***********/
   
  ///Constructor needs pointer to simulation as well as the scheduler's id and hierarchical level
  Scheduler(unsigned int _id, int level);

  /*********** INHERITED FUNCTIONS ***********/

  /**** FROM THREAD ****/

  ///Calling this function will block the calling thread until this thread exits
  virtual void join();

  ///This is the pthread's wrapper function
  void wrapper();

  /**** FROM RUNNABLE ****/

  ///This function rewrites the activate method to activate both the scheduler as well as its load
  virtual void activate();

  ///This function rewrites the deactivate method both the scheduler as well as its load
  virtual void deactivate();

  /**** FROM RESOURCEALLOCATOR ****/

  ///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void finishedJob(unsigned int runnable_id);

  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  virtual void newJob(Runnable* r);

  ///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
  virtual void updateRunnable(Runnable* r);

  /*********** MEMBER FUNCTIONS ***********/

  ///This function performs the actual scheduling (figuring out the order of execution for its load)
  virtual void schedule();
};

#endif
