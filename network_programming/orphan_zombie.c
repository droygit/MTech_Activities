#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <unistd.h>

#define WAIT    20

void orphan()
{
    int pid ; 
    
    printf("\nPID of original process = %d and PPID = %d.\n",getpid(),getppid()); 
    pid=fork(); 

    if ( pid == -1 )
    {
        perror("fork function failed");
        return;
    }

    if(pid!=0 )
    { 
        sleep(WAIT/2); 
        printf("\nParent PID = %d and PPID = %d.\n",getpid(),getppid()); 
        printf("\nChild's PID = %d\n",pid) ; 
    }
    else 
    { 
        sleep(WAIT); 
        printf("\nChild's PID = %d and PPID = %d.\n",getpid(),getppid()); 
    }

    printf ("\nPID %d terminates.\n", getpid()) ;
}

void zombie()
{
    int status;
    int pid ; 
    
    printf("\nPID of original process = %d and PPID = %d.\n",getpid(),getppid()); 
    pid=fork();

    if ( pid == -1 )
    {
        perror("fork function failed");
        return;
    }

    if(pid!=0 )
    { 
        printf("\nParent PID = %d and PPID = %d.\n",getpid(),getppid()); 
        printf("\nChild's PID = %d\n",pid);
        sleep(WAIT); 
    }
    else 
    { 
        printf("\nChild's PID = %d and PPID = %d.\n",getpid(),getppid());
        sleep(WAIT/2); 
        printf("\nTerminating child process PID %d terminates.\n", getpid()) ;
        exit(777);
    }

    if ( waitpid(pid, &status, 0) == -1 ) {
        perror("waitpid failed");
        return;
    }

    if ( WIFEXITED(status) ) {
        const int es = WEXITSTATUS(status);
        printf("exit status was %d\n", es);
    }
}

void usage()
{
    printf ("\nUsage:");
    printf ("\n\tprocess o = it means this program will generate orphan process");
    printf ("\n\tprocess z = it means this program will generate zombie process");
}

int main (int argc, char *argv[])
{
    if (argc != 2)
    {
        usage();
    }
    else
    {
        if (argv[1][0] == 'o') orphan();
        else if (argv[1][0] == 'z') zombie();
        else usage(); 
    }

    return 0;
}
