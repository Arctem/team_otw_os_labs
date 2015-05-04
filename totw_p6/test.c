#include <assert.h>

#include "disk.h"
#include "fs.h"

int main(int argc, char* argv[]) {
  int f = 0, f2 = 0;
  char test[100] = {0};
  
  assert(make_fs("disk.dat") == 0);
  assert(mount_fs("disk.dat") == 0);
  assert(fs_create("file.test") == 0);
  f = fs_open("file.test");
  assert(f == 0);
  assert(fs_get_filesize(f) == 0);
  assert(fs_write(f, (void*) "hello", 6) == 6);
  assert(fs_get_filesize(f) == 6);
  assert(fs_lseek(f, 100) == -1);
  assert(fs_lseek(f, 0) == 0);
  assert(fs_write(f, (void*) "hello", 6) == 6);
  assert(fs_get_filesize(f) == 6);
  assert(fs_read(f, test, 10) == 0);
  assert(fs_lseek(f, 0) == 0);
  assert(fs_read(f, test, 10) == 6);
  assert(strncmp("hello", test, 6) == 0);

  assert(fs_create("other.test") == 0);
  f2 = fs_open("other.test");
  assert(f2 == 1);
  
  assert(fs_close(f) == 0);
  assert(fs_close(f2) == 0);
  assert(umount_fs("disk.dat") == 0);

  memset(test, 0, 100);

  assert(mount_fs("disk.dat") == 0);
  f = fs_open("file.test");
  assert(f == 0);
  assert(fs_get_filesize(f) == 6);
  assert(fs_read(f, test, 10) == 6);
  assert(strncmp("hello", test, 6) == 0);
  assert(fs_close(f) == 0);
  assert(fs_delete("file.text") == 0);
  assert(fs_open("file.test") == -1);
  assert(umount_fs("disk.dat") == 0);
  return 0;
}
