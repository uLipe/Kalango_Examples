#include <queue.h>
#if CONFIG_ENABLE_QUEUES > 0

QueueId QueueCreate(uint32_t noof_slots, uint32_t slot_size) {
    ASSERT_KERNEL(noof_slots, NULL);
    ASSERT_KERNEL(slot_size, NULL);

    CoreInit();
    CoreSchedulingSuspend();

    Queue *queue = AllocateQueueObject();
    if(queue == NULL) {
        CoreSchedulingResume();
        return NULL;
    }

    queue->empty = true;
    queue->noof_slots = noof_slots;
    queue->slot_size = slot_size;
    queue->full = false;
    queue->head = 0;
    queue->tail = 0;
    queue->available_slots = noof_slots;

    queue->buffer = AllocateRawBuffer(noof_slots * slot_size);
    if(!queue->buffer) {
        FreeQueueObject(queue);
        CoreSchedulingResume();
        return NULL;
    }

    KernelResult r = CoreInitializeTaskList(&queue->reader_tasks_pending);
    if(r != kSuccess) {
        FreeRawBuffer(queue->buffer);
        FreeQueueObject(queue);
        CoreSchedulingResume();
        return NULL;
    }
    
    r = CoreInitializeTaskList(&queue->writer_tasks_pending);
    if(r != kSuccess) {
        FreeRawBuffer(queue->buffer);
        FreeQueueObject(queue);
        CoreSchedulingResume();
        return NULL;
    }

    CoreSchedulingResume();
    return ((QueueId)queue);
}

KernelResult QueueInsert(QueueId queue, void *data, uint32_t data_size, uint32_t timeout) {
    ASSERT_PARAM(queue);
    ASSERT_PARAM(data);
    
    //If called from ISR, requires a IRQ safe block
    if(ArchInIsr()) {
        if(!ArchGetIsrNesting()){
            return kErrorInvalidKernelState;
        } 
    }

    Queue *q = (Queue *)queue;
    CoreSchedulingSuspend();

    if(data_size != q->slot_size) {
        CoreSchedulingResume();
        return kErrorInvalidParam;
    }

    if(!q->full) {

        uint32_t write_loc = q->tail * q->slot_size;
        
        q->empty = false;
        memcpy(&q->buffer[write_loc], data, data_size);

        ArchCriticalSectionEnter();
        if(q->available_slots)
            q->available_slots--;
        ArchCriticalSectionExit();

        q->tail = ((q->tail + 1) % (q->noof_slots));

        if(!q->available_slots){
            q->full = true;
        }

        if(NothingToSched(&q->reader_tasks_pending)) {
            CoreSchedulingResume();
            return kSuccess;
        } else {

            CoreUnpendNextTask(&q->reader_tasks_pending);

            //Not need to reeschedule a new unpended task in a ISR,
            //it will be done a single time after all ISRs
            //get processed 
            if(ArchInIsr()) {
                CoreSchedulingResume();
                return kSuccess;
            } else {
                return CheckReschedule();
            }

        }
    }

    if(timeout == KERNEL_NO_WAIT) {
        CoreSchedulingResume();
        return kErrorBufferFull;
    }

    if(ArchInIsr()) {
        CoreSchedulingResume();
        return kErrorInsideIsr;
    }

    TaskControBlock *task = CoreGetCurrentTask();
    CoreMakeTaskPending(task, TASK_STATE_PEND_QUEUE, &q->writer_tasks_pending);
    AddTimeout(&task->timeout, timeout, NULL, NULL, true, &q->writer_tasks_pending);
    CheckReschedule();

    CoreSchedulingSuspend();

    if(task->timeout.expired) {
        CoreSchedulingResume();
        return kErrorTimeout;
    } else {
        uint32_t write_loc = q->tail * q->slot_size;
        
        q->empty = false;
        memcpy(&q->buffer[write_loc], data, data_size);

        ArchCriticalSectionEnter();
        if(q->available_slots)
            q->available_slots--;
        ArchCriticalSectionExit();

        q->tail = ((q->tail + 1) % (q->noof_slots));

        if(!q->available_slots){
            q->full = true;
        }

        CoreSchedulingResume();
        return kSuccess;        
    }
}

