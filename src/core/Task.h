#ifndef _TASK_H
#define _TASK_H

#include "util/Enumerations.h"

#include <time.h>

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/
 
class Task {
 
 protected:
 
  /*********** VARIABLES ***********/
  
  ///Varible describing the task's type of load (video, busy_wait)
  _task_load task_load;

 public:
 
  /*********** CONSTRUCTOR ***********/
  
  ///Constructor needs pointer to simulation, dispatcher, and its load type
  Task(_task_load load);

  /*********** MEMBER FUNCTIONS ***********/

  ///This function performs one task (whether it's decoding one frame, or one busy-wait loop)
  virtual void fire();

};
#endif
