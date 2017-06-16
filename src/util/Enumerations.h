#ifndef _ENUMERATIONS_H
#define _ENUMERATIONS_H

///This enumeration describes the two runnable states
enum _runnable_state {
  activated,
  deactivated
};

///This enumeration describes the different scheduler types
enum _sched_type {
  edf,
  fifo,
  fp,
  tdma,
  rm
};

//This enumeration describes the different scerver types
enum _serv_type {
  cbs,
  dbs
};

///This enum describes all traced actions
enum _task_action {
  task_arrival,
  sched_start,
  sched_end,
  task_start,
  task_end,
  deadline_met,
  deadline_missed,
  sim_end
};

///This enumeration describes the possible loads for a worker
enum _task_load {
  busy_wait,
  video
};

///This enumeration descibes the possible task models
enum _task_periodicity {
  aperiodic,
  periodic,
  periodic_jitter
};

///This enum describes what thread types can produce traces
enum _thread_type {
  idle,
  scheduler,
  dispatcher,
  server,
  worker
};


///This enumeration describes the two time types
enum _time_type {
  absolute,
  relative
};

#endif
