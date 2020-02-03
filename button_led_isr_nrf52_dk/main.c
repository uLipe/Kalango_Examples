#include <kalango_api.h>
#include <nrfx.h>
#include <hal/nrf_gpiote.h>
#include <printf.h>
#include <SEGGER_RTT.h>

//Led pins on NRF 52 DK Board --> Dont forget to shot SB5:SB8 soldering jumpers
#define LED1_PIN        17
#define BUTTON1_PIN     13

static QueueId button_queue;
static TimerId debounce_timer;

typedef struct {
    uint32_t button;
    bool pressed;
}ButtonEvent;

const char * const colors[2] = {
    RTT_CTRL_TEXT_BRIGHT_RED,
    RTT_CTRL_TEXT_BRIGHT_GREEN,
};

void GPIOTE_IRQHandler(void) {
    bool spurious = false;

    //Usage of Kalango functions MUST be done inside Irq guard sections:
    Kalango_IrqEnter();

    if(NRF_GPIOTE->EVENTS_IN[0] & 0x01) {
        NRF_GPIOTE->EVENTS_IN[0] = 0;
    } else {
        //Spurious ISR, just ignore it, no reesched will be done
        spurious = true;
    }

    if(!spurious) {
        Kalango_TimerStart(debounce_timer);
    }

    Kalango_IrqLeave();
}

static void ButtonDebounceCallBack(void *arg) {
    (void)arg;
    bool pressed = !((bool)(NRF_P0->IN & (1 << BUTTON1_PIN)));  

    ButtonEvent ev;
    ev.pressed = pressed;
    ev.button = 1;
    QueueInsert(button_queue, &ev, sizeof(ev), KERNEL_NO_WAIT);
}

static void BlinkTask(uint32_t led_arg) {
 
    ButtonEvent event;
    NRF_P0->PIN_CNF[LED1_PIN] =
          (GPIO_PIN_CNF_DIR_Output << GPIO_PIN_CNF_DIR_Pos)
        | (GPIO_PIN_CNF_INPUT_Disconnect << GPIO_PIN_CNF_PULL_Pos);

    NRF_P0->OUT = 1UL << LED1_PIN;

    NRF_P0->PIN_CNF[BUTTON1_PIN] =
          (GPIO_PIN_CNF_DIR_Input << GPIO_PIN_CNF_DIR_Pos)
        | (GPIO_PIN_CNF_DRIVE_S0S1  << GPIO_PIN_CNF_DRIVE_Pos) 
        | (GPIO_PIN_CNF_PULL_Pullup << GPIO_PIN_CNF_PULL_Pos) 
        | (GPIO_PIN_CNF_INPUT_Connect << GPIO_PIN_CNF_PULL_Pos);
        
    NVIC_DisableIRQ(GPIOTE_IRQn);
    NVIC_ClearPendingIRQ(GPIOTE_IRQn);
    NVIC_SetPriority(GPIOTE_IRQn, 0xFF);
    NVIC_EnableIRQ(GPIOTE_IRQn);

    //Confgure buttons 1 and 2 on board to generate interrups:
    NRF_GPIOTE->CONFIG[0] = ((BUTTON1_PIN << GPIOTE_CONFIG_PSEL_Pos) & GPIOTE_CONFIG_PORT_PIN_Msk) |
                            ((GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos) & GPIOTE_CONFIG_POLARITY_Msk);
    NRF_GPIOTE->CONFIG[0] |= GPIOTE_CONFIG_MODE_Event;
    
    NRF_GPIOTE->INTENSET = (1 << 0);

    //Up 16 slots to hold button events:
    button_queue = QueueCreate(128, sizeof(ButtonEvent));
    debounce_timer = TimerCreate(ButtonDebounceCallBack, 200, 0, NULL);

    for(;;) {
        //Block until a button event is sent by the ISR:
        QueueRemove(button_queue, &event, NULL, KERNEL_WAIT_FOREVER);

        printf("%s Ticks: %d ::: Heap free: %d \n\n",colors[0], Kalango_GetCurrentTicks(), GetKernelFreeBytesOnHeap());
        printf("%s Button: %d is pressed ::: Ticks: %d \n\n", RTT_CTRL_TEXT_BRIGHT_GREEN, event.button, Kalango_GetCurrentTicks());
        NRF_P0->OUT ^= 1UL << LED1_PIN;;
    }
}

int main(void) {

    TaskSettings settings;

    settings.arg = NULL;
    settings.function = (TaskFunction)BlinkTask;
    settings.priority = 8;
    settings.stack_size = 2048;

    Kalango_TaskCreate(&settings);
    Kalango_CoreStart();
}