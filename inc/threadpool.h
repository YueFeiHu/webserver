#ifndef __THREADPOLL_H__
#define __THREADPOLL_H__

#include <pthread.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "task_queue.h"

typedef struct threadpool
{
  int thread_num;
  task_queue_t *task_queue;
  pthread_t *threads;
  pthread_mutex_t mutex;
  bool stop;
}threadpool_t;

threadpool_t *threadpool_init(int thread_num);
void threadpool_append(threadpool_t *pool, task_t *task);

#endif