#include <timer.h>
#if CONFIG_ENABLE_TIMERS > 0

TimerId TimerCreate(TimerCallback callback, uint32_t expiry_time, uint32_t period_time, void *user_data) {
    ASSERT_KERNEL(callback, NULL);
    ASSERT_KERNEL(expiry_time, NULL);

    CoreInit();
    CoreSchedulingSuspend();
    Timer *timer = AllocateTimerObject();

    if(timer == NULL) {
        CoreSchedulingResume();
        return NULL;
    }
    timer->callback = callback;
    timer->expired=false;
    timer->running=false;
    timer->expiry_time = expiry_time;
    timer->user_data = user_data;

    if(period_time) {
        timer->period_time = period_time;
        timer->periodic = true;
    } else {
        timer->period_time = 0;
        timer->periodic = false;
    }

    CoreSchedulingResume();
    return ((TimerId) timer);
}

KernelResult TimerStart(TimerId timer) {
    ASSERT_PARAM(timer);
    Timer * t = (Timer *)timer;

    CoreSchedulingSuspend();
    KernelResult result = RemoveTimeout(&t->timeout);
    
    if(result == kSuccess) {
        result = AddTimeout(&t->timeout, t->expiry_time, t->callback, t->user_data, false, NULL);    
        if(result == kSuccess) {
            t->expired = false;
            t->running = true;
        }
    }

    CoreSchedulingResume();
    return result;
}

KernelResult TimerStop(TimerId timer) {
    ASSERT_PARAM(timer);
    Timer * t = (Timer *)timer;

    CoreSchedulingSuspend();
    if(!t->running) {
        CoreSchedulingResume();
        return kErrorTimerIsNotRunning;
    }

    KernelResult result = RemoveTimeout(&t->timeout);
    if(result == kSuccess) {
        t->running = false;
        t->expired = false;
    }

    CoreSchedulingResume();
    return result;
}

KernelResult TimerSetValues(TimerId timer, uint32_t expiry_time, uint32_t period_time) {
    ASSERT_PARAM(timer);
    ASSERT_PARAM(expiry_time);

    Timer *t = (Timer *)timer;

    CoreSchedulingSuspend();

    if(t->running) {
        RemoveTimeout(&t->timeout);
    }

    timer->expired=false;
    timer->running=false;
    timer->expiry_time = expiry_time;
    
    if(period_time) {
        timer->period_time = period_time;
        timer->periodic = true;
    } else {
        timer->period_time = 0;
        timer->periodic = false;
    }

    CoreSchedulingResume();
    return kSuccess;
}

KernelResult TimerDelete(TimerId timer) {
    ASSERT_PARAM(timer);
    Timer * t = (Timer *)timer;

    CoreSchedulingSuspend();

    if(t->running) {
        RemoveTimeout(&t->timeout);
    }
    t->callback = NULL; 
    FreeTimerObject(t);
    CoreSchedulingResume();

    return kSuccess;
}

#endif