#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/sem.h>
#include "a2_helper.h"

pthread_mutex_t lock;

pthread_mutex_t lock2;

int t4Ok;
int sem_id;

pthread_cond_t cond, cond2;

void P(int semId, int semNr)
{
    struct sembuf op = {semNr, -1, 0};

    semop(semId, &op, 1);
}

void V(int semId, int semNr)
{
    struct sembuf op = {semNr, +1, 0};

    semop(semId, &op, 1);
}

void* sync_threads(void *arg)
{
    int thID = *((int*)arg);
    int ok = 0;

    pthread_mutex_lock(&lock);//The operation which ensures the mutual exclusion access to a shared resource using a mutex
    // is called locking the mutex. This operation terminates successfully for a single thread at a given moment.
    // The other threads are put in a waiting state in a queue associated to the mutex;
    // these threads stay there until the mutex is unlocked by the thread which locked it.

    if (thID == 1)
    {
        ok = t4Ok;
        pthread_mutex_unlock(&lock);//Unlocking the mutex is the operation opposed to the locking operation.
        // These two operations are always used together to ensure that no threads wait forever for a certain mutex
        while (ok == 0)
        {
            pthread_mutex_lock(&lock);
            ok = t4Ok;
            pthread_mutex_unlock(&lock);
        }
        pthread_mutex_lock(&lock);
    }

    if (thID == 4)
    {
        t4Ok = 1;
    }

    if (thID == 3)
    {
        P(sem_id, 0);
    }
    info(BEGIN, 7, thID);
    if (thID == 4)
    {
        pthread_cond_wait(&cond2, &lock);//A thread enters a waiting state after fulfilling
        // a condition only by calling one of the functions: pthread_cond_wait.
        //The thread exits the waiting state when the function pthread_cond_signal is called by another thread,
        // which notices the fulfillment of the condition
    }
    if (thID == 1)
        pthread_cond_signal(&cond2);
    info(END, 7, thID);
    if (thID == 3)
    {
        V(sem_id, 1);
    }

    pthread_mutex_unlock(&lock);
    return NULL;
}

void* thread_barrier(void *arg)
{
    int thID = *((int*)arg);
    info(BEGIN, 4, thID);
    info(END, 4, thID);
    return NULL;
}

void* threads_diff_process(void *arg)
{
    int thID = *((int*)arg);

    if (thID == 3)
    {
        P(sem_id, 1);
    }
    info(BEGIN, 2, thID);

    info(END, 2,thID);
    if (thID == 6)
    {
        V(sem_id, 0);
    }

    return NULL;
}

int main(int argc, char **argv){

    init();

    int pid2, pid3, pid4, pid5, pid6, pid7, pid8;
    pthread_t th[50];

    int thReplace[50];
    
    info(BEGIN, 1, 0);

    sem_id = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
    semctl(sem_id, 0, SETVAL, 0);
    semctl(sem_id, 1, SETVAL, 0);
    pid2 = fork();

    if (pid2 == 0)
    {
        info(BEGIN, 2, 0);

        pid3 = fork();
        if(pid3 == 0)
        {
            info(BEGIN, 3, 0);

            pid6 = fork();
            if(pid6 == 0)
            {
                info(BEGIN, 6, 0);
                info(END, 6, 0);
            }
            else
            {
                waitpid(pid6, NULL, 0);
                info(END, 3, 0);
            }
        }
        else
        {
            pid4 = fork();
            if (pid4 == 0)
            {
                info(BEGIN, 4, 0);

                pthread_mutex_init(&lock2, NULL);

                for (int i = 1; i <= 36; i++)
                {
                    thReplace[i] = i;
                    if (pthread_create(&th[i], NULL, thread_barrier, &thReplace[i]) != 0 ) {
                        perror("cannot create threads");
                        exit(1);
                    }
                }
                for(int i = 1; i <= 36; i++)
                {
                    pthread_join(th[i], NULL);
                }

                pthread_mutex_destroy(&lock2);

                info(END, 4, 0);
            }
            else
            {
                pid5 = fork();

                if(pid5 == 0)
                {
                    info(BEGIN, 5, 0);

                    pid8 = fork();
                    if(pid8 == 0)
                    {
                        info(BEGIN, 8, 0);
                        info(END, 8, 0);
                    }
                    else
                    {
                        waitpid(pid8, NULL, 0);
                        info(END, 5, 0);
                    }
                }
                else
                {
                    for (int i = 1; i <= 6; i++)
                    {
                        thReplace[i] = i;
                        if (pthread_create(&th[i], NULL, threads_diff_process, &thReplace[i]) != 0 ) {
                            perror("cannot create threads");
                            exit(1);
                        }
                    }
                    for(int i = 1; i <= 6; i++)
                    {
                        pthread_join(th[i], NULL);
                    }

                    waitpid(pid3, NULL, 0);
                    waitpid(pid4, NULL, 0);
                    waitpid(pid5, NULL, 0);
                    info(END, 2, 0);
                }
            }
        }
    }
    else
    {
        pid7 = fork();
        if (pid7 == 0)
        {
            info(BEGIN, 7, 0);

            pthread_mutex_init(&lock,NULL);
            //In order to create a structure for storing the attributes of a mutex, we declare a variable of type pthread_mutexattr_t.
            // Before the variable can be used, it has to be initialized;
            pthread_cond_init(&cond,NULL);
            pthread_cond_init(&cond2,NULL);

            for (int i = 1; i <= 4; i++)
            {
                thReplace[i] = i;
                if (pthread_create(&th[i], NULL, sync_threads, &thReplace[i]) != 0 ) {
                    perror("cannot create threads");
                    exit(1);
                }
            }
            for(int i = 1; i <= 4; i++)
            {
                pthread_join(th[i], NULL);
            }

            pthread_mutex_destroy(&lock);
            pthread_cond_destroy(&cond);
            pthread_cond_destroy(&cond2);

            info(END, 7, 0);
        }
        else
        {
            waitpid(pid2, NULL, 0);
            waitpid(pid7, NULL, 0);
            semctl(sem_id, 0, IPC_RMID, 0);
            info(END, 1, 0);
        }
    }
    return 0;
}
