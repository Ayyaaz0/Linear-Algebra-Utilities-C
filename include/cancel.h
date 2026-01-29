#ifndef CANCEL_H
#define CANCEL_H

#include <setjmp.h>

extern jmp_buf cancel_jump_point;

void trigger_cancel(void);  //to call longjmp

#endif
