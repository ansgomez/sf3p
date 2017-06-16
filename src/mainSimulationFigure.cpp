#include "../lib/mathgl/SimulationFigure.h"

#include <iostream>
#include <sys/stat.h>

int main(int argn, char **argv) {
#if _INFO==1
  cout << "\nStarting SF3P's simfig tool\n";
#endif

  cout << endl;

  SimulationFigure *sf; 
  string *prefix, file;
  struct stat buf;

  //If any file prefix is passed as argument
  if(argn > 1) {
    for(int i=1;i<=argn;i++) {
      if(argv[i] == NULL)
	continue;

      prefix = new string(argv[i]);
      file = *prefix + "_traces.csv";

      //if file exists, simulate it!
      if (stat(file.c_str(), &buf) != -1) {
	//Create object, automatically imports CSV file
	sf = new SimulationFigure(*prefix);
	sf->generateFigures();
	//sf->generateAnimation();
	free(sf);
      }
      //else, show error
      else {
	cout << "\nFile '" << file << "' does not exist!\n";
      }

      free(prefix);
    }
  }
  //Else look for default 'simulation' prefix
  else {
    file = "simulation_traces.csv";

    //if file exists, simulate it!
    if (stat(file.c_str(), &buf) != -1) {
      //Create object, automatically imports CSV file
      sf = new SimulationFigure("simulation");
      sf->generateFigures();
      //sf->generateAnimation();
      free(sf);
    }
    //else, show error
    else {
      cout << "\nFile '" << file << "' does not exist!\n";
    }
  }

  cout << endl;

  return 0;
}
