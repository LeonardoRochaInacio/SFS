#pragma once

#include <string>
#include <sys/un.h>
#include <vector>
#include <memory>
#include <Socket/SFSConnection.h>

class LinuxSocket
{

private:

    std::vector<std::unique_ptr<Connection>> Connections;

    std::string SocketName;

    int SocketIdentifier;

    bool bCreationSuccess = false;

    int RunningProcessID = -1;

    char MessageBuffer[1024];

    struct sockaddr_un Address;

public:

    LinuxSocket(std::string SocketName);

    ~LinuxSocket();

    void ListenSocket();
    
private:

    LinuxSocket() = delete;

    void CreateSocket();

    void SocketConnectionCheck();

    void SocketLoop();

    void ClearSocket();

    void * InstanceEntryPoint(void * Parameter);

};
