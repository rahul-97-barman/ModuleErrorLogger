// client.cpp
#include <iostream>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void sendMessage(int sockfd, struct sockaddr_in &servaddr, const std::string &message) {
    sendto(sockfd, message.c_str(), message.length(), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    std::cout << "Message sent: " << message << std::endl;
}

int main() {
    int sockfd;
    struct sockaddr_in servaddr;

    // Creating socket file descriptor
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));

    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    std::string input;
    while (true) {
        std::cout << "Enter message (INFO, WARNING, CRITICAL): ";
        std::getline(std::cin, input);

        if (input == "INFO" || input == "WARNING" || input == "CRITICAL") {
            sendMessage(sockfd, servaddr, input);
        } else {
            std::cout << "Invalid input. Please enter INFO, WARNING, or CRITICAL." << std::endl;
        }
    }

    close(sockfd);
    return 0;
}
