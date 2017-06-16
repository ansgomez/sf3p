SF3P Framework
===

***Andres Gomez, Lars Schor, Pratyush Kumar, Lothar Thiele***

***ETH Zurich***

### Publication

   [SF3P Paper](https://tik-old.ee.ethz.ch/file//884a2b54347c2b66b5c0ad082278a298/paper.pdf)
   
   Please cite as:
   
   Gomez A, Schor L, Kumar P, Thiele L.
   SF3P: A Framework to Explore and Prototype Hierarchical Compositions of Real-Time Schedulers.
   In Proceedings of Rapid System Prototyping (RSP) Symposium. 2014. IEEE.

### Requirements

   SF3P requires a *NIX kernel with standard libraries. To compile
   all sources, these packages are needed:
   
     g++
     make
     octave
     php
     libmgl-dev
     libX11-dev

  Mathgl library should be compiled, and libmgl.so.7.0.0 should be
  placed in /usr/local/lib (otherwise the MATHGL variable in the makefile 
  should be changed to the appriopiate location. For more information 
  on Mathgl, please visit:

     http://mathgl.sourceforge.net/

### Configuration

1. If you SF3P folder is not located in home folder, then please change 
   line 3 of SF3P_paths.sh to the path of your SF3P folder

2. In the terminal, type:
     
         source sf3p_paths.sh
   
   This will set a new $SF3P variable, and add it to your $PATH variable. 
   You can also add it to your ~/.bashrc file, to have it load automatically
   
   You need root privileges to execute SF3P. On some older systems, you might have
   to add the following line to your bash profile in order to inherit you PATH 
   variable when using 'sudo':
         
         alias sudo='sudo env PATH=$PATH $@' 

3. Then type:

         ./install.sh

4. Run SF3P!

   You can now type in you terminal the following commands:
   
     sudo SF3P [filename(.xml)]
     simulate [filename(.xml)]
     calculate [metric] [filename]
     show [metric] [filename]
     simfig [filename]
   
     [metric] can be one (or more) of the following:
   
       exe|exec     ->  Execution Times
       resp         ->  Response Times
       throughput   ->  Throughput
       util         ->  Utilization
       alloc        ->  Resource allocation costs
       sys          ->  System allocation costs
       worker       ->  Worker costs
       missed       ->  Missed deadlines


### Examples

1. Type "cd examples"

    Description: This is where all the sample files are located. These are the default ones:
                 Each file describes the topology for different schedulers and parameters
                 such as simulation time, task periods, WCET, etc.

        edf.xml
        fifo.xml
        fixedpriority.xml
        fixedpriority_top.xml
        tdma.xml
        tdma_top.xml

2. Run the simulation, type "simulate edf"

    Description: 

        Simulation Phase
                 This will run the simulation, and generate several output files which 
                 need to be processed. They include timestamps and thread runtimes used
                 to determine metrics such as missed deadlines, and scheduling costs.

        Post-Processing Phase
                 This will process all the log files and calculate all releveant metrics. 
                 The results, with some basic statistics will be shown on screen.


    Metrics:

         Resource allocation cost - overhead from scheduling and context switching

         System Cost - includes also the 'dispatching' overhead, or generating tasks arrivals
