/* Simple Client in C
   Author: Jason Pandain
   Date: 17-02-2024

   Description: This client program establishes a connection with a server following a basic sequence of steps.
   The process includes creating a socket, configuring the server address structure, and connecting to the server.
   Once connected, the client sends a message to the server and receives a response.
   The overall structure can be summarized as follows:

   socket -> configure server address -> connect -> send data -> receive data -> close socket

   Key Steps:
   1. Create a socket using socket()
   2. Configure the server address structure (IP and port)
   3. Connect to the server using connect()
   4. Send data to the server using send()
   5. Receive data from the server using recv()
   6. Close the socket after communication

   Note: This is a simplified illustration for educational purposes and may lack error handling for brevity.
*/

#include <stdio.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

int main(void) 
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        perror("Error creating socket");
        return EXIT_FAILURE;
    }

    printf("Socket created successfully as %d\n", sockfd);

    /* Set up the server address structure */
    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    /*  inet_pton is a function in C used for converting an IPv4 or IPv6 address
        from presentation format (string) to network format (binary). 
        The name stands for "presentation to network." 
        */
    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address or address not supported");
        close(sockfd);
        return EXIT_FAILURE;
    }

    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) != 0) {
        perror("Error connecting to server");
        close(sockfd);
        return EXIT_FAILURE;
    }

    printf("Connected to server %s:%d\n", SERVER_IP, PORT);

    const char *message = "Hello, Server!";
    send(sockfd, message, strlen(message), 0);
    printf("Sent data to server: %s\n", message);

    char buffer[BUFFER_SIZE];
    ssize_t received_bytes = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
    if (received_bytes <= 0) {
        perror("Error receiving data from server");
        close(sockfd);
        return EXIT_FAILURE;
    }

    buffer[received_bytes] = '\0';
    printf("Received data from server: %s\n", buffer);

    close(sockfd);

    return EXIT_SUCCESS;
}
