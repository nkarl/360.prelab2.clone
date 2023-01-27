/*********** A Multitasking System *******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*#include "type.h"*/
/*#include "queue.c"  // comment in to use YOUR queue.c file*/
#include "wait.c"

/*PROC  proc[NPROC];  // 9 PROCs*/
/*PROC *freeList;     // FREE proc list*/
/*PROC *readyQueue;   // priority queue of READY procs*/
/*[> CUSTOM CODE <]*/
/*PROC *sleepList;    // list of SLEEP procs*/
/*PROC *running;      // pointer to current RUNNING proc*/

/*********** init freeList, readyQ, sleepList *********/
int init() {
    int   i;

    // 1. all PROCs in freeList
    for (i = 0; i < NPROC; ++i) {
        proc[i].pid      = i;                 // pid = 0 to NPROC-1
        proc[i].status   = FREE;
        proc[i].priority = 0;
        proc[i].next     = (PROC *)&proc[i+1];
    }
    proc[NPROC -1].next = 0;

    freeList   = &proc[0];
    readyQueue = 0;
    sleepList  = 0;

    // 2. create P0 as the initial running process
    running           = dequeue(&freeList);
    running->status   = READY;
    running->priority = 0;  // P0 has lowest prioriy 0

    running->child    = 0;
    running->sibling  = 0;
    running->parent   = running;

    printf("init complete: P0 running\n");
    printFreeList(freeList);
    return 0;
}


/* process body function */
int body() {
    char command[64];
    while (1) {
        printf("\n***************************************\n");
        printf("proc %d running: Parent=%d\n", running->pid, running->ppid);

        printChildList  ( running    ) ;
        printFreeList   ( freeList   ) ;
        printReadyQueue ( readyQueue ) ;
        printSleepList  ( sleepList  ) ;

        printf("\ninput a command: [ps|fork|switch] | [sleep|wakeup|exit|wait] : ");
        fgets(command, 64, stdin);
        command[strlen(command) - 1] = 0;  // kill \n at end

        if       ( !strcmp(command, "ps"     )) ps()           ;
        else if  ( !strcmp(command, "fork"   )) kfork(body, 1) ;
        else if  ( !strcmp(command, "switch" )) tswitch()      ;
        else if  ( !strcmp(command, "sleep"  )) ksleep(123);
        else if  ( !strcmp(command, "wakeup" )) kwakeup(234)   ;
        else
            printf("invalid command\n");
    }
}


/* forking a process */
/*int kfork(int(*func), int priority) {*/
int kfork(int(*func), int priority) {
    PROC *p;
    int   i;
    /*** get a proc from freeList for child proc: ***/
    p = dequeue(&freeList)       ;
    if (!p) {
        printf("no more proc\n") ;
        return -1;
    }

    /* initialize the new proc and its stack */
    p->status   = READY;
    p->priority = priority;  // for ALL PROCs except P0

    p->ppid     = running->pid;
    p->parent   = running;
    p->child    = p->sibling = 0;

    //                    -1   -2  -3  -4  -5  -6  -7  -8   -9
    // kstack contains: |retPC|eax|ebx|ecx|edx|ebp|esi|edi|eflag|
    for (i = 1; i < 10; ++i) {
        p->kstack[SSIZE - i] = 0;
    }

    p->kstack[SSIZE - 1] = (int)func;
    p->saved_sp          = &(p->kstack[SSIZE - 9]);

    enqueue(&readyQueue, p);
    return p->pid;
}


/*************** main() ******************************/
int main() {
    printf ( "Welcome to 360 Multitasking System\n") ;
    init   ( )                                       ;
    printf ( "P0 fork P1\n")                         ;
    kfork  ( body, 1)                                ;

    while (1) {
        if (readyQueue) {
            printf("P0: switch task\n");
            tswitch();
        }
    }
}


/*************** scheduler ***************************/
int scheduler() {
    printf("proc %d in scheduler()\n", running->pid) ;

    if (running->status == READY)
        enqueue(&readyQueue, running)                ;
    printReadyQueue(readyQueue)                      ;

    running = dequeue(&readyQueue)                   ;
    printf("next running = %d\n", running->pid)      ;
    return 0;
}


/* print process status */
int ps() {
    int   i;
    PROC *p;
    printf("pid   ppid    status\n")                ;
    printf("--------------------\n")                ;
    for (i = 0; i < NPROC ; ++i) {
        p = &proc[i]                                ;
        printf(" %d      %d     ", p->pid, p->ppid) ;
        if (p == running)
            printf("%s\n", pstatus[5])              ;
        else
            printf("%s\n", pstatus[p->status])      ;
    }
    return 0;
}

