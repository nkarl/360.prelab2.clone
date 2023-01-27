#include "type.h"

PROC  proc[NPROC];  // 9 PROCs
PROC *freeList;     // FREE proc list
PROC *readyQueue;   // priority queue of READY procs
PROC *sleepList;    // list of SLEEP procs
PROC *running;      // pointer to current RUNNING proc

/* process status */
char *pstatus[] = {"FREE", "READY", "SLEEP", "BLOCK", "ZOMBIE", "RUNNING"};

void printProcessInfo(PROC *p) {
    /*printf("%d", p->next);      // next proc pointer*/
    /*printf("%d", p->saved_sp);         // at offset 4: do NOT alter*/
    printf("\npid=%d", p->pid);              // pid = 0 to NPROC-1 printf("%d", ppid);             // parent pid 
    printf("\nstatus=%d", p->status);           // PROC status: FREE|READY|etc 
    printf("\nprio=%d", p->priority);         // scheduling priority 
    printf("\nevent=%d", p->event);            // event to sleep on 
    printf("\nexitCode=%d\n", p->exitCode);         // exit code value

    /*printf("%d", p->child);     // first child PROC pointer*/
    /*printf("%d", p->sibling);   // sibling PROC pointer*/
    /*printf("%d", p->parent);    // parent PROC pointer*/
}

int enqueue(PROC **queue, PROC *p)  // NOTE **queue
{
    // enter p into queue by priority; FIFO if same priority
    PROC *q = *queue;
    /* if queue is empty or p.prio > q.prio */
    if (q == 0 || p->priority > q->priority) {
        *queue = p;
        p->next = q;
        printf("\nthe p process:\n");
        printProcessInfo(p);
        /*printf("\nthe q process:\n");*/
        /*printProcessInfo(q);*/
    } else {
        /* loop until the end of the queue && p.prio > q.prio */
        while (q->next && p->priority <= q->next->priority) {
            q = q->next;
        }
        p->next = q->next;
        q->next = p;
    }
    /* placeholder return */
    return 0;
}

// remove and return first PROC from queue
PROC *dequeue(PROC **queue)  // NOTE **queue
{
    // remove and return FIRST element in queue
    PROC *p = *queue;
    if (p)
        *queue = (*queue)->next;

    return p;
}

int printFreeList(PROC *p)  // print list p
{
    PROC *q = p;
    // print the list as  freeList=[pid pri]->[pid pri] ->..-> NULL
    printf("\nfreeList     =", ' ');
    for (; q;) {
        printf("[%d %d]->", q->pid, q->priority);
        q = q->next;
    }
    printf("NULL");
    /* placeholder return */
    return 0;
}

int printSleepList(PROC *p)  // print list p
{
    // print the list as  sleepList=[pid event]->[pid event] ->..-> NULL
    printf("\nsleepList    =", ' ');
    for (; p; ++p) {
        printf("[%d event=%d]->", p->pid, p->event);
    }
    printf("NULL\n");
    /* placeholder return */
    return 0;
}

int printChildList(PROC *p)  // print list p
{
    // print the list as  childList=[pid status]->[pid status] ->..-> NULL
    printf("\nchildList    =", ' ');
    for (; p;) {
        printf("[%d status=%d]->", p->pid, p->status);
        p = p->child;
    }
    printf("NULL");
    /* placeholder return */
    return 0;
}

int printList(char *name, PROC *p) {
    printf("\n%s%3c=", name, ' ');
    while (p) {
        printf("[%d %d]->", p->pid, p->priority);
        p = p->next;
    }
    printf("NULL");

    /* placeholder return */
    return 0;
}

int printReadyQueue(PROC *p)  // print list p
{
    PROC *q = p;
    // print the list as readyQueue=[pid pri]->..-> NULL
    printf("\nreadyQueue   =", ' ');
    for (; q;) {
        printf("[%d %d]->", q->pid, q->priority);
        q = q->next;
    }
    printf("NULL");
    /* placeholder return */
    return 0;
}
