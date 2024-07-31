// server.cpp
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void forwardMessage(int sockfd, struct sockaddr_in &displayaddr, const std::string &message) {
    sendto(sockfd, message.c_str(), message.length(), MSG_CONFIRM, (const struct sockaddr *) &displayaddr, sizeof(displayaddr));
}

int main() {
    int sockfd;
    char buffer[BUFFER_SIZE];
    struct sockaddr_in servaddr, cliaddr, displayaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    memset(&displayaddr, 0, sizeof(displayaddr));

    // Filling server information
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(PORT);

    // Bind the socket with the server address
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Display module address configuration
    displayaddr.sin_family = AF_INET;
    displayaddr.sin_port = htons(PORT + 1); // Different port for the display module
    displayaddr.sin_addr.s_addr = INADDR_ANY;

    int n;
    socklen_t len = sizeof(cliaddr);

    while (true) {
        n = recvfrom(sockfd, (char *)buffer, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr *)&cliaddr, &len);
        buffer[n] = '\0';
        std::string message = buffer;

        std::cout << "Received message: " << message << std::endl;
        forwardMessage(sockfd, displayaddr, message);
        std::cout << "Message forwarded to display module." << std::endl;
    }

    close(sockfd);
    return 0;
}
