#include <iostream>
#include <boost/lexical_cast.hpp>
#include <unistd.h>
#include <cstring>
#include <sys/types.h>
#include <sys/socket.h> // sockaddr_in
#include <netinet/in.h> // sockaddr_in
#include <netdb.h> // gethostbyname, hostent

void error(const char *msg)
{
    std::cerr << msg;
    exit(1);
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        error(std::string("Usage: " + std::string(argv[0]) + std::string("<hostname> <port_no>\n")).c_str());
    }

    // Create client socket
    const int clientSocketFd = socket(AF_INET, SOCK_STREAM, 0);
    if(clientSocketFd < 0)
    {
        error("ERROR opening socket\n");
    }

    // Setup server to connect to
    const int serverPortNo = boost::lexical_cast<int>(argv[2]);
    hostent* serverInfo = gethostbyname(argv[1]);
    if(nullptr == serverInfo)
    {
        error("ERROR no such host\n");
    }
    sockaddr_in serv_addr;
    std::memset(&serv_addr, 0, sizeof(serv_addr));
    std::memcpy(&serv_addr.sin_addr.s_addr, serverInfo->h_addr, serverInfo->h_length);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(serverPortNo);

    // Connect to server. `clientSocketFd` will be used for send/receive operations
    const auto connectionStatus = connect(clientSocketFd, reinterpret_cast<sockaddr*>(&serv_addr), sizeof(serv_addr));

    if(connectionStatus < 0)
    {
        error("ERROR connecting to host\n");
    }

    // Create message to send to server
    std::string messageToSend;
    std::cout << "Message to send: ";
    std::cin >> messageToSend;

    // Write it to file descriptor, this will be read by server
    const auto bytesWritten = write(clientSocketFd, messageToSend.c_str(), messageToSend.length());
    if(bytesWritten < 0)
    {
        error("ERROR writing to socket\n");
    }

    // Create buffer which'll be used as storage for received message from server
    char buffer[256];
    std::memset(buffer, 0, 256);
    // Read from communication fd. Return value it number of bytes read.
    const int bytesRead = read(clientSocketFd, buffer, 255);
    if(bytesRead < 0)
        error("Error on read");

    std::cout << "Read from file descriptor: " << buffer << std::endl;;

    close(clientSocketFd);
    return 0;
}