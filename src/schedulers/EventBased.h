#ifndef _EVENT_BASED_H
#define _EVENT_BASED_H

#include "core/Simulation.h"
#include "pthread/Priorities.h"
#include "schedulers/Scheduler.h"

#include <deque>

class RunnableQueue;
class Criteria;

using namespace std;

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class EventBased : public Scheduler {

 protected:

  ///This queue holds all active Runnables underneath this scheduler.
  RunnableQueue* activeQueue;

  ///This points to the runnable previously activated by the scheduler
  Runnable* currentRunnable;

  ///This deque holds the ids of runnables in the active queue who already finished
  deque<unsigned int> finishedJobDeque;

  ///This queue holds the Runnables who need to be updated in the activeQueue
  deque<Runnable*> updateDeque;

  /**** SEMAPHORES ****/

  ///Semaphores to ensure proper execution
  sem_t activation_sem, event_sem, schedule_sem;

 public: 

  /*********** CONSTRUCTOR ***********/
 
  ///Constructor needs pointer to simulation, id, and its level
  EventBased(unsigned int id, int level);

  /*********** INHERITED FUNCTIONS ***********/

  /**** FROM THREAD  ****/

  ///This function redefines Thread::join() to take into account EDF unblocking mechanism...
  void join();

  /**** FROM RUNNABLE  ****/
  
  ///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load - this runs in the dispatcher thread
  void activate();

  ///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
  void deactivate();

  /**** FROM RESOURCEALLOCATOR  ****/
  
  ///This function handles the end of a Runnable's job by erasing the Runnable from the activeQueue
  void finishedJob(unsigned int runnable_id);

  ///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
  void newJob(Runnable* r);

  ///This function handles a change in the criteria of an active Runnable. This might lead to the calling Runnable to be moved from the head of the activeQueue to another position and thus cause a change in the order of execution.
  void updateRunnable(Runnable* r);

  /**** FROM SCHEDULER  ****/
  
  ///This function performs the actual scheduling (figuring out the order of execution for its load)
  void schedule();
};
#endif
