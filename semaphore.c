#include "semaphore.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <stdlib.h>

struct Semaphore* Semaphore_init(int n)
{
    struct Semaphore* self = (struct Semaphore*)malloc(sizeof(struct Semaphore));
    self->semId = semget(IPC_PRIVATE, 1, 0600|IPC_CREAT);
    
    struct sembuf op;
    op.sem_op = n;
    op.sem_flg = 0;
    op.sem_num = 0;
    semop(self->semId, &op, 1);
    
    return self;
}
void Semaphore_free(struct Semaphore* self)
{
    semctl(self->semId, 0, IPC_RMID, 0); 
    free(self);
}

void Semaphore_grab(struct Semaphore* self)
{
    struct sembuf op;
    op.sem_op = -1;
    op.sem_flg = 0;
    op.sem_num = 0;
    semop(self->semId, &op, 1);
}
void Semaphore_release(struct Semaphore* self)
{
    struct sembuf op;
    op.sem_op = 1;
    op.sem_flg = 0;
    op.sem_num = 0;
    semop(self->semId, &op, 1);
}
