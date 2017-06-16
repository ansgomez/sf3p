#include "criteria/InclusiveCriteria.h"

#include "util/Operators.h"
#include "util/TimeUtil.h"

#include <iostream>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

InclusiveCriteria::InclusiveCriteria() : Criteria() {
  relativeDeadline = TimeUtil::Millis(0);
}

/*********** INHERITED FUNCTIONS ***********/

/**** FROM CRITERIA ****/

///This function returns the current job's arrivalTime (according to its criteria policy)
struct timespec InclusiveCriteria::getArrivalTime() {
  return arrivalTime;
}

///This function returns the object's deadline (according to its criteria policy)
struct timespec InclusiveCriteria::getDeadline() {
  return absoluteDeadline;
}
  
///This function returns the current job's period (according to its criteria policy)
struct timespec InclusiveCriteria::getPeriod() {
  return period;
}

///This function prints an alert.
int InclusiveCriteria::getPriority() { 
  return priority;
}

///This function returns the object's relative deadline
struct timespec InclusiveCriteria::getRelativeDeadline() {
  return relativeDeadline;
}

///This function sets a new arrivalTime
void InclusiveCriteria::setArrivalTime(struct timespec at) {
  arrivalTime = at;
}

///This function sets the absoluteDeadline equal to arrivalTime+Criteria::relativeDeadline
void InclusiveCriteria::setDeadline(struct timespec arrivalTime) {
  absoluteDeadline = arrivalTime + relativeDeadline;
}
  
///This function sets a new period
void InclusiveCriteria::setPeriod(struct timespec p) {
  period = p;
}

///This function sets the new priority
void InclusiveCriteria::setPriority(int pr) {
  priority = pr;
}

///This function sets a new relative deadline
void InclusiveCriteria::setRelativeDeadline(struct timespec rel) {
  relativeDeadline = rel;
}
