#ifndef	__SCHED_IMPL__H__
#define	__SCHED_IMPL__H__

#include "list.h"
#include <stdlib.h>

struct thread_info {
  /*...Fill this in...*/
  struct sched_queue *sq; /*Schedule queue pointer*/
};

struct sched_queue {
  /*...Fill this in...*/
  list_t *queue;
};

#endif /* __SCHED_IMPL__H__ */
