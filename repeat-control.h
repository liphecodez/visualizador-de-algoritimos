#ifndef REPEAT_CONTROL_H
#define REPEAT_CONTROL_H

typedef struct {
    double started, next;
    int active;
} RepeatControl;

/* Return the number of changes due, including the initial press. */
static inline int RepeatChanges(RepeatControl *repeat, double now, int pressed, int held) {
    if (!held) {
        repeat->active = 0;
        return 0;
    }
    if (pressed) {
        *repeat = (RepeatControl){.started = now, .next = now + 0.350, .active = 1};
        return 1;
    }
    if (!repeat->active)
        return 0;

    int changes = 0;
    while (now >= repeat->next && changes < 100) {
        double accelerating = 0.100 - (repeat->next - repeat->started - 0.350) * 0.035;
        repeat->next += accelerating > 0.025 ? accelerating : 0.025;
        changes++;
    }
    if (now >= repeat->next)
        repeat->next = now + 0.025;
    return changes;
}

#endif
