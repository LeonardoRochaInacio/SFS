#include <iostream>
#include <Socket/Connection.h>
#include <Log/Log.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <cstring>

Connection::Connection(int DescriptorID) : DescriptorID(DescriptorID)
{
    ThreadStart();
    Log::Success("Nova conexão criada - Socket Descriptor: %d", DescriptorID);
}

Connection::~Connection()
{
    Log::Resource("Encerrando conexão - Socket Descriptor: %d", DescriptorID);
    close(DescriptorID);
}

void* Connection::ThreadRun()
{
    while(CheckSocketConnection())
    {
        Read();
    }
    
    this->~Connection();
    return nullptr;
}

bool Connection::CheckSocketConnection()
{
    char b[1];
    int ConnectionError = send(DescriptorID, b, 1, MSG_NOSIGNAL);

    if(ConnectionError == -1)
    {
        return false;
    }

    return true;     
}

void Connection::Read()
{
    PreRead();
    bzero(&DataBuffer, sizeof(DataBuffer));
    ssize_t ReadReturn;
    ReadReturn = read(DescriptorID, &DataBuffer, BUFFER_SIZE);

    if(ReadReturn == -1)
    {
        Log::Error("Falha na leitura de dados no Descriptor %lu - Thread %lu", DescriptorID, ThreadID);
    }

    if(ReadReturn > 0)
    {
        std::string STRData(DataBuffer);
        PostRead(STRData);
    }
}

void Connection::Write(std::string Data)
{
    if(write(DescriptorID, Data.c_str(), Data.size()) == -1)
    {
        Log::Error("Erro na escrita para o Descriptor %lu da Thread %lu", DescriptorID, ThreadID);
    }
}