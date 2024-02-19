# CHTTP
**Simple Server Client with Linux Socket Programming.**

## Simple Server in C
- **Author:** Jason Pandain
- **Date:** 17-02-2024

**Description:** This server program follows a basic sequence of steps for establishing a network connection. The process includes the creation of a socket, binding it to a specific address and port, and listening for incoming connections. Upon accepting a connection, the server forks a child process to handle the communication with the connected client. The communication is simple, responding with a predefined message. Finally, the server gracefully shuts down when a termination signal (Ctrl+C) is received. The overall structure can be summarized as follows:

```
socket -> bind -> listen -> accept -> fork -> handle_client -> close sockets
```

**Key Steps:**

1. **Create a socket using `socket()`**
   - Use the `socket()` function to create a socket for communication.

2. **Bind the socket to a specific address and port using `bind()`**
   - Associate the socket with a specific network address and port number.

3. **Start listening for incoming connections with `listen()`**
   - Enable the socket to accept incoming connections from clients.

4. **Accept incoming connections using `accept()`**
   - Wait for and accept incoming client connections.

5. **Fork a child process to handle the client communication**
   - Create a new process to handle communication with the connected client.

6. **Handle the client communication in the child process**
   - Implement the communication logic with the connected client.

7. **Close the sockets in both parent and child processes**
   - Terminate the communication by closing the sockets in both processes.

**Note:** This is a simplified illustration for educational purposes and may lack error handling for brevity.


# Simple Client in C

**Description:** This client program establishes a connection with a server following a basic sequence of steps. The process includes creating a socket, configuring the server address structure, and connecting to the server. Once connected, the client sends a message to the server and receives a response. The overall structure can be summarized as follows:

```
socket -> configure server address -> connect -> send data -> receive data -> close socket
```

**Key Steps:**

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

**Note:** This is a simplified illustration for educational purposes and may lack error handling for brevity.

