#include <iostream>
#include <Socket/LinuxSocket.h>
#include <Manager/FileManager.h>

int main()
{
    FileManager* FM = FileManager::GetFileManager();
    FM->SweepFiles();
    
    LinuxSocket* ConnectionSocket = new LinuxSocket("DefaultConnection");
    ConnectionSocket->ListenSocket();
    return 1;
} 