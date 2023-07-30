#ifndef __TASK_H__
#define __TASK_H__

#include <stdlib.h>
typedef void *(*func_t)(void* args);

typedef struct task
{
  func_t task_func;
  void *task_arg;
  struct task *next;
}task_t;

task_t *task_create(func_t func, void *args);

#endif