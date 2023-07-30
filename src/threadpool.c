#include "threadpool.h"

static void *thread_func(void *arg);

threadpool_t *threadpool_init(int thread_num)
{
  threadpool_t *pool = (threadpool_t *)malloc(sizeof(threadpool_t));
  pool->stop = false;
  pool->task_queue = task_queue_create(0);
  pthread_mutex_init(&pool->mutex, NULL);
  pool->thread_num = thread_num;
  pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_num);
  for (int i = 0; i < thread_num; i++)
  {
    pthread_create(&(pool->threads[i]), NULL, thread_func, (void *)pool);
    pthread_detach(pool->threads[i]);
  }
  return pool;
}

void threadpool_append(threadpool_t *pool, task_t *task)
{
  pthread_mutex_lock(&pool->mutex);
  task_queue_push(pool->task_queue, task);
  pthread_mutex_unlock(&pool->mutex);
}

void run(threadpool_t *pool)
{
  while (!pool->stop)
  {
    pthread_mutex_lock(&pool->mutex);
    task_t *task = task_queue_pop(pool->task_queue);
    if (task == NULL)
    {
      pthread_mutex_unlock(&pool->mutex);
      continue;
    }
    pthread_mutex_unlock(&pool->mutex);
    task->task_func(task->task_arg);
  }
}

void *thread_func(void *arg)
{
  printf("thread working()\n");
  threadpool_t *pool = (threadpool_t *)arg;
  run(pool);
} 