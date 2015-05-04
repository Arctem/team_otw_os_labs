#include <assert.h>

#include "disk.h"

int main(int argc, char* argv[]) {
  assert(make_fs("disk.dat") == 0);
  assert(mount_fs("disk.dat") == 0);
  assert(umount_fs("disk.dat") == 0);
  assert(mount_fs("disk.dat") == 0);
  assert(umount_fs("disk.dat") == 0);
  return 0;
}
