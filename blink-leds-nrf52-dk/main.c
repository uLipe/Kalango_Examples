#include <kalango_api.h>
#include <nrfx.h>

//Led pins on NRF 52 DK Board --> Dont forget to shot SB5:SB8 soldering jumpers
#define LED1_PIN 17
#define LED2_PIN 18
#define LED3_PIN 19
#define LED4_PIN 20

static void BlinkTask(uint32_t led_arg) {
 
    NRF_P0->PIN_CNF[led_arg] =
          (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos)
        | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_PULL_Pos);

    for(;;) {
        float scale = ((float)(20 - led_arg) + 1.0f) * 100.0f;

        Kalango_Sleep((uint32_t)scale);
        NRF_P0->OUT ^= 1UL << led_arg;;
    }
}

int main(void) {

    TaskSettings settings;

    settings.arg = (void *)LED1_PIN;
    settings.function = (TaskFunction)BlinkTask;
    settings.priority = 8;
    settings.stack_size = 2048;

    Kalango_TaskCreate(&settings);

    settings.arg = (void *)LED2_PIN;
    settings.function = (TaskFunction)BlinkTask;
    settings.priority = 4;
    settings.stack_size = 2048;

    Kalango_TaskCreate(&settings);

    settings.arg = (void *)LED3_PIN;
    settings.function = (TaskFunction)BlinkTask;
    settings.priority = 7;
    settings.stack_size = 2048;

    Kalango_TaskCreate(&settings);

    settings.arg = (void *)LED4_PIN;
    settings.function = (TaskFunction)BlinkTask;
    settings.priority = 1;
    settings.stack_size = 2048;

    Kalango_TaskCreate(&settings);
    Kalango_CoreStart();
}