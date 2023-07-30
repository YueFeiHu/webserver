#include "task_queue.h"

task_queue_t *task_queue_create(int queue_size)
{
  task_queue_t *queue = (task_queue_t *)malloc(sizeof(task_queue_t));
  queue->head = NULL;
  queue->tail = NULL;
  queue->task_num = 0;
  if (queue_size <= 0)
  {
    queue->max_queue_size = 1024;
  }
  else
  {
    queue->max_queue_size = queue_size;
  }
}

void task_queue_push(task_queue_t *queue, task_t *task)
{
  if (queue->task_num == 0)
  {
    queue->head = queue->tail = task;
  }
  else
  {
    queue->tail->next = task;
    queue->tail = task;
  }
  queue->task_num++;
}
task_t *task_queue_pop(task_queue_t *queue)
{
  if (queue->task_num == 0)
  {
    return NULL;
  }
  task_t *task = queue->head;
  queue->head = queue->head->next;
  task->next = NULL;
  queue->task_num--;
  return task;
}