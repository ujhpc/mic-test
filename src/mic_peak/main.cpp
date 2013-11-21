#include <cmdline.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
  try {
    cmdline::parser cl;
    cl.parse_check(argc, argv);
  }
  catch (std::string& ex) {
    std::cerr << "error: " << ex << std::endl;
  }
  catch (const char* ex) {
    std::cerr << "error: " << ex << std::endl;
  }
}
