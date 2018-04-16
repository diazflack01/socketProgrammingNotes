#include <boost/lexical_cast.hpp>
#include <iostream>
#include <cstring>
#include <unistd.h> // write, read, close
#include <sys/types.h>  // socket, bind, accept
#include <sys/socket.h> // socket, bind, accept, listen, 
#include <netinet/in.h> // sockaddr_in, inet_ntoa
#include <arpa/inet.h> // htonl, htons, ntohl, ntohs, inet_ntoa

void error(const char *msg)
{
    std::cerr << msg;
    exit(1);
}

int main(int argc, char** argv)
{
    if(argc != 2)
    {
        error(std::string("Usage: " + std::string(argv[0]) + std::string(" <port_no>\n")).c_str());
    }

    const int serverPort = boost::lexical_cast<int>(argv[1]);

    // Create server socket
    const int serverSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(serverSocketFd < 0)
        error("Error opening socket");

    // Create server address and set port to the value provided by user
    sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(serverPort);

    // Bind the server socket to the created server address which has the `port`
    const int bindStatus = bind(serverSocketFd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));
    if(bindStatus < 0)
        error("Error: Cannot bind to port");

    // Max pending connection queue. When this is full, client may receive ECONNREFUSED
    constexpr int maxBackLogConnectionQueue = 3;
    // Listen to the create server socket that was binded to the given port
    std::cout << "listening for connections...\n";
    listen(serverSocketFd, maxBackLogConnectionQueue);

    // Create client address variable which'll be used in storing client information
    sockaddr_in clie_addr;
    socklen_t clie_len = sizeof(clie_addr);
    // Accept a connection from the server socket which'll return a new file descriptor solely used for communication between server
    // and a specific client
    const int communicationSocketFd = accept(serverSocketFd, reinterpret_cast<sockaddr*>(&clie_addr), &clie_len);
    std::cout << "Connection request detected, will attempt to accept...\n";
    if(communicationSocketFd < 0)
        error("Error: Accepting connection of client port");

    std::cout << "server: got connection from " << inet_ntoa(clie_addr.sin_addr) << " port:"<< ntohs(clie_addr.sin_port) << std::endl;

    // Ask message to send to client
    std::cout << "Message to client: ";
    std::string msgToclient;
    std::cin >> msgToclient;
    // Send message to client using the communication file descriptor
    send(communicationSocketFd, msgToclient.c_str(), msgToclient.length(), 0);

    // Create buffer which'll be used as storage for received message from client/communication fd
    char buffer[256];
    std::memset(buffer, 0, 256);
    // Read from communication fd. Return value it number of bytes read.
    const int bytesRead = read(communicationSocketFd, buffer, 255);
    if(bytesRead < 0)
        error("Error on read");
    
    std::cout << "Read from file descriptor: " << buffer << std::endl;;

    close(communicationSocketFd);
    close(serverSocketFd);
    return 0;
}