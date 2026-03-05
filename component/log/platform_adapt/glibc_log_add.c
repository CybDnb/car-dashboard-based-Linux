#include <pthread.h>
#include "log.h"


static pthread_mutex_t log_mutex = PTHREAD_MUTEX_INITIALIZER;

void log_lock(bool lock, void *udata)
{
    pthread_mutex_t *mutex = (pthread_mutex_t *)udata;

    if (lock)
        pthread_mutex_lock(mutex);
    else
        pthread_mutex_unlock(mutex);
}
