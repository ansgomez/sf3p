#ifndef _RM_H
#define _RM_H

#include "schedulers/EventBased.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class RateMonotonic : public EventBased {

 public: 

  /*********** CONSTRUCTOR ***********/
 
  ///Constructor needs pointer to simulation, id, and its level
  RateMonotonic(unsigned int id, int level);
};

#endif
