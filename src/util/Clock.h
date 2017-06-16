#ifndef SF3PCLOCK_H
#define SF3PCLOCK_H

#if MONOTONIC == 1
  #define SF3P_CLOCK CLOCK_MONOTONIC
#else
  #define SF3P_CLOCK CLOCK_REALTIME
#endif

#endif
