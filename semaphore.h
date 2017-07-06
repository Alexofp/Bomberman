#ifndef H_SEMAPHORE
#define H_SEMAPHORE

struct Semaphore
{
    int semId;
};

struct Semaphore* Semaphore_init(int n);
void Semaphore_free(struct Semaphore* self);

void Semaphore_grab(struct Semaphore* self);
void Semaphore_release(struct Semaphore* self);

#endif
