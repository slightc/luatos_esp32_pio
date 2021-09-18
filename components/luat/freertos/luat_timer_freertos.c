
#include "luat_base.h"
#include "luat_malloc.h"
#include "luat_timer.h"
#include "luat_msgbus.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/timers.h"

#define LUAT_LOG_TAG "luat.timer"
#include "luat_log.h"

#define FREERTOS_TIMER_COUNT 32
static luat_timer_t* timers[FREERTOS_TIMER_COUNT] = {0};

static void luat_timer_callback(TimerHandle_t xTimer) {
    //LLOGD("timer callback");
    rtos_msg_t msg;
    luat_timer_t *timer = (luat_timer_t*) pvTimerGetTimerID(xTimer);
    msg.handler = timer->func;
    msg.ptr = timer;
    msg.arg1 = 0;
    msg.arg2 = 0;
    int re = luat_msgbus_put(&msg, 0);
    //LLOGD("timer msgbus re=%ld", re);
}

static int nextTimerSlot() {
    for (size_t i = 0; i < FREERTOS_TIMER_COUNT; i++)
    {
        if (timers[i] == NULL) {
            return i;
        }
    }
    return -1;
}

int luat_timer_start(luat_timer_t* timer) {
    TimerHandle_t os_timer;
    int timerIndex;
    //LLOGD(">>luat_timer_start timeout=%ld", timer->timeout);
    timerIndex = nextTimerSlot();
    //LLOGD("timer id=%ld", timerIndex);
    if (timerIndex < 0) {
        return 1; // too many timer!!
    }
    os_timer = xTimerCreate("luat_timer", timer->timeout / portTICK_RATE_MS, timer->repeat, timer, luat_timer_callback);
    //LLOGD("timer id=%ld, osTimerNew=%p", timerIndex, os_timer);
    if (!os_timer) {
        return -1;
    }
    timers[timerIndex] = timer;
    
    timer->os_timer = os_timer;
    int re = xTimerStart(os_timer, 0);
    //LLOGD("timer id=%ld timeout=%ld start=%ld", timerIndex, timer->timeout, re);
    if (re != pdPASS) {
        xTimerDelete(os_timer, 0);
        timers[timerIndex] = 0;
    }
    return re == pdPASS ? 0 : -1;
}

int luat_timer_stop(luat_timer_t* timer) {
    if (!timer)
        return 1;
    for (size_t i = 0; i < FREERTOS_TIMER_COUNT; i++)
    {
        if (timers[i] == timer) {
            timers[i] = NULL;
            break;
        }
    }
    xTimerStop((TimerHandle_t)timer->os_timer, 10);
    xTimerDelete((TimerHandle_t)timer->os_timer, 10);
    return 0;
};

luat_timer_t* luat_timer_get(size_t timer_id) {
    for (size_t i = 0; i < FREERTOS_TIMER_COUNT; i++)
    {
        if (timers[i] && timers[i]->id == timer_id) {
            return timers[i];
        }
    }
    return NULL;
}


int luat_timer_mdelay(size_t ms) {
    if (ms > 0) {
        vTaskDelay(ms / portTICK_RATE_MS);
    }
    return 0;
}

