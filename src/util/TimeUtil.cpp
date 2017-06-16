#include "util/TimeUtil.h"

#include "util/Enumerations.h"
#include "util/Clock.h"
#include "util/Operators.h"

#include <time.h>

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** STATIC VARIABLES ***********/

struct timespec TimeUtil::offset;

/*********** MEMBER FUNCTIONS ***********/

///This function returns a timespec with the current time
struct timespec TimeUtil::getTime()
{
  struct timespec aux;
  clock_gettime(SF3P_CLOCK, &aux);
  return aux;
}

///This function returns a timespec with the current time
struct timespec TimeUtil::getTime(enum _time_type type)
{
  struct timespec aux;
  clock_gettime(SF3P_CLOCK, &aux);
  
  //if(type == relative) {
    aux = aux - offset;
    //}

  return aux;
}

///This function returns an unsigned long int with the current time in microseconds
unsigned long int TimeUtil::getTimeUSec()
{
  struct timespec aux;
  clock_gettime(SF3P_CLOCK, &aux);
  
  aux = aux - offset;

  return convert_us(aux);
}

///This function converts a timestamp from absolute time to relative simulation time
struct timespec TimeUtil::relative(struct timespec absolute) {
  return absolute - offset;
}

///Sets the relative-time offset to current time
void TimeUtil::setOffset()
{
  clock_gettime(SF3P_CLOCK, &offset);
}

/*********** AUXILIARY FUNCTIONS ***********/

///Converts a timespec to unsigned long usecs
unsigned long int TimeUtil::convert_us(struct timespec t1)
{
  unsigned long int ul = (unsigned long int) (t1.tv_sec)*1000000 + (unsigned long int)(t1.tv_nsec)/1000;
  return ul;
}

///Converts a timespec to unsigned long msecs
unsigned long int TimeUtil::convert_ms(struct timespec t1) {
  unsigned long int ul = (unsigned long int) (t1.tv_sec)*1000 + (unsigned long int)(t1.tv_nsec)/1000000;
  return ul;
}

///This function returns a timespec with the specified microseconds
struct timespec TimeUtil::Micros(long int us) {
  struct timespec aux;

  if(us < 1000000) {
    aux.tv_sec = 0;
    aux.tv_nsec = us*1000;
  }
  else {
    aux.tv_sec = (int) (us/1000000);
    aux.tv_nsec = (us-aux.tv_sec*1000000)*1000;
  }

  return aux;
}

///This function returns a timespec with the specified milliseconds
struct timespec TimeUtil::Millis(long int ms) {
  struct timespec aux;


  if(ms < 1000) {
    aux.tv_sec = 0;
    aux.tv_nsec = ms*1000000;
  }
  else {
    aux.tv_sec = (int) (ms/1000);
    aux.tv_nsec = (ms-aux.tv_sec*1000)*1000000;
  }

  return aux;
}

///This function returns a timespec with the specified seconds
struct timespec TimeUtil::Seconds(__time_t s) {
  struct timespec aux;

  aux.tv_sec = s;
  aux.tv_nsec = 0;

  return aux;
}
