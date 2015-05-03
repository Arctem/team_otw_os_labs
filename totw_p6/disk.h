#ifndef _DISK_H_
#define _DISK_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

/******************************************************************************/
#define DISK_BLOCKS  8192      /* number of blocks on the disk                */
#define BLOCK_SIZE   4096      /* block size on "disk"                        */

/******************************************************************************/
int make_disk(char *name);     /* create an empty, virtual disk file          */
int open_disk(char *name);     /* open a virtual disk (file)                  */
int close_disk();              /* close a previously opened disk (file)       */

int block_write(int block, char *buf);
                               /* write a block of size BLOCK_SIZE to disk    */
int block_read(int block, char *buf);
                               /* read a block of size BLOCK_SIZE from disk   */
/******************************************************************************/

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
