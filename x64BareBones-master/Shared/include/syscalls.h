#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <stdint.h>
#include <shared_defs.h>
#include <drivers_info.h>

#define NOT_VALID_SYS_ID -1
#define NUM_SYSCALLS 39

#define KERNEL_MEM_INFO 1
#define USER_MEM_INFO 0

int64_t sys_read ( uint8_t * buffer, uint64_t amount );
int64_t sys_write ( uint64_t fd, uint8_t * buffer, uint64_t amount );
int64_t sys_get_register_snapshot ( Snapshot * snapshot );
int64_t sys_beep ( uint32_t frequency, uint32_t duration );
int64_t sys_set_font_size ( uint64_t size );
int64_t sys_clear_screen ( void );
int64_t sys_put_rectangle ( uint64_t x, uint64_t y, uint64_t width, uint64_t height, color * color );
int64_t sys_ticks_sleep ( uint64_t ticks );
int64_t sys_get_time ( LocalTime * time );
void * sys_malloc ( uint64_t size );
void sys_free ( void * p );
int64_t sys_get_pid();
int64_t sys_create_process ( main_function rip, tPriority priority, char ** argv, uint64_t argc, int64_t fds[] );
int64_t sys_nice ( tPid pid, uint64_t newPrio );
int64_t sys_kill ( tPid pid );
int64_t sys_block ( tPid pid );
int64_t sys_unblock ( tPid pid );
int64_t sys_yield();
int64_t sys_get_my_fds ( int64_t fds[CANT_FDS] );
int64_t sys_mem_info ( memoryInfo info[2] );
int64_t sys_wait ( tPid pid, int64_t * ret );
process_info_list * sys_ps();
int8_t sys_get_status ( tPid pid );
void sys_free_ps ( process_info_list * ps );
uint64_t read_with_params(uint64_t fd, uint16_t * buffer, uint64_t amount);

int64_t sys_sem_open ( int64_t sem_id, int value );
int64_t sys_sem_wait ( int64_t sem_id );
int64_t sys_sem_post ( int64_t sem_id );
int64_t sys_sem_open_get_id ( int value );
int64_t sys_sem_close ( int64_t sem_id );

int64_t sys_pipe_open ( int64_t id, tPipeMode mode );
int64_t sys_pipe_open_free ( tPipeMode mode );
int64_t sys_pipe_read ( int64_t id, uint8_t * buffer, uint64_t amount );
int64_t sys_pipe_write ( int64_t id, uint8_t * buffer, uint64_t amount );
int64_t sys_pipe_close ( int64_t id );
int64_t sys_pipe_reserve();

#endif

