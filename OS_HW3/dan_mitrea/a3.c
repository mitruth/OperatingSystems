#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/shm.h>
#include <sys/mman.h>

#define SHM_SIZE 3612184

int main(int argc, char **argv) {

    int fd1 = 0, fd2 = 0, fileNameFD = 0;
    int connectSize = 7, pongSize = 4, pingSize = 4, errorSize = 5, successSize = 7, file_name_size = 0;
    int file_size;
    short requestSize = 0;
    int shmId = 0, *pInt;
    char *data, *mmapResult;
    unsigned int  variant = 55197, create_shm_number = 0;
    char request[1000], file_name[1000];

    if ((mkfifo("RESP_PIPE_55197", 0644)) != 0)
    {
        perror("Cannot create the response pipe");
        exit(1);
    }

    fd2 = open("REQ_PIPE_55197", O_RDONLY);
    if (fd2 < 0)
    {
        perror("cannot open the request pipe");
        exit(3);
    }

    fd1 = open("RESP_PIPE_55197", O_WRONLY);

    write(fd1, &connectSize, 1);
    write(fd1, "CONNECT", connectSize);
    printf("SUCCESS\n");

    while(1)
    {
        requestSize = 0;
        read(fd2, &requestSize, 1);
        read(fd2, request, requestSize);
        request[requestSize] = '\0';
        printf("request size : %d \n", requestSize);
        printf("request  : %s \n", request);

        if (strstr(request, "PING") != NULL)
        {
            write(fd1, &pingSize, 1);
            write(fd1, "PING", 4);
            write(fd1, &pongSize, 1);
            write(fd1, "PONG", 4);
            write(fd1, &variant, sizeof(unsigned int));
        }
        else if (strstr(request, "CREATE_SHM") != NULL) {
            read(fd2, &create_shm_number, sizeof(unsigned int));
            shmId = shmget(11903, create_shm_number, 0664 | IPC_CREAT); //create shared memory area
            printf("shmId: %d\n", shmId);
            printf("create_shm_number: %u\n", create_shm_number);
            if (shmId < 0) {
                write(fd1, &requestSize, 1);
                write(fd1, request, requestSize);
                write(fd1, &errorSize, 1);
                write(fd1, "ERROR", errorSize);
            } else {
                pInt = (int *) shmat(shmId, 0, 0);
                //attach the shared memory area and acccess it as int
                printf("pInt: %d\n", *pInt);
                if (*pInt == -1) {
                    write(fd1, &requestSize, 1);
                    write(fd1, request, requestSize);
                    write(fd1, &errorSize, 1);
                    write(fd1, "ERROR", errorSize);
                }
                write(fd1, &requestSize, 1);
                write(fd1, request, requestSize);
                write(fd1, &successSize, 1);
                write(fd1, "SUCCESS", successSize);
            }
        }
        else if (strstr(request, "WRITE_TO_SHM") != NULL)
        {
            int offset = 0;
            int value = 0;
            read(fd2, &offset, sizeof(unsigned int));
            read(fd2, &value, sizeof(unsigned int));
            printf("offset: %d\n", offset);
            printf("value: %d\n", value);
            if (offset > SHM_SIZE)
            {
                write(fd1, &requestSize, 1);
                write(fd1, request, requestSize);
                write(fd1, &errorSize, 1);
                write(fd1, "ERROR", errorSize);
                exit(5);
            }
            if (offset + sizeof(unsigned int) > SHM_SIZE)
            {
                write(fd1, &requestSize, 1);
                write(fd1, request, requestSize);
                write(fd1, &errorSize, 1);
                write(fd1, "ERROR", errorSize);
                exit(5);
            }
            shmId = shmget(11903, SHM_SIZE, 0664 | IPC_CREAT); //create shared memory area
            printf("shmId: %d\n", shmId);
            if (shmId < 0)
            {
                write(fd1, &requestSize, 1);
                write(fd1, request, requestSize);
                write(fd1, &errorSize, 1);
                write(fd1, "ERROR", errorSize);
                exit(1);
            }
            pInt = (int *)shmat(shmId, 0, 0); //attach the shared memory area and acccess it as int
            printf("pInt: %d\n", *pInt);
            if (*pInt == -1)
            {
                write(fd1, &requestSize, 1);
                write(fd1, request, requestSize);
                write(fd1, &errorSize, 1);
                write(fd1, "ERROR", errorSize);
                exit(1);
            }
            data = (char*)pInt;
            memcpy(data + offset, &value, sizeof(unsigned int));
            write(fd1, &requestSize, 1);
            write(fd1, request, requestSize);
            write(fd1, &successSize, 1);
            write(fd1, "SUCCESS", successSize);
        }
        else if (strstr(request, "MAP_FILE") != NULL)
        {
            read(fd2, &file_name_size, 1);
            read(fd2, file_name, file_name_size);
            fileNameFD = open(file_name, O_RDONLY);
            if (fileNameFD < 0)
            {
                write(fd1, &requestSize, 1);
                write(fd1, request, requestSize);
                write(fd1, &errorSize, 1);
                write(fd1, "ERROR", errorSize);
                exit(5);
            }
            struct stat fileStat;
            fstat(fileNameFD, &fileStat);
            file_size = fileStat.st_size;
            mmapResult = (char*)mmap(0, file_size, PROT_READ, MAP_PRIVATE, fileNameFD, 0);
            if (mmapResult == MAP_FAILED)
            {
                write(fd1, &requestSize, 1);
                write(fd1, request, requestSize);
                write(fd1, &errorSize, 1);
                write(fd1, "ERROR", errorSize);
                exit(5);
            }
            write(fd1, &requestSize, 1);
            write(fd1, request, requestSize);
            write(fd1, &successSize, 1);
            write(fd1, "SUCCESS", successSize);
        }
        else if (strstr(request, "READ_FROM_FILE_OFFSET") != NULL)
        {
            unsigned int offset, nrBytes;
            read(fd2, &offset, sizeof(unsigned int));
            read(fd2, &nrBytes, sizeof(unsigned int));
            if (offset + nrBytes > file_size)
            {
                write(fd1, &requestSize, 1);
                write(fd1, request, requestSize);
                write(fd1, &errorSize, 1);
                write(fd1, "ERROR", errorSize);
                exit(5);
            }
        }
        else if (strstr(request, "READ_FROM_FILE_SECTION") != NULL)
        {
            break;
        }
        else if (strstr(request, "EXIT") != NULL)
        {
            break;
        }
        else
        {
            break;
        }
    }
    unlink("RESP_PIPE_55197");
    shmdt(pInt); //detach the area
    shmctl(shmId, IPC_RMID, 0);
    close(fd1);
    close(fd2);
    return 0;
}
