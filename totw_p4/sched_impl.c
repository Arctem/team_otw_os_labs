#include "scheduler.h"
#include "sched_impl.h"

/* Fill in your scheduler implementation code below: */

static void init_thread_info(thread_info_t *info, sched_queue_t *queue) {
  info->sq = queue;
}

static void destroy_thread_info(thread_info_t *info) {
  free(info);
}

static void enter_sched_queue(thread_info_t *info) {
}

static void leave_sched_queue(thread_info_t *info) {
}

static void wait_for_cpu(thread_info_t *info) {
}

static void release_cpu(thread_info_t *info) {
}

static void init_sched_queue(sched_queue_t *queue, int queue_size) {
  /*...Code goes here...*/
  list_init(queue->queue); /*Initialize the list.*/
}

static void destroy_sched_queue(sched_queue_t *queue) {
  free(queue);
}

static void wake_up_worker(thread_info_t *queue) {
}

static void wait_for_worker(sched_queue_t *queue) {
}

static thread_info_t *next_worker(sched_queue_t *queue) {
  return NULL;
}

static void wait_for_queue(sched_queue_t *queue) {
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
