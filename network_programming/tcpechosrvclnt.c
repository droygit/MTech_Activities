#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>

#define DEBUG_PRINT ;//printf("\n%s:%d", __func__, __LINE__)

#define MAX_STRING_LENGTH 1500
#define PORT 8080
typedef struct sockaddr sock_addr;

#define SERVER_ARGC_COUNT   4   // <app_name> -s -p <SELF_PORT_ID> 
#define CLIENT_ARGC_COUNT   6   // <app_name> -c -p <SELF_PORT_ID> -t <SELF_PORT_ID>
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
  
// Function designed for echo message between server and client.
void server_echo_msg_func(int sockfd)
{
    char buff[MAX_STRING_LENGTH];
    int n;

    // infinite loop for echo message
    while (1) {
        // Reset memory
        memset(buff, 0, MAX_STRING_LENGTH);
  
        // read the message from client and copy it in buffer
        read(sockfd, buff, sizeof(buff));
        
        // print buffer which contains the client contents
        printf("From client: [%s]. Return same message to client.\n", buff);
 
        // Return same message to client
        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));
  
        // if msg contains "Quit" then server exit and echo message ended.
        if (strncmp("quit", buff, 4) == TRUE) {
            printf("Server Quits.\n");
            break;
        }
    }
}
  
enum boolean server_main(int port_id)
{
    int sockfd, connfd, len;
    struct sockaddr_in srv_addr, cli;
  
    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("Error! Socket creation failed.\n");
        return FALSE;
    }
    else
        printf("Socket successfully created.\n");

    memset(&srv_addr, 0, sizeof(srv_addr));
  
    // assign IP, PORT
    srv_addr.sin_family = AF_INET;
    srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);   // The socket will be bound to all locali interfaces
    srv_addr.sin_port = htons(port_id);             // Server port id, given by user
  
    // Binding newly created socket to given IP address and verification
    if ((bind(sockfd, (sock_addr*)&srv_addr, sizeof(srv_addr))) != 0) {
        printf("Error! Socket binding operation failed.\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");
  
    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Error! Listen failed.\n");
        exit(0);
    }
    else
        printf("Server started listening.\n");

    len = sizeof(cli);
  
    // Accept the data packet from client and verification
    connfd = accept(sockfd, (sock_addr*)&cli, &len);
    if (connfd < 0) {
        printf("Error! Server acccept failed.\n");
        exit(0);
    }
    else
        printf("server acccept the client...\n");
  
    // Function for echoing same message between client and server
    server_echo_msg_func(connfd);
  
    // After chatting close the socket
    close(sockfd);
    printf("Server Successfully closed!\n");

    return TRUE;
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
        
        write(sockfd, buff, sizeof(buff));
        
        memset(buff, 0, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        
        printf("From Server : %s", buff);
        
        if ((strncmp(buff, "exit", 4)) == TRUE) {
            printf("Client Exit...\n");
            break;
        }
    }
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

    if (argc == 4)
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
