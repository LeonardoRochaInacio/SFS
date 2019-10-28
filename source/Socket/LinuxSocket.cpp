#include <LinuxSocket.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <Log/Log.h>
#include <pthread.h>

 LinuxSocket::LinuxSocket(std::string SocketName)
 {
    this->SocketName = "/tmp/" + SocketName + ".sock";
    Log::Common("Iniciando criação de socket listener %s", this->SocketName.c_str());
    RunningProcessID = getpid();   
    CreateSocket();
 }

 LinuxSocket::~LinuxSocket()
 {
     ClearSocket();
 }

void LinuxSocket::CreateSocket()
{
    SocketIdentifier = socket(AF_UNIX, SOCK_STREAM, 0);
    
    if(SocketIdentifier == -1)
    {
        Log::Error("Erro na criação do socket");
        return;
    }

    ClearSocket();
    Address.sun_family = AF_UNIX;
    strcpy(Address.sun_path, this->SocketName.c_str()); 
    int BindResult = bind(SocketIdentifier, (struct sockaddr*)&Address, sizeof(struct sockaddr_un));

    if(BindResult == -1)
    {
        Log::Error("Erro no socket bind");
        return;
    }

    bCreationSuccess = true;
}

void LinuxSocket::ListenSocket()
{
    if(bCreationSuccess)
    {
        listen(SocketIdentifier, 2);
        Log::Common("Ouvindo socket %s", SocketName.c_str());
        SocketConnectionCheck();
    }
    else
    {
        Log::Warning("Não foi possível escutar o socket %s, socket inválido", SocketName.c_str());
    }
}

void LinuxSocket::SocketConnectionCheck()
{  
        while(true)
        {
            int NewDescriptorID = accept(SocketIdentifier, nullptr, nullptr);
            if(NewDescriptorID == -1)
            {
                Log::Error("Falha na conexão com um cliente");
                sleep(1);
            }
            else
            {
                new SFSConnection(NewDescriptorID); 
            }
        }     
}

void LinuxSocket::ClearSocket()
{
    Log::Common("Limpando %s", SocketName.c_str());
    unlink(this->SocketName.c_str());
    memset(&Address, 0, sizeof(struct sockaddr_un));
}