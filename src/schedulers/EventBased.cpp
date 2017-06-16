#include "schedulers/EventBased.h"

#include "core/Criteria.h"
#include "queues/RunnableQueue.h"

#include <iostream>
#include <time.h>

#define _DEBUG 0
#define _INFO 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation
EventBased::EventBased(unsigned int _id, int level) : Scheduler(_id, level) {
  #if _INFO==1
  cout << "Creating EventBased with ID: " << id << endl;
  #endif

  currentRunnable = NULL;

  //Subclasses should initialize the activeQueue
  //variable depending on their schedulable criteria

  //Initialize semaphores
  sem_init(&activation_sem, 0, 1);  //mutex semaphore
  sem_init(&event_sem, 0, 0);       //sem used as signal
  sem_init(&schedule_sem, 0, 1);    //mutex semaphore
}

//TODO: DESTRUCTOR

/*********** INHERITED FUNCTIONS ***********/

/**** FROM THREAD  ****/

///This function redefines Thread::join() to take into account the EventBased unblocking mechanism...
void EventBased::join() {

  #if _DEBUG==1
  cout << "Attempting to join EventBased\n";
  #endif

  sem_post(&event_sem);
  sem_post(&schedule_sem);
  sem_post(&activation_sem);

  if(parent!=NULL) {
    parent->join();
  }

  //after posting to all blocking sems, 
  join2();

}

/**** FROM RUNNABLE  ****/

///This function rewrites the activate method to 'activate' both the scheduler as well as its load - this runs in the higher level scheduler thread
void EventBased::activate() {

  #if _DEBUG==1
  cout << "Activating EventBased...\n";
  #endif

  if(state == activated) {
    cout << "EventBased::activate() error - already activated!\n";
  }

  sem_wait(&activation_sem);
    sem_wait(&schedule_sem);
      setPriority(Priorities::get_sched_pr(level));
      state = activated;

      if(currentRunnable != NULL) {
	currentRunnable->activate();
      }

      //if there are new jobs, wait for scheduler to process the new jobs himself
    sem_post(&schedule_sem); 
  sem_post(&activation_sem);

  #if _DEBUG==1
  cout << "Activated EventBased\n";
  #endif
}

///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
void EventBased::deactivate() {
  if(state == deactivated) {
    cout << "EventBased::deactivate() error - already deactivated!\n";
  }

  sem_wait(&activation_sem);
    sem_wait(&schedule_sem);
      //Deactivate currently active job (if any)
      if(currentRunnable != NULL) {
	currentRunnable->deactivate();
      }
      //Decrease the priority
      setPriority(Priorities::get_inactive_pr());
      state = deactivated;
    sem_post(&schedule_sem);
  sem_post(&activation_sem);

  #if _DEBUG==1
  cout << "Dectivated EventBased\n";
  #endif
}

