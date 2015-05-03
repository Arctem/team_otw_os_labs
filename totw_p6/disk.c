#include "disk.h"

/******************************************************************************/
static int active = 0;  /* is the virtual disk open (active) */
static int handle;      /* file handle to virtual disk       */

/******************************************************************************/
int make_disk(char *name) {
  int f, cnt;
  char buf[BLOCK_SIZE];

  if (!name) {
    fprintf(stderr, "make_disk: invalid file name\n");
    return -1;
  }

  if ((f = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
    perror("make_disk: cannot open file");
    return -1;
  }

  memset(buf, 0, BLOCK_SIZE);
  for (cnt = 0; cnt < DISK_BLOCKS; ++cnt)
    write(f, buf, BLOCK_SIZE);

  close(f);

  return 0;
}

int open_disk(char *name) {
  int f;

  if (!name) {
    fprintf(stderr, "open_disk: invalid file name\n");
    return -1;
  }  
  
  if (active) {
    fprintf(stderr, "open_disk: disk is already open\n");
    return -1;
  }
  
  if ((f = open(name, O_RDWR, 0644)) < 0) {
    perror("open_disk: cannot open file");
    return -1;
  }

  handle = f;
  active = 1;

  return 0;
}

int close_disk() {
  if (!active) {
    fprintf(stderr, "close_disk: no open disk\n");
    return -1;
  }
  
  close(handle);

  active = handle = 0;

  return 0;
}

int block_write(int block, char *buf) {
  if (!active) {
    fprintf(stderr, "block_write: disk not active\n");
    return -1;
  }

  if ((block < 0) || (block >= DISK_BLOCKS)) {
    fprintf(stderr, "block_write: block index out of bounds\n");
    return -1;
  }

  if (lseek(handle, block * BLOCK_SIZE, SEEK_SET) < 0) {
    perror("block_write: failed to lseek");
    return -1;
  }

  if (write(handle, buf, BLOCK_SIZE) < 0) {
    perror("block_write: failed to write");
    return -1;
  }

  return 0;
}

int block_read(int block, char *buf) {
  if (!active) {
    fprintf(stderr, "block_read: disk not active\n");
    return -1;
  }

  if ((block < 0) || (block >= DISK_BLOCKS)) {
    fprintf(stderr, "block_read: block index out of bounds\n");
    return -1;
  }

  if (lseek(handle, block * BLOCK_SIZE, SEEK_SET) < 0) {
    perror("block_read: failed to lseek");
    return -1;
  }

  if (read(handle, buf, BLOCK_SIZE) < 0) {
    perror("block_read: failed to read");
    return -1;
  }

  return 0;
}

/*File system management functions*/

/*Make the file system*/
int make_fs(char* disk_name) {
  int disk = make_disk(disk_name);

  /*If there was an error making the disk, cannot make the file system.*/
  if(disk == -1) {
    return -1;
  }

  disk = open_disk(disk_name);
  if(disk == -1) {
    return -1;
  }

  /*How are we going to implement our file system?*/

  /*Write/initialize meta-info for fs to be mounted*/

  /*Made the disk, better close it now*/
  disk = close_disk();
  if(disk == -1) {
    return -1;
  }
  
  return 0;
}

int mount_fs(char* disk_name) {
  
  /*Open the disk and make it active*/
  int disk = open_disk(disk_name);
  if(disk == -1) {
    return -1;
  }

  return 0;
}

int umount_fs(char* disk_name) {
  /*write last bit of stuff to it*/

  int disk = close_disk();
  if(disk == -1) {
    return -1;
  }

  return 0;
}

/* File functions, file system should be mounted first */
int fs_open(char *name) {
  if(active == 0) {
    return -1;
  }

  file_meta tmp = file_meta_thingy;
  
  while(tmp != NULL) {
    if(strcmp(tmp.name,name) == 0) {
      /*TODO: open the file*/
      break;
    }
    tmp = tmp->next;
  }
  
  return 0;
}

int fs_close(int filedes) {
  if(active == 0) {
    return -1;
  }
  
  return 0;
}

int fs_create(char *name) {
  if(active == 0) {
    return -1;
  }

  return 0;
}

int fs_delete(char *name) {
  if(active == 0) {
    return -1;
  }

  return 0;
}

int fs_read(int fildes, void *buf, size_t nbyte) {
  if(active == 0) {
    return -1;
  }

  return 0;
}

int fs_write(int fildes, void *buf, size_t nbyte) {
  if(active == 0) {
    return -1;
  }

  return 0;
}

int fs_get_filesize(int fildes) {
  if(active == 0) {
    return -1;
  }

  return 0;
}

int fs_lseek(int fildes, off_t offset) {
  if(active == 0) {
    return -1;
  }

  return 0;
}

int fs_truncate(int fildes, off_t length) {
  if(active == 0) {
    return -1;
  }

  return 0;
}
