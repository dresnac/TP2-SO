#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

//POR AHORA NO SE USA MAS

#include <stddef.h>
#include <process.h>
#include <scheduler.h>


typedef struct {
    int segundos;
    int minutos;
    int horas;
    int dias;
    int mes;
    int anio;
}LocalTime;

typedef struct {
	uint64_t r15, r14, r13, r12, r11, r10, r9, r8, rsi, rdi, rbp, rdx, rcx, rbx, rax;
} pushed_registers;

void empty(pushed_registers * regs);
void read(pushed_registers * regs);
uint64_t read_with_params(uint64_t fd, uint16_t * buffer, uint64_t amount);
void write(pushed_registers * regs);
void clear_screen(pushed_registers * regs);
void time(pushed_registers * regs);
void regs(pushed_registers * regs);
void change_font_size(pushed_registers * regs);
void put_rectangle(pushed_registers * regs);
void get_ticks(pushed_registers * regs);
void do_beep(pushed_registers * regs);
//void test_mm(pushed_registers * regs);

void syscallDispatcher(pushed_registers * regs);

void my_test_mm (pushed_registers * regs);
int64_t my_getpid();
//int64_t my_create_process ( main_function rip, tPriority priority, char ** my_argv, uint64_t my_argc, int64_t fds[]);
int64_t my_create_process (pushed_registers * regs);
int64_t my_nice ( uint64_t pid, uint64_t newPrio );
int64_t my_kill ( uint64_t pid );
int64_t my_block ( uint64_t pid );
int64_t my_unblock ( uint64_t pid );
int64_t my_sem_open ( char *sem_id, uint64_t initialValue );
int64_t my_sem_wait ( char *sem_id );
int64_t my_sem_post ( char *sem_id );
int64_t my_sem_close ( char *sem_id );
int64_t my_yield();
int64_t my_wait ( int64_t pid );
#endif