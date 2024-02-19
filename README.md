# CHTTP
Simple Server Client with Linux Socket Programming.

##   Simple Client in C
--   Author: Jason Pandain
--   Date: 17-02-2024

**Description:** This client program establishes a connection with a server following a basic sequence of steps. The process includes creating a socket, configuring the server address structure, and connecting to the server. Once connected, the client sends a message to the server and receives a response. The overall structure can be summarized as follows:

```   socket -> configure server address -> connect -> send data -> receive data -> close socket

## Key Steps:

1. **Create a socket using `socket()`**
   - Use the `socket()` function to create a socket for communication.

2. **Configure the server address structure (IP and port)**
   - Set up the server's address information, including the IP address and port number.

3. **Connect to the server using `connect()`**
   - Establish a connection to the server using the `connect()` function.

4. **Send data to the server using `send()`**
   - Use the `send()` function to transmit data from the client to the server.

5. **Receive data from the server using `recv()`**
   - Utilize the `recv()` function to receive data from the server.

6. **Close the socket after communication**
   - Terminate the connection by closing the socket using the `close()` function.


   Note: This is a simplified illustration for educational purposes and may lack error handling for brevity.
