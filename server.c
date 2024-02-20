/*  Simple Server in C
    Author: Jason Pandain
    Date: 17-02-2024

    Description: This server program follows a basic sequence of steps for establishing a network connection.
    The process includes the creation of a socket, binding it to a specific address and port, and 
    listening for incoming connections. Upon accepting a connection, the server forks a child process 
    to handle the communication with the connected client. The communication is simple, responding with 
    a predefined message. Finally, the server gracefully shuts down when a termination signal (Ctrl+C) is received.
    The overall structure can be summarized as follows:

    socket -> bind -> listen -> accept -> fork -> handle_client -> close sockets

    Key Steps:
    1. Create a socket using socket()
    2. Bind the socket to a specific address and port using bind()
    3. Start listening for incoming connections with listen()
    4. Accept incoming connections using accept()
    5. Fork a child process to handle the client communication
    6. Handle the client communication in the child process
    7. Close the sockets in both parent and child processes

    Note: This is a simplified illustration for educational purposes and may lack error handling for brevity.
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>

#define PORT 8080
#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024

volatile sig_atomic_t shutdown_requested = 0;

void handle_shutdown(int signum) 
{
    shutdown_requested = 1;
}

int handle_client(int client_socket);

void get_server_ip(void);

int main(void)
{
    /* Register a singnal handler for the graceful shutdown */
    signal(SIGINT, handle_shutdown);

    /* Create a socket file discripter for the server */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating a socket file discripter with (sys/system.h)");
        return EXIT_FAILURE;
    }

    printf("Socket created successfully as %d\n", sockfd);
    
    /* Set up the server address from the sockaddr_in structure */
    struct sockaddr_in host_addr = {0};
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY); /* Default in address of the host */

    /* Bind the socket to a specific address and port, using if(bind and check) */
    if (bind(sockfd, ((struct sockaddr *) &host_addr), sizeof(host_addr)) != 0) {
        perror("Error binding socket with the host using (sys/socket.h)");
        close(sockfd);
        return EXIT_FAILURE;
    }

    /* Make the server to listening for for incomming connetions */
    if (listen(sockfd, MAX_CONNECTIONS) != 0) {
        perror("Error in listening to the incomming connections");
        close(sockfd);
        return EXIT_FAILURE;
    }

    /* If the struct of the host is done correcly it will be printed,
    it is not much upto the bind or listen functions*/
    printf("server listening on the port %d\n", ntohs(host_addr.sin_port));

    get_server_ip();

    while(!shutdown_requested) {
        /* Create a client address from the sockaddr_in structure */
        struct sockaddr_in client_addr = {0};
        socklen_t client_addrlen = sizeof(client_addr);

        int client_socket = accept(sockfd, ((struct sockaddr *) &client_addr), &client_addrlen);

        if (client_socket == -1) {
            perror("Error accepting connection in the server");
            continue;
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /* Create a child process to handle each client call. */
        if (fork() == 0) {
            close(sockfd);
            handle_client(client_socket);
            close(client_socket);
            _exit(EXIT_SUCCESS);
        } else {
            close(client_socket);
        }

    }

    close(sockfd);

    printf("Server shitting down gravefully.\n");

    return 0;
}


/* Internal FUnction to handle client comminication inside the server */
int handle_client(int client_socket)
{
    char buffer[BUFFER_SIZE];

    /* reading or writing the data, return every bits(bytes) processed  */
    ssize_t received_bytes;

    /* Receive data from the client */
    received_bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (received_bytes <= 0) {
        perror("Error receiving data (sys/socket.h)");
        return EXIT_FAILURE;
    }

    buffer[received_bytes] = '\0';

    printf("Received from the client: %s\n", buffer);

    /* Assuming a simple response from the server */
    const char *response = "Hello, Client!\n";

    /* Send this response to the client */
    send(client_socket, response, strlen(response), 0);

    return 0;

}

/* Function to get and print the (host)server's IP address */
void get_server_ip(void)
{
    char hostname[256];
    struct hostent *host_info;

    if (gethostname(hostname, sizeof(hostname)) == 0) {
        if ((host_info = gethostbyname(hostname)) != NULL) {
            struct in_addr address;
            memcpy(&address, host_info->h_addr_list[0], sizeof(struct in_addr));
            printf("Host Info Address: %p\n", host_info);
            printf("Server IP Address: %s\n", inet_ntoa(address));
        }
    }
}
