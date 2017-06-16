#include <iostream>

#include "core/Simulation.h"

#include <stdlib.h>
#include <sys/stat.h>

using namespace std;

int main(int argc, char** argv) {

  Simulation* sim;
  string* file;
  struct stat buf;

  //If XML file names are received as input
  if(argc > 1) {

    //Iterate through all input names
    for(int i=1;i<=argc;i++) {

      if(argv[i] == NULL)
	continue;

      file = new string(argv[i]);

      if(file->find(".xml") == string::npos) {
	*file = *file + ".xml";
      }

      //if file exists, simulate it!
      if (stat(file->c_str(), &buf) != -1) {
	sim = new Simulation(*file, 1);
	sim->simulate();

	if (argc > 2)
	  cout << "\nFinished simulation #" << i << " ...\n\n";

	free(sim);
      }
      //else, show error
      else {
	cout << "\nFile '" << *file << "' does not exist!\n";
      }

      free(file);
    }
  }
  //Else look for default simulation.xml
  else {
    file = new string("simulation.xml");

    //if file exists, simulate it!
    if (stat(file->c_str(), &buf) != -1) {
      sim = new Simulation("simulation.xml", 1);
      sim->simulate();
      free(sim);
    }
    //else, show error
    else {
      cout << "\nFile '" << *file << "' does not exist!\n";
    }

    free(file);
  }

#if _INFO == 1
  cout << "Exiting main\n";
#endif

  return 0;
}
