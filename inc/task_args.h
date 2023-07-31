#ifndef __TASK_ARGS_H__
#define __TASK_ARGS_H__
typedef struct task_args{
  int epollfd;
  void *arg;
}task_args_t;

#endif