#include "schedulers/TDMA.h"

#include "core/Runnable.h"
#include "pthread/Priorities.h"
#include "results/Trace.h"
#include "util/Enumerations.h"
#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <errno.h>
#include <iostream>
#include <time.h>

#define _INFO 0

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

///Constructor needs pointer to simulation
TDMA::TDMA(unsigned int _id, int level) : Scheduler(_id, level) {
  #if _INFO==1
  cout << "Creating TDMA with ID: " << id << endl;
  #endif

  sem_init(&activation_sem, 0, 1); //mutex semaphore
  sem_init(&schedule_sem, 0, 0); //mutex semaphore -> initialized to 0 because of activate()
  sem_init(&timing_sem, 0, 1); //mutex semaphore
  sem_init(&preempt_sem, 0, 0); //sem used as signal

  active_index = -1;
  sched_type = tdma;
  state = deactivated;
}

/*********** INHERITED FUNCTIONS ***********/
    
/**** FROM THREAD ****/

///This function rewrites the join method to account for the scheduler's load (they are all joined)
void TDMA::join() {
  if(parent!=NULL){
    parent->join();
  }

  join2();
}

/**** FROM RUNNABLE ****/

///This function rewrites the activate method to activate both the scheduler(through its semaphores) as well as its load - this runs in the dispatcher thread
void TDMA::activate() {
  if(state == activated) {
    cout << "TDMA::activate error - already activated!\n";
  }

  sem_wait(&activation_sem);
    sem_post(&schedule_sem);
    setPriority(Priorities::get_sched_pr(level));
    state = activated;
  sem_post(&activation_sem);
}

///This function rewrites the deactivate method both the scheduler (through its semaphores) as well as its load
void TDMA::deactivate() {
  int sts;

  if(state == deactivated) {
    cout << "TDMA::deactivate error - already deactivated!\n";
  }

  sem_wait(&activation_sem);
    sts = sem_trywait(&timing_sem);
    //If the scheduler isn't timing, no need to do anything (just wait for sem to be freed)
    if (sts == 0) {
      sem_post(&timing_sem);
    }
    else { 
      //If the scheduler has timing_sem locked, then it must be preempted
      sem_post(&preempt_sem);
    } 
    sem_wait(&schedule_sem);
    //now decrease the priority
    setPriority(Priorities::get_inactive_pr());
    state = deactivated;
  sem_post(&activation_sem);
}

/**** FROM RESOURCEALLOCATOR ****/

///This function handles the end of a job in its load. Depending on the scheduling, this could change the order of execution.
void TDMA::finishedJob(unsigned int worker_id){
  if (parent != NULL) {
    parent->finishedJob(worker);
  }
}

///This function handles a new job in its load. Depending on the scheduling, this could change the order of execution.
void TDMA::newJob(Runnable *obj) {

  #if _INFO==1
  cout << "TDMA handled runnable " << obj->getId() << "'s new job\n";
  #endif

  if (parent != NULL) {
    parent->newJob(obj);
  }
  //Unless it's the top level entity, null parent is a problem!
  else if (level != 0) {
    cout << "TDMA::newJob - non top-level entity has null parent!\n";
  }

}

///This function handles a change in the criteria of an active Runnable. This might lead to the calling Runnable to be moved from the head of the activeQueue to another position and thus cause a change in the order of execution.
void TDMA::updateRunnable(Runnable* r) {
  if(parent!=NULL) {
    criteria = r->getCriteria();
    parent->updateRunnable(this);
  }
}

/**** FROM SCHEDULER ****/

///This function performs the actual scheduling (figuring out the order of execution for its load)
void TDMA::schedule(){
  struct timespec timeA, timeB, time_slot, aux;
  bool exit = false;
  int ret, err_no;
  active_index = -1;

  #if _DEBUG == 1
  cout << "Began scheduling...\n";
  #endif

  while (Simulation::isSimulating()) {
    
    for(active_index=0; active_index<(int)timeslots.size() && Simulation::isSimulating(); active_index++) {

      time_slot = timeslots[active_index];
      exit = false;
      while(exit == false && Simulation::isSimulating() ) {
        sem_wait(&schedule_sem);
          //If simulation ended while asleep, break
          if( !Simulation::isSimulating() ) {
            sem_post(&schedule_sem);
            break;
          }

	  /**** TIMED WAIT CALL ****/
          sem_wait(&timing_sem);
	    timeA= TimeUtil::getTime(); 
            if(load[active_index] != NULL) {
              load[active_index]->activate();
            }
            aux = timeA + time_slot;
            ret = sem_timedwait(&preempt_sem, &aux);
            err_no = errno;
          sem_post(&timing_sem);
	  /**** END OF TIMED WAIT CALL ****/


          //If simulation ended while asleep, break
          if( !Simulation::isSimulating() ) {
            sem_post(&schedule_sem);
            break;
          }

          if(load[active_index] != NULL) {
            load[active_index]->deactivate();
          }

	  /**** HANDLE EVENT (EITHER SIGNAL, OR TIMEDOUT ****/
          if(ret == -1) {
            //if timeslot was exhausted, pass on to the next time slot
            if(err_no == ETIMEDOUT) {
              sem_post(&schedule_sem);
              exit = true;
            }
            else if (err_no==EINVAL) {
              cout << "TDMA::schedule: EINVAL ERROR\n";
            }
            else if (err_no==EAGAIN) {
              cout << "TDMA::schedule: EAGAIN ERROR\n";
            }
            else {
              cout << "TDMA::schedule: semaphore error (" << errno << ") - " << strerror(errno) << "\n";
	    }
          }
          //If the call received a signal, it is being deactivated
          else {
	    timeB = TimeUtil::getTime();
            time_slot = time_slot - (timeB-timeA);
            if(time_slot < TimeUtil::Millis(0)) {
              exit = true;
            }
            sem_post(&schedule_sem);
          }
       }//end of while(exit==false)
    }//end of foreach(timeslot)
    active_index = -1;
  }//end of while(simulating)
}//end of function


/*********** MEMBER FUNCTIONS ***********/

///This function adds a load to the scheduler (could be another scheduler, or a worker)
void TDMA::add_load(Runnable *new_load) {
  load.push_back(new_load);
}

///This function adds a timeslot to the scheduler. They are assigned in the same order as the load is defined
void TDMA::add_slot(struct timespec slot) {
  timeslots.push_back(slot);
}
