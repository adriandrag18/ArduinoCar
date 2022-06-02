#include "actionStack.h"

action actions[MAX_ACTIONS];
int begin_action = 0;
int end_action = 0;

bool fullActionsStack() {
    return begin_action == end_action;
}

bool emtpyActionsStack() {
    return (begin_action + 1) % MAX_ACTIONS == end_action;
}

void addAction(char action_command, int action_speed, int action_duration) {
    if (!emtpyActionsStack()) {
        if (actions[(MAX_ACTIONS + end_action - 1) % MAX_ACTIONS].command == action_command &&
            actions[(MAX_ACTIONS + end_action - 1) % MAX_ACTIONS]._speed == action_speed) {
                actions[(MAX_ACTIONS + end_action - 1) % MAX_ACTIONS].duration += action_duration;
            }
    }
    action tmp;
    tmp.command = action_command;
    tmp._speed = action_speed;
    tmp.duration = action_duration;
    actions[end_action] = tmp;
    if (fullActionsStack())
        begin_action = (begin_action + 1) % MAX_ACTIONS;
    end_action = (end_action + 1) % MAX_ACTIONS;
}

action popAction() {
    if (emtpyActionsStack())
        return {-1, -1, -1};
    end_action = (MAX_ACTIONS + end_action - 1) % MAX_ACTIONS;
    return actions[end_action];
}

action getAction() {
    if (emtpyActionsStack())
        return {-1, -1, -1};
    return actions[(MAX_ACTIONS + end_action - 1) % MAX_ACTIONS];
}

