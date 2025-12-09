#include "cancel.h"

jmp_buf cancel_jump_point;

void trigger_cancel(void) {
    longjmp(cancel_jump_point, 1);
}
