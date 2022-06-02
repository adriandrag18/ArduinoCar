#ifndef ACTION_STACK_H
#define ACTION_STACK_H

#define MAX_ACTIONS 200

typedef struct {
  char command;
  int _speed;
  int duration;
} action;

extern void addAction(char action_command, int action_speed, int action_duration);
extern bool fullActionsStack();
extern bool emtpyActionsStack();
extern action popAction();
extern action getAction();

#endif