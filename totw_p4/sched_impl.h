#ifndef	__SCHED_IMPL__H__
#define	__SCHED_IMPL__H__

#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>

struct thread_info {
  struct sched_queue *sq;
  sem_t *sem;
};

typedef struct list_wrapper {
  list_t *data;
  sem_t *sem;
} list_wrap_t;

struct sched_queue {
  list_wrap_t *queue;
  int max_running;
  list_wrap_t *running;
  sem_t *sem;
  sem_t *queue_sem;
};

#endif /* __SCHED_IMPL__H__ */
