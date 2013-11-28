#include <cmdline.h>
#include <cstdio>
#include <omp.h>

#include "common/timer.h"

// Set to float or double
#define REAL double
#define BW_ARRAY_SIZE (1000 * 1000 * 64)
// number of mem ops each iteration
// 1 read + 1 write
#define OPS_PERITER 2
// define some arrays
// make sure they are 64 byte aligned - for fastest cache
// access
REAL fa[BW_ARRAY_SIZE] __attribute__((align(64)));
REAL fb[BW_ARRAY_SIZE] __attribute__((align(64)));
REAL fc[BW_ARRAY_SIZE] __attribute__((align(64)));

int main(int argc, char* argv[]) {

  try {
    cmdline::parser cl;
    cl.add<int>("threads", 't', "number of threads", false);
    cl.add<int>("iter", 'i', "millions of iterations", false, 1000);
    cl.parse_check(argc, argv);

    std::cout << "Initializing" << std::endl;

    if (cl.exist("threads")) {
      omp_set_num_threads(cl.get<int>("threads"));
    }

    REAL a = 1.1;
#pragma omp parallel for
    for (int i = 0; i < BW_ARRAY_SIZE; ++i) {
      fa[i] = (REAL)i + 0.1;
      fb[i] = (REAL)i + 0.2;
      fc[i] = (REAL)i + 0.3;
    }

    int max_threads = omp_get_max_threads();
    std::cout << "Starting bandwidth test on " << max_threads << " threads"
              << std::endl;

    Timer timer;

    // use omp to scale the test across
    // the threads requested. Need to set environment
    // variables OMP_NUM_THREADS and KMP_AFFINITY
    int iters = cl.get<int>("iter");
    for (int i = 0; i < iters; ++i) {
#pragma omp parallel for
      // copy the arrays to/from memory (2 bw ops)
      // use openmp to scale and get aggregate bw
      for (int k = 0; k < BW_ARRAY_SIZE; ++k) {
        fa[k] = fb[k];
      }
    }

    // # of gigabytes we just moved
    double gbytes =
        (double)(1e-9 * OPS_PERITER * iters * BW_ARRAY_SIZE * sizeof(REAL));
    // elasped time
    double sec = timer();
    std::cout << "GB = " << gbytes << std::endl;
    std::cout << "secs = " << sec << std::endl;
    std::cout << "GB/s = " << (gbytes / sec) << std::endl;
  }
  catch (std::string& ex) {
    std::cerr << "error: " << ex << std::endl;
  }
  catch (const char* ex) {
    std::cerr << "error: " << ex << std::endl;
  }
}
