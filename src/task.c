#include "task.h"

task_t *task_create(func_t func, void *args)
{
  task_t *task = (task_t *)malloc(sizeof(task_t));
  task->task_func = func;
  task->task_arg = args;
  return task;
}