/*********************************
 *
 * Input parameter 1 .. N - names of SF3P simulations to publish
 *
 *********************************/

#include <iostream>
#include <stdlib.h>
#include <string>

using namespace std;

/*********** VARIABLES ***********/

///This variable holds the prefix for the input files (_traces, _runtimes, _missedDeadlines).
string prefix="simulation";

/*********** FUNCTIONS ***********/

///This function interprets all input parameters
void interpret(string str);

///This function executes, analyzes and shows the metrics of the input file
void publish(string f);

/*********** MAIN FUNCTION  ***********/
int main(int argn, char **argv) {
  string* aux;

#if _INFO==1
  cout << "\nStarting SF3P's publish tool\n\n";
#endif

  ///If there are input parameters, interpret them
  if(argn > 1) {
    for(int i=1;i<=argn;i++) {

      if(argv[i] == NULL)
	continue;

      aux = new string(argv[i]);
      interpret(*aux);
      free(aux);
    }
  }
  ///otherwise analyze all by default, 
  else {
    publish("simulation");
  }

#if _INFO==1
  cout << "\nDone." <<endl;
#endif

  return 0;
}

///This function executes, analyzes and shows the metrics of the input file
void publish(string prefix) {
  string aux;

  //Call sf3p
  aux = "$SF3P/scripts/bash/publish.sh " + prefix;
  system(aux.c_str());

  cout << "\n***   Published: " + prefix + ".hmtl\t***\n\n";

}

///This function interprets all input parameters
void interpret(string str) {

  //check if filename doesn't have an .xml extension, and if so add it
  if(str.find(".xml") != string::npos) {
    str = str.substr(0,str.size()-4);
  }

  publish(str);
}
