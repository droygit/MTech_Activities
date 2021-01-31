#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_LOOP 100
#define SHM_KEY 0x12345

void ipc_read()
{
    int shmid, status;
    int *read_int;
    int i;

    shmid = shmget(SHM_KEY, sizeof(int), 0777|IPC_CREAT);

    read_int = (int *) shmat(shmid, 0, 0);

    for( i=0; i < MAX_LOOP; i++) {
        sleep(1);
        printf("\t\t\tIPC Reads: %d\n",*read_int);
    }

    shmdt(read_int);
}

void ipc_write()
{
    int shmid, status;
    int *write_int;
    int i;
    time_t t;

    //shmid = shmget(IPC_PRIVATE, sizeof(int), 0777|IPC_CREAT);
    shmid = shmget(SHM_KEY, sizeof(int), 0777|IPC_CREAT);

    write_int = (int *) shmat(shmid, 0, 0);
    srand((unsigned) time(&t));
    
    for( i=0; i < MAX_LOOP; i++) {
        sleep(1);
        *write_int = rand()%100;
        printf("IPC write: %d\n", *write_int);
    }

    shmdt(write_int);
    shmctl(shmid, IPC_RMID, 0);
}

void usage()
{
        printf ("\nUsage:");
        printf ("\n\tipc r = it means this program will read");
        printf ("\n\tipc w = it means this program will write");
}

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        usage();
    }
    else
    {
        if (argv[1][0] == 'r') ipc_read();
        else if (argv[1][0] == 'w') ipc_write();
        else usage(); 
    }

    return 0;
}
