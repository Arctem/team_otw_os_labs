#ifndef _FS_H_
#define _FS_H_

#include <stdlib.h>

#include "disk.h"

#define NUM_FILES 64        /* the max allowed number of files on the disk */
#define OPEN_FILES 32       /* the max allowed number of file descriptors */

typedef struct file_meta_t {
  char in_use;
  char name[15]; /* name of the file, not null-terminated */
  short num_blocks; /* how many blocks the file uses */
  short size_last; /* how much of the last block is in use */
  short blocks[DISK_BLOCKS / 2];
} file_meta;
typedef struct descriptor_t {
  char file;
  int cursor;
} descriptor;

/******************************************************************************
 * File system structure:
 * 4096 blocks of "meta" (most unused).
 * 4096 blocks of data (use depends on files that have been created).
 *
 * Block 0 of meta data is a simple byte list representing which data blocks
 * are currently in use. Each byte is either 0 (false) or 1 (true) and corresponds
 * to one of the data blocks. That is, byte i represents the status of data block
 * i (located at block 4096 + i).
 *
 * Blocks 1 through 192 are used to store file data. These blocks are divided into
 * 64 3-block groups, as there is a limit of 64 files in the file system at any
 * given time. The first block of each group is an info block while the second two
 * store the list of blocks used by that file.
 * The structure of the info block is as follows:
 * 1 byte containing either 0 (false) or 1 (true) representing if the file exists.
 * 25 bytes representing the file's name. Is null-terminated if under 25 characters long.
 * A short of 2 bytes indicating how many blocks the file uses.
 * A short of 2 bytes indicating how much of the final block the file uses.
 *
 * The structure of the list blocks are as follows:
 * 2048 2-byte shorts representing a data block the file uses. A file's data blocks
 * may not necessarily be sequential. Both blocks together contain 4096 entries, allowing
 * for a file to potentially use up all 4096 data blocks. Theoretically all entries past
 * the number of blocks indicated by the grou's info block should be 0, but they will
 * be ignored so their exact value should not be important.
 ******************************************************************************/

int make_fs(char* disk_name); /* make a file system */
int mount_fs(char* disk_name); /* mount a file system */
int umount_fs(char* disk_name); /* un-mount a file system */

int fs_open(char *name); /* open file for reading and writing */
int fs_close(int filedes); /* close file */
int fs_create(char *name); /* create file */
int fs_delete(char *name); /* delete file */
int fs_read(int fildes, void *buf, size_t nbyte); /* read file */
int fs_write(int fildes, void *buf, size_t nbyte); /* write to file */
int fs_get_filesize(int fildes); /* get the current size of the file */
int fs_lseek(int fildes, off_t offset); /* set file pointer to offset */
int fs_truncate(int fildes, off_t length); /* truncate file to length */

#endif
