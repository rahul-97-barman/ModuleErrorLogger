//Updated
#include <iostream>
#include <csignal>
#include <ctime>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int sockfd;
struct sockaddr_in servaddr;
pid_t pid;
std::string hostname;

std::string getCurrentTimestamp() {
    std::time_t now = std::time(nullptr);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

void sendMessage(const std::string &message) {
    sendto(sockfd, message.c_str(), message.length(), MSG_CONFIRM, (const struct sockaddr *) &servaddr, sizeof(servaddr));
    std::cout << "Message sent: " << message << std::endl;
}

void signalHandler(int signum) {
    std::string errorType;
    if (signum == SIGINT) { // Ctrl+C
        errorType = "INFO";
    } else if (signum == SIGTSTP) { // Ctrl+Z
        errorType = "WARNING";
    } else if (signum == SIGQUIT) { // Ctrl+\
        errorType = "CRITICAL";
    }

    std::string timestamp = getCurrentTimestamp();
    std::string message = timestamp + " " + std::to_string(pid) + " " + hostname + " " + errorType;
    sendMessage(message);
}

int main() {
    pid = getpid(); // Get process ID

    // Get hostname
    char host[256];
    gethostname(host, sizeof(host));
    hostname = std::string(host);

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

    // Register signal handler
    signal(SIGINT, signalHandler);  // Ctrl+C
    signal(SIGTSTP, signalHandler); // Ctrl+Z
    signal(SIGQUIT, signalHandler); // Ctrl+\

    std::cout << "Running application. Use Ctrl+C for INFO, Ctrl+Z for WARNING, Ctrl+\\ for CRITICAL." << std::endl;

    // Keep the application running to catch signals
    while (true) {
        pause(); // Wait for signals
    }

    close(sockfd);
    return 0;
}