/**** FROM RESOURCEALLOCATOR  ****/

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void EventBased::finishedJob(unsigned int runnable_id) {

  #if _DEBUG==1
  cout << "EventBased::finishedJob() is waiting for schedule_sem\n";
  #endif

  sem_wait(&schedule_sem);
    #if _DEBUG==1
    cout << "EventBased::finishedJob() is processing finishedJob\n";
    #endif
    finishedJobDeque.push_back(runnable_id); //Add id to finishedJobDeque

    #if _DEBUG==1
    cout << "EventBased::finishedJob() - Registering finishedJob & event...\n";
    #endif

    //Protecting posts to event_sem with sched_sem assures one event handled 
    //per post in the scheduler, otherwise, multiple jobs could be handled from just one post
    sem_post(&event_sem);//Register event with this scheduler

    #if _DEBUG==1
    cout << "finishedJob is done!\n";
    #endif
  sem_post(&schedule_sem);
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution. This is executed by the worker thread itself (always of a lower priority than its scheduler)
void EventBased::newJob(Runnable* r) {

  #if _DEBUG==1
  cout << "EventBased::newJob() is waiting for schedule_sem\n";
  #endif

  sem_wait(&schedule_sem);

    #if _DEBUG==1
    cout << "EventBased::newJob() registered newJob and event!\n";
    #endif

    //Notify parent of new head only when there was a change of head in the queue
    if(activeQueue->insertRunnable(r)) {
      if(parent!=NULL) {
	criteria = activeQueue->peek_front()->getCriteria();
	parent->newJob(this);
      }
      else if (level != 0) {
	cout << "EventBased::newJob() - non-top level entity has null parent!\n";
      }
    }

    #if _DEBUG==1
    cout << "EventBased::newJob() is done!\n";
    #endif

    sem_post(&event_sem); //register event
  sem_post(&schedule_sem);
}

///This function handles a job that had been queued by the worker. The worker object is thus already in the scheduler's queue, but now has a different schedulable criteria (and thus requires a change in the scheduling queue).
void EventBased::updateRunnable(Runnable* r) {
  #if _DEBUG==1
  cout << "EventBased::updateRunnable() is waiting for schedule_sem\n";
  #endif

  sem_wait(&schedule_sem);
    updateDeque.push_back(r);
    sem_post(&event_sem); //register event
  sem_post(&schedule_sem);
}

/**** FROM SCHEDULER  ****/

///This function performs the actual scheduling (figuring out the order of execution for its load)
void EventBased::schedule() {

  while(Simulation::isSimulating()) {

    //Wait for an event to ocurr
    sem_wait(&event_sem);

    #if _DEBUG==1
    cout << "EventBased::schedule() is handling an event!\n";
    #endif

    //If simulation ended while blocked, break
    if( !Simulation::isSimulating() )  break;
    
    sem_wait(&schedule_sem);

      #if _DEBUG==1
      cout << "EventBased::schedule() is processing an event: ";
      cout << newJobDeque.size() << ":" << finishedJobDeque.size() << ":" << updateDeque.size() << "\n";
      #endif

      //Deactivate currently active job (if any) in order to process new/finished jobs
      if(currentRunnable != NULL) {
        currentRunnable->deactivate();
      }

      /***** handle updates *****/
      if(updateDeque.size() > 0) {
        #if _DEBUG==1
	cout << "EventBased::schedule() is handling a runnable update!\n";
        #endif

	Runnable* r = updateDeque.front();

	if(r != NULL) {
	  activeQueue->deleteRunnable(r->getId());//erase from activeQueue
	  activeQueue->insertRunnable(r);//insert into activeQueue with updated criteria
	  updateDeque.pop_front();//erase from updateDeque
	}
	else {
	  cout << "EventBased::schedule() - runnable r is null!\n";
	}
	
	//Update this runnable's critera
	criteria = activeQueue->peek_front()->getCriteria();

	//Register it with the parent
	if(parent!=NULL) {
	  parent->updateRunnable(this);
	}
	else if (level != 0) {
	  cout << "EventBased::schedule() - non-top level entity has null parent!\n";
	}
      }
      /***** handle finished jobs *****/
      else if(finishedJobDeque.size() > 0) {
        #if _DEBUG==1
        cout << "EventBased::schedule() is handling a finished job!\n";
        #endif

	//erase from activeQueue
        if(!activeQueue->deleteRunnable(finishedJobDeque.front())) {
	  cout << "EventBased::schedule() error - finished runnable was not found!\n";
	}
        finishedJobDeque.pop_front();//erase from finishedJobDeque

	//If there are any pending jobs, update to new head's criteria and update with parent (if any)
	if(activeQueue->getSize() > 0) {
	  Runnable *aux = activeQueue->peek_front();

	  if(aux != NULL) {
	    criteria = aux->getCriteria();
	  }
	  else {
	    cout << "EventBased::schedule() - fnishedjob, activeQueue head is null!\n";
	  }
	  currentRunnable = activeQueue->peek_front();

	  //Register event with parent
	  if(parent!=NULL) {
	    parent->updateRunnable(this);
	  }
	  else if (level != 0) {
	    cout << "EventBased::schedule() - non-top level entity has null parent!\n";
	  }	  
	}
	//otherwise, simple register a finished job
	else {
          currentRunnable=NULL;
	  //Register event with parent
	  if(parent!=NULL) {
	    parent->finishedJob(id);
	  }
	  else if (level != 0) {
	    cout << "EventBased::schedule() - non-top level entity has null parent!\n";
	  }
	}
      }

      //activate the (possibly new) head of activeQueue
      if(activeQueue->getSize()>0) {
	#if _DEBUG==1
	cout << "EventBased::schedule() - activating: " << activeQueue->peek_front()->getId() << endl;
	#endif

	currentRunnable = activeQueue->peek_front();

	if(currentRunnable!=NULL) {
          currentRunnable->activate();
        }
	else {
	  cout << "EventBased::schedule() - current runnable is null (when it shouldn't)...\n";
        }
      }
      else {
        currentRunnable = NULL;
      }

      #if _DEBUG==1
      cout << "EventBased::schedule() is releasing sem...\n";
      #endif

    //Release sched_sem
    sem_post(&schedule_sem);
  }
}

