#include <assert.h>

#include "disk.h"

int main(int argc, char* argv[]) {
  assert(make_disk("disk.dat") == 0);
  return 0;
}
