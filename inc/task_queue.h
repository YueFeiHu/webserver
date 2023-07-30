#ifndef __TASK_QUEUE_H__
#define __TASK_QUEUE_H__

#include "task.h"

typedef struct task_queue
{
  task_t *head;
  task_t *tail; 
  int task_num;
  int max_queue_size; 
}task_queue_t;

task_queue_t *task_queue_create(int queue_size);
void task_queue_push(task_queue_t *queue, task_t *task);
task_t *task_queue_pop(task_queue_t *queue);

#endif