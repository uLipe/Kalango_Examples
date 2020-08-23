#include <kalango_api.h>
#include "test_macros.h"

KernelResult KalangoTasksTests(void) {

    int fails = 0;

    //Current task priority is the same when created if not changed:
    TaskId current = Kalango_GetCurrentTaskId();
    fails |= TEST_ASSERT_NOT_NULL(current);
    fails |= TEST_ASSERT_EQUALS(Kalango_TaskGetPriority(current), 16);

    //Setting priority results in task base priority:
    uint32_t old = Kalango_TaskSetPriority(current, 17);
    fails |= TEST_ASSERT_NOT_EQUALS(old, 17);
    fails |= TEST_ASSERT_EQUALS(Kalango_TaskGetPriority(current), 17);

    //Reset priority gives the task original priority:
    Kalango_TaskSetPriority(current, old);
    fails |= TEST_ASSERT_EQUALS(Kalango_TaskGetPriority(current), old);

    return (KernelResult)fails;
}

KernelResult KalangoQueueTests(void) {
    return kSuccess;
}

KernelResult KalangoMutexTests(void) {
    return kSuccess;
}

KernelResult KalangoTimerTests(void) {
    return kSuccess;
}

KernelResult KalangoSemaphoreTests(void) {
    return kSuccess;
}

KernelResult KalangoArchTests(void) {
    return kSuccess;
}

KernelResult KalangoCoreTests(void) {

    int fails = 0;

    //Test sleep in ticks matches with current tick count:
    uint32_t ticks_previous = Kalango_GetCurrentTicks();
    fails |= TEST_ASSERT_EQUALS(Kalango_Sleep(123), kSuccess);
    fails |= TEST_ASSERT_EQUALS(Kalango_GetCurrentTicks() - ticks_previous,
                        123);

    return (KernelResult)fails;
}

KernelResult KalangoSystemTest(void) {
    KernelResult r = KalangoCoreTests();
    r |= KalangoTasksTests();
    return r;
}