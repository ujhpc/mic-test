#include <time.h>

class Timer {
 private:
#ifdef __linux__
  typedef struct timespec TimeType;
  static void get_time(TimeType& time) { clock_gettime(CLOCK_REALTIME, &time); }
#else
  typedef struct timeval TimeType;
  static get_time(TimeType& time) { gettimeofday(&time, NULL); }
#endif
  TimeType start;

 public:
  Timer() { get_time(start); }

  double operator()() {
    TimeType now;
    get_time(now);
    return (double)(now.tv_sec - start.tv_sec) +
           1e-9 * (now.tv_nsec - start.tv_nsec);
  }
};
