#ifndef _EDF_H
#define _EDF_H

#include "schedulers/EventBased.h"

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class EDF : public EventBased {

 public: 

  /*********** CONSTRUCTOR ***********/
 
  ///Constructor needs pointer to simulation, id, and its level
  EDF(unsigned int id, int level);

};

#endif
