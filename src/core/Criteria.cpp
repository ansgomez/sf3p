#include "core/Criteria.h"

#include "util/TimeUtil.h"

#include <iostream>
#include <sstream>

using namespace std;

/***************************************
 *        CLASS DEFINITION             * 
 ***************************************/

/*********** CONSTRUCTOR ***********/

Criteria::Criteria() {
  arrivalTime = TimeUtil::Millis(0);
  absoluteDeadline = TimeUtil::Millis(0);
  period = TimeUtil::Millis(0);
  priority = 0;
}

/*********** GETTERS AND SETTERS ***********/

///This function returns the current job's arrivalTime (according to its criteria policy)
struct timespec Criteria::getArrivalTime() {
  cout << "Criteria::getArrivalTime() - this should not print!\n";
  return arrivalTime;
}

///This function returns the object's deadline (according to its criteria policy)
struct timespec Criteria::getDeadline() {
  cout << "Criteria::getDeadline() - this should not print!\n";
  return absoluteDeadline;
}

///This function returns the current job's period (according to its criteria policy)
struct timespec Criteria::getPeriod() {
  cout << "Criteria::getPeriod() - this should not print!\n";
  return period;  
}
  
///This function returns the object's priority (according to its criteria policy)
int Criteria::getPriority() {
  cout << "Criteria::getPriority() - this should not print!\n";
  return priority;
}

///This function returns the object's relative deadline
struct timespec Criteria::getRelativeDeadline() {
  cout << "Criteria::getRelativeDeadline() - this should not print!\n";
  return relativeDeadline;
}

///This function sets a new arrivalTime
void Criteria::setArrivalTime(struct timespec at) {
  cout << "Criteria::setArrivalTime() - this should not print!\n";
}

///This function sets a new deadline
void Criteria::setDeadline(struct timespec dln) {
  cout << "Criteria::setDeadline() - this should not print!\n";
}

///This function sets a new period
void Criteria::setPeriod(struct timespec p) {
  cout << "Criteria::setPeriod() - this should not print!\n";  
}

///This function sets the new priority
void Criteria::setPriority(int pr) {
  cout << "Criteria::setPriority() - this should not print!\n";
}

///This function sets a new relative deadline
void Criteria::setRelativeDeadline(struct timespec rel) {
  cout << "Criteria::setRelativeDeadline() - this should not print!\n";
}

///This auxiliaty function return all criteria variables in a string
string Criteria::toString() {
  stringstream aux;

  aux << "Arrival Time : " << arrivalTime.tv_sec << "." << arrivalTime.tv_nsec << endl;
  aux << "Relative Deadline: " << relativeDeadline.tv_sec << "." << relativeDeadline.tv_nsec << endl;
  aux << "Absolute Deadline: " << absoluteDeadline.tv_sec << "." << absoluteDeadline.tv_nsec << endl;
  aux << "Period: " << period.tv_sec << "." << period.tv_nsec << endl;
  aux << "Priority: " << priority << endl;

  return aux.str();
}
