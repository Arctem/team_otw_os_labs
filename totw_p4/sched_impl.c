#include "scheduler.h"
#include "sched_impl.h"

/* Utility functions */

void insert_item_head(list_t *lst, void *datum) {
  list_elem_t *elt = malloc(sizeof(list_elem_t));
  list_elem_init(elt, datum);
  list_insert_head(lst, elt);
}

void insert_item_tail(list_t *lst, void *datum) {
  list_elem_t *elt = malloc(sizeof(list_elem_t));
  list_elem_init(elt, datum);
  list_insert_tail(lst, elt);
}

void *peek_item_head(list_t *lst) {
  list_elem_t *elt = list_get_head(lst);
  return elt->datum;
}

void *peek_item_tail(list_t *lst) {
  list_elem_t *elt = list_get_tail(lst);
  return elt->datum;
}

/* Start of thread functions */

static void init_thread_info(thread_info_t *info, sched_queue_t *queue) {
  info->sq = queue;
  info->sem = malloc(sizeof(sem_t));
  sem_init(info->sem, 0, -1); /* initialize semaphore as busy */
}

static void destroy_thread_info(thread_info_t *info) {
  sem_destroy(info->sem);
  free(info->sem);
}

static void enter_sched_queue(thread_info_t *info) {
  insert_item_tail(info->sq->queue, info);
  sem_post(info->sq->queue_sem);
}

static void leave_sched_queue(thread_info_t *info) {
}

static void wait_for_cpu(thread_info_t *info) {
  sem_wait(info->sem);
}

static void release_cpu(thread_info_t *info) {
}

/* Start of scheduler functions */

static void init_sched_queue(sched_queue_t *queue, int queue_size) {
  queue->queue = malloc(sizeof(list_t));
  list_init(queue->queue);
  queue->max_running = queue_size;
  queue->running = malloc(sizeof(list_t));
  list_init(queue->running);
  
  queue->sem = malloc(sizeof(sem_t));
  sem_init(queue->sem, 0, queue_size - 1); /* initialize semaphore based on size */
  queue->queue_sem = malloc(sizeof(sem_t));
  sem_init(queue->queue_sem, 0, -1); /* initialize semaphore to wait on threads */
}

static void destroy_sched_queue(sched_queue_t *queue) {
  sem_destroy(queue->queue_sem);
  free(queue->queue_sem);
  sem_destroy(queue->sem);
  free(queue->sem);
  free(queue->running);
  free(queue->queue);
}

static void wake_up_worker(thread_info_t *info) {
}

static void wait_for_worker(sched_queue_t *queue) {
}

static thread_info_t *next_worker(sched_queue_t *queue) {
  if(list_size(queue->queue) == 0) {
    return NULL;
  } else {
    return peek_item_head(queue->queue);
  }
}

static void wait_for_queue(sched_queue_t *queue) {
  if(list_size(queue->queue) == 0) {
    /* reinitialize semaphore to wait on threads */
    sem_init(queue->queue_sem, 0, -1);
    sem_wait(queue->queue_sem);
  }
}

/* You need to statically initialize these structures: */
sched_impl_t sched_fifo = {
  { init_thread_info, destroy_thread_info, enter_sched_queue,
    leave_sched_queue, wait_for_cpu, release_cpu },
  { init_sched_queue, destroy_sched_queue, wake_up_worker,
    wait_for_worker, next_worker, wait_for_queue }
};
sched_impl_t sched_rr = {
  { init_thread_info, destroy_thread_info /*, ...etc... */ },
  { init_sched_queue, destroy_sched_queue /*, ...etc... */ }
};
