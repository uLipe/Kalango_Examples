#pragma once 

#ifndef CONFIG_KERNEL_BLOCKS
#define CONFIG_KERNEL_BLOCKS 16
#endif

typedef struct {
    sys_dlist_t task_list[CONFIG_PRIORITY_LEVELS];
    uint32_t ready_task_bitmap;
    uint32_t lock_level;
}TaskPriorityList;

typedef struct {
    void (*timeout_callback) (void *);
    void *user_data;
    bool is_task;
    uint32_t next_wakeup_tick;
    bool expired;
    TaskPriorityList *bonded_list; 
    sys_dnode_t timed_node;
}Timeout;

typedef struct {
    uint8_t *stackpointer;
    uint32_t stack_size;
    void (*entry_point) (void *);
    void *arg1;
    uint32_t priority;
    uint32_t state;
    Timeout timeout;
    sys_dnode_t ready_node;
} TaskControBlock;

typedef struct {
    uint32_t count;
    uint32_t limit;
    TaskPriorityList pending_tasks;
}Semaphore;

typedef struct {
    bool owned;
    void *owner;
    uint32_t recursive_taking_count;
    uint32_t old_priority;
    TaskPriorityList pending_tasks;
}Mutex;

typedef struct {
    void (*callback) (void *);
    void *user_data;
    bool periodic;
    bool expired;
    bool running;
    uint32_t expiry_time;
    uint32_t period_time;
    Timeout timeout;
}Timer;

typedef struct {
    uint8_t *buffer;
    uint32_t tail;
    uint32_t head;
    uint32_t slot_size;
    uint32_t noof_slots;
    uint32_t available_slots;
    bool full;
    bool empty;
    TaskPriorityList writer_tasks_pending;
    TaskPriorityList reader_tasks_pending;
} Queue;
