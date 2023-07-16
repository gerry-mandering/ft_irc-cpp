#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/event.h>
#include <netdb.h>

#define MAX_CLIENTS 10
#define BUFFER_SIZE 1024

int main() {
    int server_fd, kq, event_count;
    struct addrinfo hints;
    struct addrinfo *server_addr = nullptr;
    struct kevent change, events[MAX_CLIENTS];
    char buffer[BUFFER_SIZE];

    std::memset(&hints, 0, sizeof(hints));

    // Set up hints for getaddrinfo
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    // Get server address information
    int result = getaddrinfo(nullptr, "6667", &hints, &server_addr);
    if (result != 0) {
        std::cerr << "Failed to get server address: " << gai_strerror(result) << std::endl;
        exit(1);
    }

    // Create server socket
    server_fd = socket(server_addr->ai_family, server_addr->ai_socktype, server_addr->ai_protocol);
    if (server_fd == -1) {
        perror("Failed to create socket");
        exit(1);
    }

    // Bind socket to address
    if (bind(server_fd, server_addr->ai_addr, server_addr->ai_addrlen) == -1) {
        perror("Failed to bind socket");
        exit(1);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("Failed to listen for connections");
        exit(1);
    }

    // Create kqueue
    kq = kqueue();
    if (kq == -1) {
        perror("Failed to create kqueue");
        exit(1);
    }

    // Set up event filter for server socket
    EV_SET(&change, server_fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);

    // Register server socket for monitoring
    if (kevent(kq, &change, 1, nullptr, 0, nullptr) == -1) {
        perror("Failed to register server socket to kqueue");
        exit(1);
    }

    std::cout << "Server started. Listening for connections..." << std::endl;

    freeaddrinfo(server_addr);  // No longer needed

    while (true) {
        // Wait for events
        event_count = kevent(kq, nullptr, 0, events, MAX_CLIENTS, nullptr);
        if (event_count == -1) {
            perror("Error in kevent");
            exit(1);
        }

        // Handle events
        for (int i = 0; i < event_count; ++i) {
            int fd = events[i].ident;

            // New client connection
            if (fd == server_fd) {
                // Accept the new connection
                int client_fd = accept(server_fd, nullptr, nullptr);
                if (client_fd == -1) {
                    perror("Failed to accept client connection");
                    exit(1);
                }

                // Set up event filter for client socket
                EV_SET(&change, client_fd, EVFILT_READ, EV_ADD, 0, 0, nullptr);

                // Register client socket for monitoring
                if (kevent(kq, &change, 1, nullptr, 0, nullptr) == -1) {
                    perror("Failed to register client socket to kqueue");
                    exit(1);
                }

                std::cout << "New client connected: " << client_fd << std::endl;
            } else {  // Existing client activity
                ssize_t bytes_read = read(fd, buffer, BUFFER_SIZE);
                if (bytes_read == -1) {
                    perror("Failed to read from client");
                    exit(1);
                }

                std::cout << "Client Message: " << buffer << std::endl;

                if (bytes_read == 0) {
                    // Connection closed by client
                    close(fd);
                    std::cout << "Client disconnected: " << fd << std::endl;
                } else {
                    // Echo received message back to the client
                    if (write(fd, buffer, bytes_read) == -1) {
                        perror("Failed to write to client");
                        exit(1);
                    }
                }
            }
        }
    }

    // Clean up
    close(server_fd);
    close(kq);

    return 0;
}
