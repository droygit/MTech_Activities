#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>

#define DEBUG_PRINT ;//printf("\n%s:%d", __func__, __LINE__)

#define MAX_STRING_LENGTH   1501
#define MAX_WAIT_TIME       5   // In minutes
#define BACKLOG 10
typedef struct sockaddr sock_addr;

#define ARGC_COUNT   4   // <app_name> -s -p <SELF_PORT_ID> OR <app_name> -c -p <SELF_PORT_ID>
enum process_catagory 
{
    SERVER = 0,
    CLIENT,
    INCORRCT_ARGC,
    UNKNOWN_CATAGORY
};

enum boolean
{
    TRUE = 0,
    FALSE = 1
};

// ---- TCP Server code ----


void server_main(int port_id)
{
    int    i, len, ret_val, on = 1;
    int    listenfd, maxfd, newfd;
    int    desc_ready, quit_server = FALSE;
    int    close_conn;
    char   buffer[MAX_STRING_LENGTH];
    struct sockaddr_in  addr;
    struct timeval      timeout;
    fd_set              master_set, working_set;

    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (listenfd < 0)
    {
        printf("Error! Socket creation failed\n");
        exit(-1);
    }

    ret_val = setsockopt(listenfd, SOL_SOCKET,  SO_REUSEADDR, (char *)&on, sizeof(on));
    if (ret_val < 0)
    {
        printf("Error! Function setsockopt() failed");
        close(listenfd);
        exit(-1);
    }

    ret_val = ioctl(listenfd, FIONBIO, (char *)&on);
    if (ret_val < 0)
    {
        printf("Error! ioctl() failed");
        close(listenfd);
        exit(-1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sin_family      = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port        = htons(port_id);
    
    // Bind the socket
    ret_val = bind(listenfd,(struct sockaddr *)&addr, sizeof(addr));
    if (ret_val < 0)
    {
        printf("Error! bind() failed");
        close(listenfd);
        exit(-1);
    }

    ret_val = listen(listenfd, BACKLOG);
    if (ret_val < 0)
    {
        printf("Error! listen() failed");
        close(listenfd);
        exit(-1);
    }

    // Initialize the FD_SET
    FD_ZERO(&master_set);
    maxfd = listenfd;
    FD_SET(listenfd, &master_set);

    // Initialize the timeval struct to a few minutes.
    // If no activity after a few minutes this program will end.
    timeout.tv_sec  = MAX_WAIT_TIME * 60;
    timeout.tv_usec = 0;

    // Waiting for incoming connects or for incoming data
    // on any of the connected sockets.
    do
    {
        memcpy(&working_set, &master_set, sizeof(master_set));

        printf("Waiting on select() for %d minutes.\n", MAX_WAIT_TIME);
        ret_val = select(maxfd + 1, &working_set, NULL, NULL, &timeout);

        // If select called failed, then break the loop and end the program.
        if (ret_val < 0)
        {
            printf("Error! Function select() failed");
            break;
        }

        // Check for timer expiration.
        if (ret_val == 0)
        {
            printf(" Function select() timed out.  End program.\n");
            break;
        }

        desc_ready = ret_val;
        for (i=0; i <= maxfd  &&  desc_ready > 0; ++i)
        {
            if (FD_ISSET(i, &working_set))
            {
                desc_ready -= 1;

                if (i == listenfd) // New connection
                {
                    printf("  Listening socket is readable\n");

                    do
                    {
                        newfd = accept(listenfd, NULL, NULL);
                        if (newfd < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                perror("Error! Function accept() failed. ");
                                quit_server = TRUE;
                            }
                            break;
                        }

                        printf("  New incoming connection - %d\n", newfd);
                        FD_SET(newfd, &master_set);
                        if (newfd > maxfd)
                            maxfd = newfd;

                    } while (newfd != -1);
                }
                else // Existing connection
                {
                    printf("  Descriptor %d is readable\n", i);
                    close_conn = FALSE;
                    while (1)
                    {
                        memset(buffer, 0, sizeof(buffer));
                        ret_val = recv(i, buffer, sizeof(buffer), 0);
                        if (ret_val < 0)
                        {
                            if (errno != EWOULDBLOCK)
                            {
                                printf("Error! Function recv() failed");
                                close_conn = TRUE;
                            }
                            break;
                        }

                        if (ret_val == 0)
                        {
                            printf("Connection closed\n");
                            close_conn = TRUE;
                            break;
                        }

                        // Data received
                        len = ret_val;
                        printf("  %d bytes received from client\n", len);
                        printf(" Data = %s\n", buffer);

                        // Echo the data back to the client
                        ret_val = send(i, buffer, len, 0);
                        if (ret_val < 0)
                        {
                            printf("Error! Function send() failed");
                            close_conn = TRUE;
                            break;
                        }

                    }

                    // If the close_conn flag is set to TRUE, we need
                    // to clean up this active connection.
                    if (close_conn == TRUE)
                    {
                        close(i);
                        FD_CLR(i, &master_set);
                        if (i == maxfd)
                        {
                            while (FD_ISSET(maxfd, &master_set) == FALSE)
                                maxfd -= 1;
                        }
                    }
                }
            }
        }

    } while (quit_server == FALSE);

    /*************************************************************/
    /* Clean up all of the sockets that are open                 */
    /*************************************************************/
    for (i=0; i <= maxfd; ++i)
    {
        if (FD_ISSET(i, &master_set))
            close(i);
    }
}

// -------------------------

// ---- TCP Client code ----
void client_func(int sockfd)
{
    char buff[MAX_STRING_LENGTH];
    int n;

    // Infinite loop
    while (1) {
        memset(buff, 0, sizeof(buff));
        printf("Enter the string (max %d bytes): ", MAX_STRING_LENGTH-1);
        n = 0;

        // newline is considered as end of sentense.
        while ((buff[n++] = getchar()) != '\n');
        
        if ((strncmp(buff, "exit", 4)) == TRUE)
        {
            printf("Client Exit...\n");
            break;
        }
        else
        {
            write(sockfd, buff, sizeof(buff));

            memset(buff, 0, sizeof(buff));
            read(sockfd, buff, sizeof(buff));

            printf("From Server : %s", buff);
        }
    }

    // Requesting Server to quit gracefully.
    strcpy(buff, "exit");
    write(sockfd, buff, sizeof(buff));
    memset(buff, 0, sizeof(buff));
    read(sockfd, buff, sizeof(buff));
    printf("From Server : %s", buff);
}

int client_main(int server_port_id)
{
    int sockfd, connfd;
    struct sockaddr_in srv_addr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Error! Socket creation failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");

    memset(&srv_addr, 0, sizeof(srv_addr));

    // assign IP, PORT
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    srv_addr.sin_port = htons(server_port_id);

    // connect the client socket to server socket
    if (connect(sockfd, (sock_addr*)&srv_addr, sizeof(srv_addr)) != 0) {
        printf("Connection with the server failed...\n");
        exit(0);
    }
    else
        printf("Connected to the server..\n");

    // function for chat
    client_func(sockfd);

    // close the socket
    close(sockfd);
}

// -------------------------

void usage ()
{
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("For starting a server - \n");
    printf("\t tcp_server_client -s -p <SELF_PORT_ID>\n");
    printf("For starting a client - \n");
    printf("\t tcp_server_client -c -p <SELF_PORT_ID>");
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
}

enum process_catagory extract_info(int argc, char *argv[], 
    int *out_server_port_id,
    int *out_client_port_id)
{
    enum process_catagory ret = UNKNOWN_CATAGORY;

    if (argc == ARGC_COUNT)
    {
        DEBUG_PRINT;
        printf (" %s %s %s", argv[1], argv[2], argv[3]);

        if (strcmp(argv[1], "-s") == TRUE &&
            strcmp(argv[2], "-p") == TRUE)
        {
            DEBUG_PRINT;
            *out_server_port_id = atoi(argv[3]);
            ret = SERVER;
        }
        else if (strcmp(argv[1], "-c") == TRUE &&
            strcmp(argv[2], "-p") == TRUE)
        {
            DEBUG_PRINT;
            *out_server_port_id = atoi(argv[3]);
            ret = CLIENT;
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
    int self_port_id, server_port_id, client_port_id;
    catagory = extract_info(argc, argv, &server_port_id, &client_port_id);
    switch (catagory)
    {
        case SERVER: 
            printf("\nThis process will be acting as TCP SERVER\n");
            server_main(server_port_id);
            break;
        case CLIENT:
            printf("\nThis process will be acting as TCP CLIENT\n");
            client_main(server_port_id);
            break;
        case INCORRCT_ARGC:
            printf("\nIncorrect command line argument! argc = %d", argc);
            usage();
        default:
            printf("\nError: Incorrect option(s)!");
            printf("Please try again. \nHow to use is given below.");
            usage();
    }
    printf("\nProgram ENDS\n");
}
