#include "scheduler.h"
#include "sched_impl.h"

/* Utility functions */

void insert_item_head(list_wrap_t *lst, void *datum) {
  sem_wait(lst->sem);
  list_elem_t *elt = malloc(sizeof(list_elem_t));
  list_elem_init(elt, datum);
  list_insert_head(lst->data, elt);
  sem_post(lst->sem);
}

void insert_item_tail(list_wrap_t *lst, void *datum) {
  sem_wait(lst->sem);
  list_elem_t *elt = malloc(sizeof(list_elem_t));
  list_elem_init(elt, datum);
  list_insert_tail(lst->data, elt);
  sem_post(lst->sem);
}

list_elem_t *peek_item_head(list_wrap_t *lst) {
  sem_wait(lst->sem);
  list_elem_t *elt = list_get_head(lst->data);
  sem_post(lst->sem);
  return elt;
}

list_elem_t *peek_item_tail(list_wrap_t *lst) {
  sem_wait(lst->sem);
  list_elem_t *elt = list_get_tail(lst->data);
  sem_post(lst->sem);
  return elt;
}

int elt_in_list(list_wrap_t *lst, void *datum) {
  sem_wait(lst->sem);
  list_elem_t *elt = list_get_head(lst->data);
  while(elt != NULL) {
    if(elt->datum == datum) {
      sem_post(lst->sem);
      return 1;
    } else
      elt = elt->next;
  }
  sem_post(lst->sem);
  return 0;
}

list_elem_t *find_in_list(list_wrap_t *lst, void *datum) {
  sem_wait(lst->sem);
  list_elem_t *elt = list_get_head(lst->data);
  while(elt != NULL) {
    if(elt->datum == datum) {
      sem_post(lst->sem);
      return elt;
    } else
      elt = elt->next;
  }
  sem_post(lst->sem);
  return NULL;
}

void remove_item(list_wrap_t *lst, void *datum) {
  list_elem_t *elt = find_in_list(lst, datum);
  sem_wait(lst->sem);
  if(elt) {
    list_remove_elem(lst->data, elt);
    free(elt);
  }
  sem_post(lst->sem);
}

int wrapper_size(list_wrap_t *lst) {
  sem_wait(lst->sem);
  int size = list_size(lst->data);
  sem_post(lst->sem);
  return size;
}

list_wrap_t *create_wrapper() {
  list_wrap_t *wrapper = calloc(1, sizeof(list_wrap_t));
  wrapper->data = calloc(1, sizeof(list_t));
  wrapper->sem = calloc(1, sizeof(sem_t));
  sem_init(wrapper->sem, 0, 1);
  return wrapper;
}

void destroy_wrapper(list_wrap_t *wrapper) {
  sem_wait(wrapper->sem);
  sem_destroy(wrapper->sem);
  free(wrapper->sem);
  free(wrapper->data);
  free(wrapper);
}

/* Start of thread functions */

static void init_thread_info(thread_info_t *info, sched_queue_t *queue) {
  info->sq = queue;
  info->sem = calloc(1, sizeof(sem_t));
  sem_init(info->sem, 0, -1); /* initialize semaphore as busy */
}

static void destroy_thread_info(thread_info_t *info) {
  sem_destroy(info->sem);
  free(info->sem);
}

static void enter_sched_queue(thread_info_t *info) {
  insert_item_tail(info->sq->queue, info);
}

static void leave_sched_queue(thread_info_t *info) {
  remove_item(info->sq->queue, info);
}

static void wait_for_cpu(thread_info_t *info) {
  sem_wait(info->sem);
}

static void fifo_release_cpu(thread_info_t *info) {
  remove_item(info->sq->running, info);
  sem_post(info->sq->sem);
}

static void rr_release_cpu(thread_info_t *info) {
  remove_item(info->sq->running, info);
  remove_item(info->sq->queue, info);
  insert_item_tail(info->sq->queue, info);
  sem_post(info->sq->sem);
}

/* Start of scheduler functions */

static void init_sched_queue(sched_queue_t *queue, int queue_size) {
  queue->queue = create_wrapper();
  queue->max_running = queue_size;
  queue->running = create_wrapper();
  
  queue->sem = calloc(1, sizeof(sem_t));
  sem_init(queue->sem, 0, queue_size - 1); /* initialize semaphore based on size */
  printf("Queue size: %d\n", queue_size);
}

static void destroy_sched_queue(sched_queue_t *queue) {
  sem_destroy(queue->sem);
  free(queue->sem);
  destroy_wrapper(queue->running);
  destroy_wrapper(queue->queue);
}

static void wake_up_worker(thread_info_t *info) {
  insert_item_tail(info->sq->running, info);
  sem_post(info->sem);
}

static void wait_for_worker(sched_queue_t *queue) {
  /* int val = 0; */
  /* sem_getvalue(queue->sem, &val); */
  /* printf("Sem value: %d\n", val); */
  sem_wait(queue->sem);
}

static thread_info_t *next_worker(sched_queue_t *queue) {
  list_elem_t *elt = peek_item_head(queue->queue);
  while(elt != NULL) {
    if(!elt_in_list(queue->running, elt->datum))
      return elt->datum;
    else
      elt = elt->next;
  }
  return NULL;
}

static void wait_for_queue(sched_queue_t *queue) {
  static int done_thing = 0;
  if(done_thing) {
    /* Do nothing here because if we did we'd miss when */
    /* num_workers_remaining in scheduler.c hits 0. */
    /* Unfortunate side effect is busy waiting */
  } else {
    while(wrapper_size(queue->queue) == 0)
      ;
    done_thing = 1;
  }
}

/* You need to statically initialize these structures: */
sched_impl_t sched_fifo = {
  { init_thread_info, destroy_thread_info, enter_sched_queue,
    leave_sched_queue, wait_for_cpu, fifo_release_cpu },
  { init_sched_queue, destroy_sched_queue, wake_up_worker,
    wait_for_worker, next_worker, wait_for_queue }
};
sched_impl_t sched_rr = {
  { init_thread_info, destroy_thread_info, enter_sched_queue,
    leave_sched_queue, wait_for_cpu, rr_release_cpu },
  { init_sched_queue, destroy_sched_queue, wake_up_worker,
    wait_for_worker, next_worker, wait_for_queue }
};
