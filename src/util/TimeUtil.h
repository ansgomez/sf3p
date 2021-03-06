#ifndef _TIMEUTIL_H
#define _TIMEUTIL_H

#include "util/Enumerations.h"

#include <stdlib.h>
#include <time.h>

/***************************************
 *        CLASS DECLARATION            * 
 ***************************************/

class TimeUtil {

  public:

  /*********** VARIABLES ***********/

  ///This variable holds the (absolute) time at which the simulation started (used to calculate relative time)
  static struct timespec offset;

  /*********** MEMBER FUNCTIONS ***********/

  ///This function returns a timespec with the current time
  static struct timespec getTime();

  ///This function returns a timespec with the current time
  static struct timespec getTime(enum _time_type type);

  ///This function returns an unsigned long int with the current time in microseconds
  static unsigned long int getTimeUSec();

  ///This function converts a timestamp from absolute time to relative simulation time
  static struct timespec relative(struct timespec absolute);

  //Sets the relative-time offset to current time
  static void setOffset();

  /*********** AUXILIARY FUNCTIONS ***********/

  ///Converts a timespec to unsigned long usecs
  static unsigned long int convert_us(struct timespec t1);

  ///Converts a timespec to unsigned long msecs
  static unsigned long int convert_ms(struct timespec t1);

  ///This function returns a timespec with the specified microseconds
  static struct timespec Micros(long int us);

  ///This function returns a timespec with the specified milliseconds
  static struct timespec Millis(long int ms);

  ///This function returns a timespec with the specified seconds
  static struct timespec Seconds(__time_t s);
};

#endif
