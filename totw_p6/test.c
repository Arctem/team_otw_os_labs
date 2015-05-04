#include <assert.h>

#include "disk.h"
#include "fs.h"

int main(int argc, char* argv[]) {
  int f = 0;
  
  assert(make_fs("disk.dat") == 0);
  assert(mount_fs("disk.dat") == 0);
  assert(fs_create("file.test") == 0);
  f = fs_open("file.test");
  assert(f == 0);
  assert(fs_get_filesize(f) == 0);
  assert(fs_write(f, (void*) "hello", 6) == 6);
  assert(fs_get_filesize(f) == 6);
  assert(fs_close(f) == 0);
  assert(umount_fs("disk.dat") == 0);

  assert(mount_fs("disk.dat") == 0);
  f = fs_open("file.test");
  assert(f == 0);
  assert(fs_get_filesize(f) == 6);
  assert(fs_close(f) == 0);
  assert(fs_delete("file.text") == 0);
  assert(fs_open("file.test") == -1);
  assert(umount_fs("disk.dat") == 0);
  return 0;
}
