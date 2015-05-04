#include "fsutil.h"

extern int active;
extern file_meta *file_metas;

int save_file_meta(int filenum) {
  char *data = NULL;
  int i = 0;
  if(active == 0) {
    return -1;
  }
    
  data = malloc(BLOCK_SIZE * sizeof(char));
  data[0] = file_metas[filenum].in_use;
  memcpy(data + 1, &file_metas[filenum].name, NAMELEN);
  data[NAMELEN + 2] = file_metas[filenum].num_blocks / 256;
  data[NAMELEN + 3] = file_metas[filenum].num_blocks % 256;
  data[NAMELEN + 4] = file_metas[filenum].size_last / 256;
  data[NAMELEN + 5] = file_metas[filenum].size_last % 256;
  block_write(3*filenum + 1, data);

  for(i = 0; i < DISK_BLOCKS / 4; i++) {
    data[2*i] = file_metas[filenum].blocks[i] / 256;
    data[2*i + 1] = file_metas[filenum].blocks[i] % 256;
  }
  block_write(3*filenum + 2, data);

  for(i = 0; i < DISK_BLOCKS / 4; i++) {
    data[2*i] = file_metas[filenum].blocks[i + DISK_BLOCKS / 4] / 256;
    data[2*i + 1] = file_metas[filenum].blocks[i + DISK_BLOCKS / 4] % 256;
  }
  block_write(3*filenum + 3, data);
  
  free(data);
  return 0;
}
