
#ifndef _SAME54_PY_TIMER_

#define _SAME54_PY_TIMER_

#include "definitions.h"

typedef void (*TMR_Start) (void);
typedef void (*TMR_Stop) (void);
typedef void (*TMR_PeriodSet)(uint32_t period);
typedef void (*TMR_CallbackRegister)( TC_TIMER_CALLBACK callback_fn, uintptr_t context );


#define Timer_Index0_Start TC0_TimerStart
#define Timer_Index0_Stop TC0_TimerStop
#define Timer_Index0_32bitPeriodSet TC0_Timer32bitPeriodSet
#define Timer_Index0_CallbackRegister TC0_TimerCallbackRegister

#endif