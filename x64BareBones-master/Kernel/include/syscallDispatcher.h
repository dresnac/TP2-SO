#ifndef SYSCALLDISPATCHER_H
#define SYSCALLDISPATCHER_H

#include <stddef.h>
#include <stack_registers.h>
#include <process.h>
#include <scheduler.h>

typedef struct {
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rbp;
    uint64_t rsp;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
} Snapshot;


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

void syscallDispatcher(stack_registers * regs);

int64_t my_getpid();
int64_t my_create_process ( main_function rip, tPriority priority, char ** my_argv, uint64_t my_argc, int64_t fds[]);
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