KernelResult QueuePeek(QueueId queue, void *data, uint32_t *data_size, uint32_t timeout) {
    ASSERT_PARAM(queue);
    ASSERT_PARAM(data);
    (void)data_size;

    //If called from ISR, requires a IRQ safe block
    if(ArchInIsr()) {
        if(!ArchGetIsrNesting()){
            return kErrorInvalidKernelState;
        } 
    }

    Queue *q = (Queue*)queue;

    CoreSchedulingSuspend();
    
    if(!q->empty) {
        uint32_t read_loc = q->head * q->slot_size;
        memcpy(data, &q->buffer[read_loc], q->slot_size);
        CoreSchedulingResume();
        return kSuccess;
    }


    if(timeout == KERNEL_NO_WAIT) {
        CoreSchedulingResume();
        return kErrorBufferEmpty;
    }

    if(ArchInIsr()) {
        CoreSchedulingResume();
        return kErrorInsideIsr;
    }

    TaskControBlock *task = CoreGetCurrentTask();
    CoreMakeTaskPending(task, TASK_STATE_PEND_QUEUE, &q->reader_tasks_pending);
    AddTimeout(&task->timeout, timeout, NULL, NULL, true, &q->reader_tasks_pending);
    CheckReschedule();

    CoreSchedulingSuspend();

    if(task->timeout.expired) {
        CoreSchedulingResume();
        return kErrorTimeout;
    } else {
        uint32_t read_loc = q->head;
        memcpy(data, &q->buffer[read_loc], q->slot_size);
        CoreSchedulingResume();
        return kSuccess;
    }
}

KernelResult QueueRemove(QueueId queue, void *data, uint32_t *data_size, uint32_t timeout) {
    ASSERT_PARAM(queue);
    ASSERT_PARAM(data);
    (void)data_size;

    //If called from ISR, requires a IRQ safe block
    if(ArchInIsr()) {
        if(!ArchGetIsrNesting()){
            return kErrorInvalidKernelState;
        } 
    }

    Queue *q = (Queue*)queue;

    CoreSchedulingSuspend();

    if(!q->empty) {
        uint32_t read_loc = q->head * q->slot_size;
        
        q->full = false;
        memcpy(data, &q->buffer[read_loc], q->slot_size);

        q->head = ((q->head + 1) % q->slot_size);

        ArchCriticalSectionEnter();
        if(q->available_slots < 0xFFFFFFFF)
            q->available_slots++;
        q->head = read_loc;
        ArchCriticalSectionExit();
     
        if(q->available_slots >= q->noof_slots) {
            q->available_slots = q->noof_slots;
            q->empty = true;
        }

        if(NothingToSched(&q->writer_tasks_pending)) {
            CoreSchedulingResume();
            return kSuccess;
        } else {

            CoreUnpendNextTask(&q->writer_tasks_pending);
            
            //Not need to reeschedule a new unpended task in a ISR,
            //it will be done a single time after all ISRs
            //get processed 
            if(ArchInIsr()) {
                CoreSchedulingResume();
                return kSuccess;
            } else {
                return CheckReschedule();
            }
        }

    }

    if(timeout == KERNEL_NO_WAIT) {
        CoreSchedulingResume();
        return kErrorBufferEmpty;
    }

    if(ArchInIsr()) {
        CoreSchedulingResume();
        return kErrorInsideIsr;
    }

    TaskControBlock *task = CoreGetCurrentTask();
    CoreMakeTaskPending(task, TASK_STATE_PEND_QUEUE, &q->reader_tasks_pending);
    AddTimeout(&task->timeout, timeout, NULL, NULL, true, &q->reader_tasks_pending);
    CheckReschedule();

    CoreSchedulingSuspend();

    if(task->timeout.expired) {
        CoreSchedulingResume();
        return kErrorTimeout;
    } else {

        uint32_t read_loc = q->head * q->slot_size;
        
        q->full = false;
        memcpy(data, &q->buffer[read_loc], q->slot_size);

        read_loc = ((read_loc + 1) % q->slot_size);

        ArchCriticalSectionEnter();
        if(q->available_slots < 0xFFFFFFFF)
            q->available_slots++;
        q->head = read_loc;
        ArchCriticalSectionExit();
     
        if(q->available_slots >= q->noof_slots) {
            q->available_slots = q->noof_slots;
            q->empty = true;
        }

        CoreSchedulingResume();
        return kSuccess;
    }
}

KernelResult QueueDelete(QueueId queue) {
    ASSERT_PARAM(queue);
    ASSERT_KERNEL(!ArchInIsr(), kErrorInsideIsr);

    Queue *q = (Queue *)queue;

    CoreSchedulingSuspend();
    CoreMakeAllTasksReady(&q->writer_tasks_pending);
    CoreMakeAllTasksReady(&q->reader_tasks_pending);
    FreeRawBuffer(q->buffer);
    FreeQueueObject(q);

    return  CheckReschedule();
}

#endif