#include "fs.h"

extern int active;
char *usage = NULL;    /* tracks which blocks are in use    */
file_meta *file_metas = NULL;
descriptor *descriptors = NULL;;

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
  usage = malloc(DISK_BLOCKS / 2 * sizeof(char));
  block_read(0, data);
  for(i = 0; i < DISK_BLOCKS / 2; i++) {
    usage[i] = data[i];
  }

  /* load each of the file meta data things */
  file_metas = malloc(NUM_FILES * sizeof(file_meta));
  for(i = 0; i < NUM_FILES; i++) {
    block_read(3*i + 1, data);
    file_metas[i].in_use = data[0];
    
    if(file_metas[i].in_use) {
      /* load other values if file exists */
      /* each value is stored in 2 bytes, so we need to parse that */
      memcpy(&file_metas[i].name, data + 1, NAMELEN);
      file_metas[i].num_blocks = 256 * data[NAMELEN + 2] + data[NAMELEN + 3];
      file_metas[i].size_last = 256 * data[NAMELEN + 4] + data[NAMELEN + 5];
      
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

  /* initialize file descriptors */
  descriptors = malloc(OPEN_FILES * sizeof(descriptor));
  for(i = 0; i < OPEN_FILES; i++) {
    descriptors[i].file = -1;
    descriptors[i].cursor = 0;
  }

  free(data);
  return 0;
}

int umount_fs(char* disk_name) {
  int i = 0;
  for(i = 0; i < OPEN_FILES; i++) {
    if(descriptors[i].file != -1) {
      return -1;
    }
  }
  
  /*write last bit of stuff to it*/

  /* free the various structures */
  free(usage);
  free(file_metas);
  free(descriptors);

  if(close_disk() == -1) {
    return -1;
  }

  return 0;
}

/* File functions, file system should be mounted first */
int fs_open(char *name) {
  int i = 0;
  if(active == 0) {
    return -1;
  }

  int file_num = -1;
  for(i = 0; i < NUM_FILES; i++) {
    if(file_metas[i].in_use && strncmp(file_metas[i].name, name, NAMELEN) == 0) {
      file_num = i;
      break;
    }
  }

  if(file_num != -1) {
    for(i = 0; i < OPEN_FILES; i++) {
      if(descriptors[i].file == -1) {
	descriptors[i].file = file_num;
	descriptors[i].cursor = 0;
	return i;
      }
    }
  }
  
  return -1;
}

int fs_close(int filedes) {
  if(active == 0 || descriptors[filedes].file == -1) {
    return -1;
  }
  descriptors[filedes].file = -1;
  
  return 0;
}

int fs_create(char *name) {
  int i = 0;
  
  if(active == 0) {
    return -1;
  }

  for(i = 0; i < NUM_FILES; i++) {
    if(!file_metas[i].in_use) {
      file_metas[i].in_use = 1;
      memcpy(file_metas[i].name, name, NAMELEN);
      file_metas[i].num_blocks = 0;
      file_metas[i].size_last = 0;
      save_file_meta(i);
      return 0;
    }
  }

  return -1;
}

int fs_delete(char *name) {
  int i = 0, k = 0;
  
  if(active == 0) {
    return -1;
  }

  for(i = 0; i < NUM_FILES; i++) {
    if(file_metas[i].in_use) {
      file_metas[i].in_use = 0;
      for(k = 0; k < file_metas[i].num_blocks; k++) {
	usage[file_metas[i].blocks[k] - DISK_BLOCKS / 2] = 0;
      }
      save_file_meta(i);
      return 0;
    }
  }

  return -1;
}

int fs_read(int fildes, void *buf, size_t nbyte) {
  int read = 0;
  int dist;
  int offset;
  int block;
  char *data = NULL;
  descriptor *desc = NULL;
  if(active == 0) {
    return -1;
  }

  data = malloc(BLOCK_SIZE * sizeof(char));
  desc = &descriptors[fildes];

  while(desc->cursor < fs_get_filesize(fildes) && nbyte > 0) {
    offset = desc->cursor % BLOCK_SIZE;
    dist = BLOCK_SIZE - offset;
    if(fs_get_filesize(fildes) - desc->cursor < dist) {
      dist = fs_get_filesize(fildes) - desc->cursor;
      if(dist == 0) {
	break;
      }
    }
    if(nbyte < dist) {
      dist = nbyte;
    }

    block = desc->cursor / BLOCK_SIZE;

    block_read(file_metas[(int) desc->file].blocks[block], data);
    memcpy(buf, data + offset, dist);
    
    read += dist;
    nbyte -= dist;
    buf += dist;
    desc->cursor += dist;
  }

  free(data);
  return read;
}

int fs_write(int fildes, void *buf, size_t nbyte) {
  int written = 0;
  int dist;
  int offset;
  int block;
  int newblock;
  char *data = NULL;
  descriptor *desc = NULL;
  if(active == 0) {
    return -1;
  }

  data = malloc(BLOCK_SIZE * sizeof(char));
  desc = &descriptors[fildes];

  /* write inside existing file */
  while(desc->cursor < fs_get_filesize(fildes) && nbyte > 0) {
    offset = desc->cursor % BLOCK_SIZE;
    dist = BLOCK_SIZE - offset;
    if(nbyte < dist) {
      dist = nbyte;
    }
    block = desc->cursor / BLOCK_SIZE;

    block_read(file_metas[(int) desc->file].blocks[block], data);
    memcpy(data + offset, buf, dist);
    block_write(file_metas[(int) desc->file].blocks[block], data);
    
    written += dist;
    nbyte -= dist;
    buf += dist;
    desc->cursor += dist;
    if(desc->cursor > fs_get_filesize(fildes)) {
      file_metas[(int) desc->file].size_last += dist;
    }
  }

  /* write past former end */
  while(nbyte > 0) {
    if(file_metas[(int) desc->file].size_last == BLOCK_SIZE ||
       file_metas[(int) desc->file].num_blocks == 0) {
      /* expand into more blocks if needed */
      newblock = find_empty_block();
      if(newblock == -1) {
	break;
      }
      int num_blocks = file_metas[(int) desc->file].num_blocks;
      file_metas[(int) desc->file].blocks[num_blocks] = newblock + DISK_BLOCKS / 2;
      file_metas[(int) desc->file].num_blocks++;
      file_metas[(int) desc->file].size_last = 0;
      usage[newblock] = 1;
    }

    offset = desc->cursor % BLOCK_SIZE;
    dist = BLOCK_SIZE - offset;
    if(nbyte < dist) {
      dist = nbyte;
    }
    block = desc->cursor / BLOCK_SIZE;

    block_read(file_metas[(int) desc->file].blocks[block], data);
    memcpy(data + offset, buf, dist);
    block_write(file_metas[(int) desc->file].blocks[block], data);

    written += dist;
    nbyte -= dist;
    buf += dist;
    desc->cursor += dist;

    if(desc->cursor > fs_get_filesize(fildes)) {
      file_metas[(int) desc->file].size_last += dist;
    }
  }
  save_file_meta(desc->file);

  free(data);
  return written;
}

int fs_get_filesize(int fildes) {
  if(active == 0 || descriptors[fildes].file == -1) {
    return -1;
  }
  file_meta f = file_metas[(int) descriptors[fildes].file];
  if(f.num_blocks == 0)
    return 0;

  return (f.num_blocks - 1) * BLOCK_SIZE + f.size_last;
}

int fs_lseek(int fildes, off_t offset) {
  if(active == 0 || descriptors[fildes].file == -1 ||
     offset < 0 || offset > fs_get_filesize(fildes)) {
    return -1;
  }
  descriptors[fildes].cursor = offset;

  return 0;
}

int fs_truncate(int fildes, off_t length) {
  if(active == 0 || descriptors[fildes].file == -1 ||
     length > fs_get_filesize(fildes)) {
    return -1;
  }

  file_meta *f = &file_metas[(int) descriptors[fildes].file];
  int target_blocks = length / BLOCK_SIZE;
  if(length % BLOCK_SIZE > 0) {
    target_blocks++;
  }
  while(target_blocks < f->num_blocks) {
    usage[f->blocks[f->num_blocks] - DISK_BLOCKS / 2] = 0;
    f->num_blocks--;
  }
  f->size_last = length % BLOCK_SIZE;
  save_file_meta(descriptors[fildes].file);
  
  return 0;
}
