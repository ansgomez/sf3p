#ifndef _FIXEDPRIORITY_H
#define _FIXEDPRIORITY_H

#include "schedulers/EventBased.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class FixedPriority : public EventBased {

 public: 

  /*********** CONSTRUCTOR ***********/
 
  ///Constructor needs pointer to simulation, id, and its level
  FixedPriority(unsigned int id, int level);
};

#endif
