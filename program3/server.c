// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define PORT 8080
#define MAX_PACKET_SIZE 150  // Limit packet size to 150 bytes

// Function declarations
int create_server_socket();
void bind_server_socket(int server_fd, struct sockaddr_in *address);
int accept_client_connection(int server_fd, struct sockaddr_in *address);
void handle_client(int client_socket);
void close_server_socket(int server_fd);

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    
    server_fd = create_server_socket();
    bind_server_socket(server_fd, &address);

    while (1) {
        client_socket = accept_client_connection(server_fd, &address);
        handle_client(client_socket);
    }

    close_server_socket(server_fd);
    return 0;
}

// Function to create the server socket
int create_server_socket() {
    // TODO: Implement server socket creation
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    return server_fd;
}

// Function to bind the server socket to an address and port
void bind_server_socket(int server_fd, struct sockaddr_in *address) {
    // TODO: Implement binding the server socket to an address
    bind(server_fd, (struct sockaddr*)&address, sizeof(address));
}

// Function to accept client connections
int accept_client_connection(int server_fd, struct sockaddr_in *address) {
    // TODO: Implement accepting client connection
    char buffer[1024];
    socklen_t addr_len = sizeof(address);
    int bytes_recv = recvfrom(server_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&address, &addr_len);

    if (bytes_recv < 0) {
        perror("recvfrom failed");
        return -1;
    }

    return 0; // Indicate successful connection
}

// Function to handle communication with the client
void handle_client(int client_socket) {
    // TODO: Implement the logic to receive and send data to the client
    char buffer[1024];   

    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1) {
        // Receive a message from the client
        int bytes_recv = recvfrom(client_socket, buffer, 1024, 0,
                                  (struct sockaddr *)&client_addr, &addr_len);

        if (bytes_recv < 0) {
            perror("recvfrom failed");
            exit(EXIT_FAILURE);
        }

        // Process the received message (replace this with your specific logic)
        printf("Received message: %s\n", buffer);

        // Send a response back to the client
        const char *response = "Hello from the server!";
        sendto(client_socket, response, strlen(response), 0,
               (const struct sockaddr *)&client_addr, addr_len);
    }
}

// Function to close the server socket
void close_server_socket(int server_fd) {
    // TODO: Implement closing the server socket
    close(server_fd)
}
