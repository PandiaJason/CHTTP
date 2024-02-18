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
#include <sys/socket.h>
#include <unistd.h>
#include <signal.h>
#include <netdb.h>

#define PORT 8080
#define MAX_CONNECTIONS 10
#define BUFFER_SIZE 1024

volatile sig_atomic_t shutdown_requested = 0;

void handle_shutdown(int signum) {
    shutdown_requested = 1;
}

void handle_client(int client_socket);

void get_server_ip();

int main(void) 
{
    /* Register a signal handler for graceful shutdown */
    signal(SIGINT, handle_shutdown);

    /* Create a socket for the server */
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket (sys/socket.h)");
        return EXIT_FAILURE;
    }

    printf("Socket created successfully as %d\n", sockfd);

    /* Set up the server address structure */
    struct sockaddr_in host_addr = {0};
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    /* Bind the socket to a specific address and port */
    if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(host_addr)) != 0) {
        perror("Error binding socket (sys/socket.h)");
        close(sockfd);
        return EXIT_FAILURE;
    }

    /* Start listening for incoming connections */
    if (listen(sockfd, MAX_CONNECTIONS) != 0) {
        perror("Error listening (sys/socket.h)");
        close(sockfd);
        return EXIT_FAILURE;
    }

    /* Get and print the server's IP address */
    get_server_ip();

    printf("Server listening on port %d\n", PORT);

    while (!shutdown_requested) {
        /* Accept a connection from a client */
        struct sockaddr_in client_addr = {0};
        socklen_t client_addrlen = sizeof(client_addr);

        int client_socket = accept(sockfd, (struct sockaddr *)&client_addr, &client_addrlen);

        if (client_socket == -1) {
            perror("Error accepting connection (sys/socket.h)");
            continue;
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        /* Create a child process to handle the client */
        if (fork() == 0) {
            close(sockfd); /* Close the listening socket in the child process */
            handle_client(client_socket);
            close(client_socket);
            _exit(EXIT_SUCCESS);
        } else {
            close(client_socket); /* Close the client socket in the parent process */
        }
    }

    /* Gracefully close the server */
    close(sockfd);
    printf("Server shutting down gracefully.\n");

    return EXIT_SUCCESS;
}

/* Function to handle communication with a connected client */
void handle_client(int client_socket) 
{
    char buffer[BUFFER_SIZE];
    ssize_t received_bytes;

    /* Receive data from the client */
    received_bytes = recv(client_socket, buffer, sizeof(buffer) - 1, 0);
    if (received_bytes <= 0) {
        perror("Error receiving data (sys/socket.h)");
        return;
    }

    buffer[received_bytes] = '\0'; /* Null-terminate the received data */

    /* Assuming a simple response for demonstration purposes */
    const char *response = "HTTP/1.1 200 OK\r\nContent-Length: 12\r\n\r\nHello, World!\n";

    /* Send the response back to the client */
    send(client_socket, response, strlen(response), 0);
}

/* Function to get and print the server's IP address */
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
