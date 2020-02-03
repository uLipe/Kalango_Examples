#include <kalango_api.h>
#include <nrfx.h>
#include <printf.h>
#include <SEGGER_RTT.h>

//Led pins on NRF 52 DK Board --> Dont forget to shot SB5:SB8 soldering jumpers
#define LED1_PIN 17
#define LED2_PIN 18
#define LED3_PIN 19
#define LED4_PIN 20

static TimerId led_timers[4];

const char * const colors[] = {
    RTT_CTRL_TEXT_BRIGHT_GREEN,
};

static void LedTimerCallback(void *user) {
    uint32_t user_led = (uint32_t) user;
    NRF_P0->OUT ^= 1UL << user_led;
}

static void InfoTask(void *arg) {
    (void)arg;

    NRF_P0->PIN_CNF[LED1_PIN] =
          (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos)
        | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_PULL_Pos);

    NRF_P0->PIN_CNF[LED2_PIN] =
          (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos)
        | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_PULL_Pos);

    NRF_P0->PIN_CNF[LED3_PIN] =
          (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos)
        | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_PULL_Pos);

    NRF_P0->PIN_CNF[LED4_PIN] =
          (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos)
        | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_PULL_Pos);

    for(int i =0 ; i < 4 ; i++) {
        led_timers[i] = Kalango_TimerCreate(LedTimerCallback, 
                                            (i + 1) * 100,
                                            (i + 1) * 100,
                                            (void *)(LED1_PIN + i)); 

        Kalango_TimerStart(led_timers[i]);
    }

    for(;;) {

        //Print system info each 1 second:
        Kalango_Sleep(1000);
        printf("%s Ticks: %d ::: Heap free: %d \n\n",
                colors[0], Kalango_GetCurrentTicks(), GetKernelFreeBytesOnHeap());
    }
}

int main(void) {

    TaskSettings settings;

    settings.arg = NULL;
    settings.function = (TaskFunction)InfoTask;
    settings.priority = 8;
    settings.stack_size = 2048;

    Kalango_TaskCreate(&settings);

    Kalango_CoreStart();
}