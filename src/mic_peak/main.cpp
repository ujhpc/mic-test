#include <cmdline.h>
#include <cstdio>
#include <omp.h>

#include "common/timer.h"

#define FLOPS_ARRAY_SIZE (1024 * 1024)
#define LOOP_COUNT 128
// number of float pt ops per calculation
#define FLOPSPERCALC 2

// define some arrays - make sure they are 64 byte
// aligned - for fastest cache access
float fa[FLOPS_ARRAY_SIZE] __attribute__((align(64)));
float fb[FLOPS_ARRAY_SIZE] __attribute__((align(64)));

int main(int argc, char* argv[]) {

  try {
    cmdline::parser cl;
    cl.parse_check(argc, argv);

    std::cout << "Initializing" << std::endl;

    omp_set_num_threads(2);
    // kmp_set_defaults("KMP_AFFINITY=compact");

#pragma omp parallel for
    for (auto i = 0; i < FLOPS_ARRAY_SIZE; i++) {
      fa[i] = (float)i + 0.1f;
      fb[i] = (float)i + 0.2f;
    }

    int max_threads = omp_get_max_threads();
    std::cout << "Starting compute on" << max_threads << "threads"
              << std::endl;

    Timer timer;

    float a = 1.1f;
    int iters = 1000000 * cl.get<int>("iter");

#pragma omp parallel for
    for (int i = 0; i < max_threads; ++i) {
      // each thread will work its own array section
      int offset = i * LOOP_COUNT;
      // loop many times to get lots of calculations
      for (int j = 0; j < iters; ++j) {
        // scale 1st array and add in the 2nd array
        for (int k = 0; k < LOOP_COUNT; ++k) {
          fa[k + offset] = a * fa[k + offset] + fb[k + offset];
        }
      }
    }

    double gflops = (double)(1e-9 * max_threads * LOOP_COUNT * iters *
                             FLOPSPERCALC);
    // elasped time
    double sec = timer();
    std::cout << "Gflops = " << gflops << std::endl;
    std::cout << "secs = " << sec << std::endl;
    std::cout << "Gflops/s = " << (gflops / sec) << std::endl;
  }
  catch (std::string& ex) {
    std::cerr << "error: " << ex << std::endl;
  }
  catch (const char* ex) {
    std::cerr << "error: " << ex << std::endl;
  }
}
