/*********************************
 *
 * Input parameter 1 .. N - names of SF3P xml files to execute
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
void simulate(string f);

/*********** MAIN FUNCTION  ***********/
int main(int argn, char **argv) {
  string* aux;

#if _INFO==1
  cout << "\nStarting SF3P's simulate tool\n\n";
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
    simulate("simulation");
  }

#if _INFO==1
  cout << "\nDone." <<endl;
#endif

  return 0;
}

///This function executes, analyzes and shows the metrics of the input file
void simulate(string prefix) {
  string aux;

  //Clear previous results
  aux = "rm -f *.csv ";
  system(aux.c_str());

  //Call sf3p
  aux = "sudo $SF3P/bin/SF3P " + prefix;
  system(aux.c_str());

  //Call calculate
  aux = "calculate " + prefix;
  system(aux.c_str());  

  //Call simfig
  //aux = "simfig " + prefix;
  //system(aux.c_str());

  //Call show
  aux = "show " + prefix;
  system(aux.c_str());

  //Call publish
  //aux = "publish " + prefix;
  //system(aux.c_str());
}

///This function interprets all input parameters
void interpret(string str) {

  //check if filename doesn't have an .xml extension, and if so add it
  if(str.find(".xml") != string::npos) {
    str = str.substr(0,str.size()-4);
  }

  simulate(str);
}
