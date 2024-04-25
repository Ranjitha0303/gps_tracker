#ifndef _NWY_OSI_API_H_
#define _NWY_OSI_API_H_

#include "nwy_common.h"
#ifdef __cplusplus
extern "C" {
#endif

#include "osi_api.h"

typedef struct osiTimer* nwy_osi_timer_t;
typedef struct osiThread* nwy_osi_thread_t;
typedef struct osiMessageQueue* nwy_osi_msg_queue_t;
typedef struct osiMutex* nwy_osi_mutex_t;
typedef struct osiSemaphore* nwy_osi_semaphore_t;
typedef struct osiPipe* nwy_pipe_t;
typedef void * nwy_timer_cb_para_t;

typedef void (*nwy_task_cb_func_t)(void *ctx);
typedef void (*nwy_timer_cb_func_t)(void *ctx);
typedef void (*nwy_pipe_event_cb_func_t)(void *param, unsigned event);


typedef enum
{
    NWY_OSI_SHUTDOWN_RESET = 0,               ///< normal reset
    NWY_OSI_SHUTDOWN_FORCE_DOWNLOAD = 0x5244, ///< 'RD' reset to force download mode
    NWY_OSI_SHUTDOWN_DOWNLOAD = 0x444e,       ///< 'DN' reset to download mode
    NWY_OSI_SHUTDOWN_BL_DOWNLOAD = 0x4244,    ///< 'BD' reset to bootloader download mode
    NWY_OSI_SHUTDOWN_CALIB_MODE = 0x434c,     ///< 'CL' reset to calibration mode
    NWY_OSI_SHUTDOWN_NB_CALIB_MODE = 0x4e43,  ///< 'NC' reset to NB calibration mode
    NWY_OSI_SHUTDOWN_BBAT_MODE = 0x4241,      ///< 'BA' reset to BBAT mode
    NWY_OSI_SHUTDOWN_UPGRADE = 0x4654,        ///< 'FT' reset to upgrade mode
    NWY_OSI_SHUTDOWN_POWER_OFF = 0x4f46,      ///< 'OF' power off
    NWY_OSI_SHUTDOWN_PSM_SLEEP = 0x5053,      ///< 'PS' power saving mode
} nwy_osiShutdownMode_t;
typedef enum nwy_osiThreadPriority
{
    NWY_OSI_PRIORITY_IDLE = 1, // reserved
    NWY_OSI_PRIORITY_LOW = 8,
    NWY_OSI_PRIORITY_BELOW_NORMAL = 16,
    NWY_OSI_PRIORITY_NORMAL = 24,
    NWY_OSI_PRIORITY_ABOVE_NORMAL = 32,
    NWY_OSI_PRIORITY_HIGH = 40,
    NWY_OSI_PRIORITY_REALTIME = 48,
    NWY_OSI_PRIORITY_HISR = 56, // reserved
} nwy_osiThreadPriority;

typedef enum
{
    NWY_OSA_NO_SUSPEND = 0,
    NWY_OSA_SUSPEND = -1,
}nwy_timeout_type_e;

typedef enum
{
    NWY_TIMER_ONE_TIME = 0,
    NWY_TIMER_PERIODIC = 1,
}nwy_timer_type_e;


typedef enum
{
    NWY_TIME_ZONE_DISABLE = 0,
    NWY_TIME_ZONE_ENABLE = 1
}nwy_time_zone_switch_e;

/* Task states returned by eTaskGetState. */
typedef enum
{
    NWY_RUNNING = 0,    /* A task is querying the state of itself, so must be running. */
    NWY_READY,          /* The task being queried is in a read or pending ready list. */
    NWY_BLOCKED,        /* The task being queried is in the Blocked state. */
    NWY_SUSPENDED,      /* The task being queried is in the Suspended state, or is in the Blocked state with an infinite time out. */
    NWY_DELETED,        /* The task being queried has been deleted, but its TCB has not yet been freed. */
    NWY_INVALID         /* Used as an 'invalid state' value. */
} nwy_thread_state_e;


typedef struct nwy_thread_info_t
{
    nwy_osi_thread_t thread_handle;         /* The handle of the task to which the rest of the information in the structure relates. */
    const char *thread_name;            /* A pointer to the task's name.  This value will be invalid if the task was deleted since the structure was populated! */ /*lint !e971 Unqualified char types are allowed for strings and single characters only. */
    unsigned long thread_number;        /* A number unique to the task. */
    nwy_thread_state_e thread_current_state;      /* The state in which the task existed when the structure was populated. */
    unsigned long thread_current_priority;  /* The priority at which the task was running (may be inherited) when the structure was populated. */
    unsigned long thread_base_priority;     /* The priority to which the task will return if the task's current priority has been inherited to avoid unbounded priority inversion when obtaining a mutex.  Only valid if configUSE_MUTEXES is defined as 1 in FreeRTOSConfig.h. */
    uint32_t thread_run_time_counter;       /* The total run time allocated to the task so far, as defined by the run time stats clock.  See http://www.freertos.org/rtos-run-time-stats.html.  Only valid when configGENERATE_RUN_TIME_STATS is defined as 1 in FreeRTOSConfig.h. */
    uint32_t *thread_stack_base;        /* Points to the lowest address of the task's stack area. */
    uint16_t thread_stack_highwatermark;    /* The minimum amount of stack space that has remained for the task since the task was created.  The closer this value is to zero the closer the task has come to overflowing its stack.convert word count to byte count should multiply 4 */
} nwy_thread_info_t;

#ifndef NWY_TIME_STRUCT
#define NWY_TIME_STRUCT
typedef struct nwy_time_t
{
    unsigned short year; // [0,127] Year-2000, tm_year: Year-1900
    unsigned char mon;  // [1,12], tm_mon: (0-11)
    unsigned char day;  // [1,31], tm_mday: (1-31)
    unsigned char hour; // [0,23], tm_hour: (0-23)
    unsigned char min;  // [0,59], tm_min: (0-59)
    unsigned char sec;  // [0,59], tm_sec: (0-60)
    unsigned char wday; // [1,7] 1 for Monday, tm_wday: (0-6) 0 for Sunday
} nwy_time_t;
#endif

typedef struct nwy_event_msg_t
{
    uint32 id;     ///< event identifier
    uint32 param1; ///< 1st parameter
    uint32 param2; ///< 2nd parameter
    uint32 param3; ///< 3rd parameter
} nwy_event_msg_t;

typedef struct nwy_timer_para_t
{
    int expired_time;
    nwy_timer_type_e type;
    nwy_osi_thread_t thread_hdl;
    nwy_timer_cb_func_t cb;
    nwy_timer_cb_para_t cb_para;
}nwy_timer_para_t;

// set app version , and user <at+nappcheck?> to read
bool nwy_app_version(char* ver, char* build_time);


//thread API
int nwy_create_thread(nwy_osi_thread_t *hdl, uint32 stack_size,
            uint8 priority, char *task_name, nwy_task_cb_func_t func, void *argv, uint32 event_count);

int nwy_create_thread_withstack(nwy_osi_thread_t *hdl, void *stack, uint32 stack_size,
            uint8 priority, char *task_name, nwy_task_cb_func_t func, void *argv, uint32 event_count);

int nwy_get_current_thread(nwy_osi_thread_t *hdl);
int nwy_get_thread_priority(nwy_osi_thread_t hdl, uint32_t *priority);
int nwy_set_thread_priority(nwy_osi_thread_t hdl, uint32_t new_pri, uint32_t *old_pri);
int nwy_suspend_thread(nwy_osi_thread_t hdl);
int nwy_resume_thread(nwy_osi_thread_t hdl);
bool nwy_send_thread_event(nwy_osi_thread_t hdl, nwy_event_msg_t *event, uint32 timeout);
bool nwy_wait_thread_event(nwy_osi_thread_t hdl, nwy_event_msg_t *event, uint32 timeout);
uint32_t nwy_get_thread_pendingevent_cnt(nwy_osi_thread_t hdl);
uint32_t nwy_get_thread_spaceevent_cnt(nwy_osi_thread_t hdl);
int nwy_get_thread_info(nwy_osi_thread_t hdl, nwy_thread_info_t *thread_info);
int nwy_get_thread_list(char *thread_list);
int nwy_get_thread_runtime_stats(char *stats_buf);
uint32_t nwy_set_rrc_release(uint32_t val);
void nwy_sleep(uint32 ms);
void nwy_usleep(uint32 us);
int nwy_exit_thread_self();
int nwy_get_thread_free_stack(nwy_osi_thread_t hdl);


// get sys time from start
int64_t nwy_get_ms(void);
#ifdef FEATURE_8850_8910_API_SAME
extern void nwy_set_time(nwy_time_t *julian_time, char timezone);
extern int nwy_get_time(nwy_time_t *julian_time, char *timezone);
#else
extern void nwy_set_time(nwy_time_t *julian_time, int timezone);
extern int nwy_get_time(nwy_time_t *julian_time, int *timezone);
#endif
//message queue API
int nwy_msg_queue_create(nwy_osi_msg_queue_t *msg_q, char *name, uint32 msg_size,
                    uint32 msg_num);
int nwy_msg_queue_delete(nwy_osi_msg_queue_t msg_q);
int nwy_msg_queue_send(nwy_osi_msg_queue_t msg_q, uint32 size, const void *msg_ptr, uint32 timeout);
int nwy_msg_queue_recv(nwy_osi_msg_queue_t msg_q,uint32 size, void *msg_ptr, uint32 timeout);
int nwy_msg_queue_get_pendingevent_cnt(nwy_osi_msg_queue_t msg_q,uint32 *size);
int nwy_msg_queue_get_spaceevent_cnt(nwy_osi_msg_queue_t msg_q,uint32 *size);


//mutex API
int nwy_create_mutex(nwy_osi_mutex_t *mutex);
int nwy_lock_mutex(nwy_osi_mutex_t mutex, int time_out);
int nwy_unlock_mutex(nwy_osi_mutex_t mutex);
int nwy_delete_mutex(nwy_osi_mutex_t mutex);


//Semaphore API
int nwy_semaphore_create(nwy_osi_semaphore_t *sem, uint32 max_count, uint32 init_count);
int nwy_semaphore_acquire(nwy_osi_semaphore_t sem, int time_out);
int nwy_semahpore_release(nwy_osi_semaphore_t sem);
int nwy_semahpore_delete(nwy_osi_semaphore_t sem);

//timer API
int nwy_sdk_timer_create(nwy_osi_timer_t *timer, nwy_osi_thread_t hdl, nwy_timer_cb_func_t cb, nwy_timer_cb_para_t ctx);
int nwy_sdk_timer_create_ex(nwy_osi_timer_t *timer, nwy_timer_para_t *para);
int nwy_sdk_timer_destroy(nwy_osi_timer_t timer);
int nwy_sdk_timer_start(nwy_osi_timer_t timer, uint32 experi_time, nwy_timer_cb_func_t cb, nwy_timer_cb_para_t ctx, nwy_timer_type_e type);
int nwy_sdk_timer_start_ex(nwy_osi_timer_t timer, nwy_timer_para_t *para);
int nwy_sdk_timer_stop(nwy_osi_timer_t timer);

//pipe
int nwy_osi_pipe_create(nwy_pipe_t *pipe_hdl,unsigned size);
void nwy_osi_pipe_delete(nwy_pipe_t pipe);
int nwy_osi_pipe_read(nwy_pipe_t pipe, void *buf, unsigned size);
int nwy_osi_pipe_write(nwy_pipe_t pipe, void *buf, unsigned size);
int nwy_osi_pipe_set_write_cb(nwy_pipe_t pipe, unsigned mask, nwy_pipe_event_cb_func_t cb, void *ctx);
int nwy_osi_pipe_set_read_cb(nwy_pipe_t pipe, unsigned mask, nwy_pipe_event_cb_func_t cb, void *ctx);

/**
 * \brief monoclinic system time in microsecond
 *
 * The monoclinic system time in unit of microsecond.
 *
 * nwy_get_up_time_us_int64 is highly recommended,
 * because nwy_get_up_time_us will overflow after 71 minutes.
 *
 * \return      32 bits monoclinic system time in microseconds
 */
int nwy_get_up_time_us(void);
/**
 * \brief monoclinic system time in microsecond
 *
 * The monoclinic system time in unit of microsecond.
 *
 * \return      64 bits monoclinic system time in microseconds
 */
#ifdef FEATURE_NWY_MICROPYTHON
typedef long long           int64; //qj delete _com_dtypes.h
#endif
int64 nwy_get_up_time_us_int64(void);

int nwy_set_time_zone_switch(nwy_time_zone_switch_e status);

int nwy_get_time_zone_switch(nwy_time_zone_switch_e *status);
void nwy_open_log(int tag, const char *format, ...);

#ifdef __cplusplus
   }
#endif


#endif
