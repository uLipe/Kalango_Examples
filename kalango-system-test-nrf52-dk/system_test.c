#include <kalango_api.h>
#include "test_macros.h"

void KalangoTestTask(void *arg) {
    uint32_t command = (uint32_t) arg;

    if(command == 0) {
        TEST_ASSERT_EQUALS(Kalango_TaskDelete(Kalango_GetCurrentTaskId()), kSuccess);
    }

    if(command == 1) {
        TEST_ASSERT_EQUALS(Kalango_TaskSuspend(Kalango_GetCurrentTaskId()), kSuccess);
        TEST_ASSERT_EQUALS(Kalango_Sleep(10), kSuccess);
        TEST_ASSERT_EQUALS(Kalango_TaskDelete(Kalango_GetCurrentTaskId()), kSuccess);
    }

    if(command == 2) {
        TEST_ASSERT_EQUALS(Kalango_Sleep(5), kSuccess);
        TEST_ASSERT_EQUALS(TaskYield(), kSuccess);
        TEST_ASSERT_EQUALS(Kalango_TaskGetPriority(Kalango_GetCurrentTaskId()), 9);
        TEST_ASSERT_EQUALS(Kalango_TaskDelete(Kalango_GetCurrentTaskId()), kSuccess);

    }

    if(command == 3) {
        TEST_ASSERT_EQUALS(Kalango_Sleep(5), kSuccess);
        TEST_ASSERT_EQUALS(TaskYield(), kSuccess);
        TEST_ASSERT_EQUALS(Kalango_TaskGetPriority(Kalango_GetCurrentTaskId()), 9);
        TEST_ASSERT_EQUALS(Kalango_TaskDelete(Kalango_GetCurrentTaskId()), kSuccess);
    }
}

KernelResult KalangoTasksTests(void) {

    int fails = 0;

    //Current task priority is the same when created if not changed:
    TaskId current = Kalango_GetCurrentTaskId();
    fails |= TEST_ASSERT_NOT_NULL(current);
    fails |= TEST_ASSERT_EQUALS(Kalango_TaskGetPriority(current), 8);

    //Setting priority results in task base priority:
    uint32_t old = Kalango_TaskSetPriority(current, 17);
    fails |= TEST_ASSERT_NOT_EQUALS(old, 17);
    fails |= TEST_ASSERT_EQUALS(Kalango_TaskGetPriority(current), 17);

    //Reset priority gives the task original priority:
    Kalango_TaskSetPriority(current, old);
    fails |= TEST_ASSERT_EQUALS(Kalango_TaskGetPriority(current), old);

    TaskSettings settings;
    settings.arg = (void *)0;
    settings.function = KalangoTestTask;
    settings.stack_size = 1024;
    settings.priority = 9;

    TaskId id = Kalango_TaskCreate(&settings);
    fails |= TEST_ASSERT_NOT_NULL(id);
    fails |= TEST_ASSERT_EQUALS(Kalango_Sleep(10), kSuccess);

    settings.arg = (void *)1;
    id = Kalango_TaskCreate(&settings);
    fails |= TEST_ASSERT_NOT_NULL(id);
    fails |= TEST_ASSERT_EQUALS(Kalango_TaskResume(id), kSuccess);
    fails |= TEST_ASSERT_EQUALS(Kalango_Sleep(50), kSuccess);

    settings.arg = (void *)2;
    id = Kalango_TaskCreate(&settings);
    fails |= TEST_ASSERT_NOT_NULL(id);

    settings.arg = (void *)3;
    id = Kalango_TaskCreate(&settings);
    fails |= TEST_ASSERT_NOT_NULL(id);
    fails |= TEST_ASSERT_EQUALS(Kalango_Sleep(100), kSuccess);


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