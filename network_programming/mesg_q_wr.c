// This source code will be responsible to run one application as reader from 
// message queue and write to same message queue.

#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEBUG_PRINT printf("\n%s:%d\n", __func__, __LINE__)
#define ARGC_COUNT  2   // <app_name> -r -n <number of message> OR <app_name> -w -n <number of message>
#define PATH_NAME   "ftok_file_1"
#define PROJ_ID     50
#define MAX_LENGTH_MSG  100
#define MAX_VALUE   100
#define SLEEP_TIME  1   // In seconds
#define NUM_READ_WRITE_OP   50

enum process_catagory 
{
    READER = 0,
    WRITER,
    INCORRCT_ARGC,
    UNKNOWN_CATAGORY
};

enum boolean
{
    TRUE = 0,
    FALSE = 1
};

struct msgbuf {
    long mtype;                     /* message type, must be > 0 */
    char mtext[MAX_LENGTH_MSG];     /* message data */
};

void read_process (int num_msg)
{

    key_t key_value;
    int msg_id;
    int text;
    struct msgbuf message;

    key_value = ftok(PATH_NAME, PROJ_ID);
    msg_id = msgget(key_value, 0666 | IPC_CREAT);
    message.mtype = 1;

    while (num_msg >= 0)
    {
        memset(message.mtext, 0, MAX_LENGTH_MSG*sizeof(char));
        // msgrcv to receive message
        msgrcv(msg_id, &message, sizeof(message), 1, 0);

        // display the message
        printf("Data Received is : %s \n", message.mtext);

        sleep (SLEEP_TIME);

        --num_msg;
    }

    sleep (10*SLEEP_TIME);
    // delete the message queue
     msgctl(msg_id, IPC_RMID, NULL);

}

void write_process(int num_msg)
{
    key_t key_value;
    int msg_id;
    int text;
    struct msgbuf message;

    // The ftok() function shall return the same key value
    // for all paths that name the same file, when called
    // with the same id value
    key_value = ftok(PATH_NAME, PROJ_ID);

    // msgget creates a message queue
    // and returns identifier
    msg_id = msgget(key_value, 0666 | IPC_CREAT);
    message.mtype = 1;

    srand(time(0));

    while (num_msg >= 0)
    {
        text = rand() % MAX_VALUE;
        text *= text;
        memset(message.mtext, 0, MAX_LENGTH_MSG*sizeof(char));
        sprintf(message.mtext, "%d", text);

        // The msgsnd() system call appends a copy of the message
        // pointed to by msgp to the message queue whose identifier
        // is specified by msqid.
        msgsnd(msg_id, &message, sizeof(message), 0);

        // Show the message
        printf("Data send : %s \n", message.mtext);

        sleep(SLEEP_TIME);

        --num_msg;
    }
}

void usage ()
{
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("For starting a message queue reader - \n");
    printf("\t mesg_q_wc -r -n <number of message>\n");
    printf("For starting a message queue writer - \n");
    printf("\t mesg_q_wc -w -n <number or message>\n");
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

enum process_catagory extract_info(int argc, char *argv[])
{
    enum process_catagory ret = UNKNOWN_CATAGORY;

    if (argc == ARGC_COUNT)
    {
        DEBUG_PRINT;
        //printf (" %s %s %s", argv[1], argv[2], argv[3]);

        if (strcmp(argv[1], "-w") == TRUE)
        {
            DEBUG_PRINT;
            ret = WRITER;
        }
        else if (strcmp(argv[1], "-r") == TRUE)
        {
            DEBUG_PRINT;
            ret = READER;
        }
        else 
            ret = INCORRCT_ARGC;
    }
    else 
    {
        DEBUG_PRINT;
        ret = UNKNOWN_CATAGORY;
    }

    return ret;
}

int main (int argc, char *argv[])
{
    enum process_catagory catagory;
    int num_msg = NUM_READ_WRITE_OP;
    catagory = extract_info(argc, argv);
    switch (catagory)
    {
        case READER: 
            printf("\nThis process will be acting as Message Reader\n");
            read_process(num_msg);
            break;
        case WRITER:
            printf("\nThis process will be acting as Message Writer\n");
            write_process(num_msg);
            break;
        case INCORRCT_ARGC:
            printf("\nIncorrect command line argument! argc = %d", argc);
            usage();
            break;
        default:
            printf("\nError: Incorrect option(s)!");
            printf("Please try again. \nHow to use is given below.");
            usage();
    }
    printf("\nProgram ENDS\n");
}
