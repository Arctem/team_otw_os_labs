#include "disk.h"

/******************************************************************************/
static int active = 0;  /* is the virtual disk open (active) */
static int handle;      /* file handle to virtual disk       */
static short *usage = NULL;    /* tracks which blocks are in use    */
static file_meta *file_metas = NULL;

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
  int i = 0;
  char empty[BLOCK_SIZE] = {0};

  /*If there was an error making the disk, cannot make the file system.*/
  if(make_disk(disk_name) == -1) {
    return -1;
  }

  if(open_disk(disk_name) == -1) {
    return -1;
  }

  /* Zero out everything. */
  for(i = 0; i < DISK_BLOCKS; i++) {
    block_write(i, empty);
  }

  if(close_disk() == -1) {
    return -1;
  }
  
  return 0;
}

int mount_fs(char* disk_name) {
  int i = 0, k = 0;
  /*Open the disk and make it active*/
  int disk = open_disk(disk_name);
  char *data = NULL;
  if(disk == -1) {
    return -1;
  }

  data = malloc(BLOCK_SIZE * sizeof(char));

  /* load data about which blocks are in use */
  usage = malloc(DISK_BLOCKS / 2 * sizeof(short));
  block_read(0, data);
  for(i = 0; i < DISK_BLOCKS / 2; i++) {
    /* each value is stored in 2 bytes, so we need to parse that */
    usage[i] = 256 * data[2*i] + data[2*i + 1];
  }

  /* load each of the file meta data things */
  file_metas = malloc(NUM_FILES * sizeof(file_meta));
  for(i = 0; i < NUM_FILES; i++) {
    block_read(3*i + 1, data);
    file_metas[i].in_use = data[0];
    
    if(file_metas[i].in_use) {
      /* load other values if file exists */
      memcpy(&file_metas[i].name, data + 1, 25);
      file_metas[i].num_blocks = 256 * data[26] + data[27];
      file_metas[i].size_last = 256 * data[28] + data[29];
      
      block_read(3*i + 2, data);
      for(k = 0; k < DISK_BLOCKS / 4; k++) {
	file_metas[i].blocks[k] = 256 * data[2*k] + data[2*k + 1];
      }
      block_read(3*i + 3, data);
      for(k = 0; k < DISK_BLOCKS / 4; k++) {
	file_metas[i].blocks[k + DISK_BLOCKS / 4] = 256 * data[2*k] + data[2*k + 1];
      }
    } else {
      /* if file is unused, just zero everything else */
      memset(&file_metas[i].name, 0, 25);
      file_metas[i].num_blocks = 0;
      file_metas[i].size_last = 0;
      memset(&file_metas[i].blocks, 0, DISK_BLOCKS / 2);
    }
  }

  free(data);
  return 0;
}

int umount_fs(char* disk_name) {
  /*write last bit of stuff to it*/

  if(close_disk() == -1) {
    return -1;
  }

  return 0;
}

/* File functions, file system should be mounted first */
int fs_open(char *name) {
  if(active == 0) {
    return -1;
  }

  /* file_meta tmp = file_meta_thingy; */
  
  /* while(tmp != NULL) { */
  /*   if(strcmp(tmp.name,name) == 0) { */
  /*     /\*TODO: open the file*\/ */
  /*     break; */
  /*   } */
  /*   tmp = tmp->next; */
  /* } */
  
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
