#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct addrinfo hints;
    struct addrinfo *server_addr = nullptr;
    char buffer[BUFFER_SIZE];

    std::memset(&hints, 0, sizeof(hints));

    // Set up hints for getaddrinfo
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;

    // Get server address information
    int result = getaddrinfo("127.0.0.1", "6667", &hints, &server_addr);
    if (result != 0) {
        std::cerr << "Failed to get server address: " << gai_strerror(result) << std::endl;
        exit(1);
    }

    // Create client socket
    client_fd = socket(server_addr->ai_family, server_addr->ai_socktype, server_addr->ai_protocol);
    if (client_fd == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    // Connect to the server
    if (connect(client_fd, server_addr->ai_addr, server_addr->ai_addrlen) == -1) {
        perror("Failed to connect to server");
        exit(1);
    }

    std::cout << "Connected to IRC server." << std::endl;

    freeaddrinfo(server_addr);  // No longer needed

    while (true) {
        std::cout << "Enter a message (or 'q' to quit): ";
        std::cin.getline(buffer, BUFFER_SIZE);

        if (std::strcmp(buffer, "q") == 0) {
            break;
        }

        ssize_t bytes_sent = write(client_fd, buffer, strlen(buffer));
        if (bytes_sent == -1) {
            perror("Failed to send message");
            exit(1);
        }

        ssize_t bytes_received = read(client_fd, buffer, BUFFER_SIZE);
        if (bytes_received == -1) {
            perror("Failed to receive response");
            exit(1);
        }

        buffer[bytes_received] = '\0';
        std::cout << "Received response: " << buffer << std::endl;
    }

    // Clean up
    close(client_fd);

    return 0;
}