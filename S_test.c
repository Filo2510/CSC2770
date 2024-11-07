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
void handle_client(int server_fd);
void close_server_socket(int server_fd);

int main() {
    int server_fd;
    struct sockaddr_in address;

    server_fd = create_server_socket();
    bind_server_socket(server_fd, &address);

    printf("Server is running and listening on port %d...\n", PORT);

    handle_client(server_fd);

    close_server_socket(server_fd);
    return 0;
}

// Function to create the server socket
int create_server_socket() {
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return server_fd;
}

// Function to bind the server socket to an address and port
void bind_server_socket(int server_fd, struct sockaddr_in *address) {
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;  // Accept connections on any network interface
    address->sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)address, sizeof(*address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
}

// Function to handle communication with the client
void handle_client(int server_fd) {
    char buffer[MAX_PACKET_SIZE];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1) {
        int bytes_recv = recvfrom(server_fd, buffer, MAX_PACKET_SIZE, 0, 
                                  (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_recv < 0) {
            perror("recvfrom failed");
            continue;
        }

        buffer[bytes_recv] = '\0';  // Null-terminate the received message
        printf("Received message: %s\n", buffer);

        // Send a response back to the client (optional)
        const char *response = "Message received";
        sendto(server_fd, response, strlen(response), 0, 
               (const struct sockaddr *)&client_addr, addr_len);
    }
}

// Function to close the server socket
void close_server_socket(int server_fd) {
    close(server_fd);
}
