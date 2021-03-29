// This source code will be responsible to run one application as UDP Server
// or UDP Client. In this code, whatever string UDP Client will send to UDP Server, Server
// will return same string to UDP Client.
// Assumption:
//      1. There will be one client and one server.
//      2. Message length will be maximum 1500 bytes.

#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define DEBUG_PRINT //printf("\n%s:%d\n", __func__, __LINE__)

#define MAX_STRING_LENGTH 1501
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

// ---- UDP Server code ----
  
enum boolean server_main(int port_id)
{
    int sockfd, connfd, len, n = 0;
    struct sockaddr_in srv_addr, cli;
    enum boolean is_quit_server = FALSE;
    char buff[MAX_STRING_LENGTH];
  
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Error! Socket creation failed.\n");
        return FALSE;
    }
    else
        printf("Socket successfully created.\n");

    memset(&srv_addr, 0, sizeof(srv_addr));
    memset(&cli, 0, sizeof(cli));
    len = sizeof(cli);
  
    // assign IP, PORT
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // The socket will be bound to all locali interfaces
    srv_addr.sin_port = htons(port_id);             // Server port id, given by user
  
    // Binding newly created socket to given IP address and verification
    if ((bind(sockfd, (sock_addr *)&srv_addr, sizeof(srv_addr))) != 0) {
        printf("Error! Socket binding operation failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
  
    while (is_quit_server == FALSE)
    {
        memset(buff, 0, sizeof(buff));
        n = 0;
        // Now server is ready to receive
        n = recvfrom(sockfd, buff, MAX_STRING_LENGTH, 
                MSG_WAITALL, (sock_addr *) &cli, &len);

        buff[n] = '\0';
        printf("Client sends : %s\n", buff);
       
        DEBUG_PRINT;
        // if msg contains "Quit" then server exit and echo message ended.
        if (strncmp("exit", buff, 4) == TRUE) {
            strcpy(buff, "Server quits! Bye.");
            is_quit_server = TRUE;
        }
        DEBUG_PRINT;

        sendto(sockfd, buff, strlen(buff), 
                MSG_CONFIRM, (sock_addr *) &cli,
                sizeof(cli));
        DEBUG_PRINT;
    }

    // After chatting close the socket
    close(sockfd);
    printf("Server Successfully closed!\n");

    return TRUE;
}

// -------------------------

int client_main(int server_port_id)
{
    int sockfd, connfd, len, n = 0;
    struct sockaddr_in srv_addr, cli;
    enum boolean is_quit_client = FALSE;
    char buff[MAX_STRING_LENGTH];

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        printf("Error! Socket creation failed.\n");
        exit(0);
    }
    else
        printf("UDP Socket successfully created..\n");

    memset(&srv_addr, 0, sizeof(srv_addr));
    memset(&cli, 0, sizeof(cli));
    len = sizeof(cli);

    // assign IP, PORT
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    srv_addr.sin_port = htons(server_port_id);

    while (is_quit_client == FALSE)
    {
        n = 0;
        memset(buff, 0, sizeof(buff));
        printf("Enter the string (max %d bytes): ", MAX_STRING_LENGTH-1);
        
        // newline is considered as end of sentense.
        while ((buff[n++] = getchar()) != '\n');
        buff[n-1] = '\0';
        printf("Client Message : %s\n", buff);
        
        if ((strncmp(buff, "exit", 4)) == TRUE)
        {
            printf("UDP Client Exit.\n");
            is_quit_client = TRUE;
            
            // Requesting Server to quit gracefully.
            strcpy(buff, "exit");
        }
        
        sendto(sockfd, buff, strlen(buff),
                MSG_CONFIRM, (sock_addr *) &srv_addr, 
                sizeof(srv_addr));
        printf("UDP message sent.\n");
        DEBUG_PRINT;

        n = recvfrom(sockfd, (char *)buff, MAX_STRING_LENGTH, 
                MSG_WAITALL, (sock_addr *) &srv_addr, &len);
        DEBUG_PRINT;
        
        buff[n] = '\0';
        printf("Server Reply : %s\n", buff);
        DEBUG_PRINT;
    }

    // close the socket
    close(sockfd);
}

// -------------------------

void usage ()
{
    printf("\n+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");
    printf("For starting a server - \n");
    printf("\t udp_server_client -s -p <SELF_PORT_ID>\n");
    printf("For starting a client - \n");
    printf("\t udp_server_client -c -p <SELF_PORT_ID>");
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
            printf("\nThis process will be acting as UDP SERVER\n");
            server_main(server_port_id);
            break;
        case CLIENT:
            printf("\nThis process will be acting as UDP CLIENT\n");
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
