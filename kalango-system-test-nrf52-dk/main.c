#include <kalango_api.h>
#include <nrfx.h>
#include "test_macros.h"

extern KernelResult KalangoSystemTest(void);

static void SystemTask(void *arg) {
 
    for(;;) {
        TEST_LOG_W("System Test is starting!");
        KernelResult ret = KalangoSystemTest();
        int final_result = TEST_ASSERT_EQUALS(ret, kSuccess);
        if (final_result) {
            TEST_LOG_W("Some tests failed!");
        } else {
            TEST_LOG_INFO("All tests passed!");
        }
        Kalango_Sleep(100);
        Kalango_TaskDelete(Kalango_GetCurrentTaskId());
    }
}

void _putchar(char c) {
    SEGGER_RTT_Write(0, &c, 1);
}

int main(void) {

    TaskSettings settings;

    settings.arg = NULL;
    settings.function = (TaskFunction)SystemTask;
    settings.priority = 16;
    settings.stack_size = 4096;

    Kalango_TaskCreate(&settings);
    Kalango_CoreStart();
}