#include <Socket/SFSConnection.h>
#include <cstring>
#include <Manager/FileManager.h>
#include <pthread.h>
#include <algorithm>

SFSConnection::SFSConnection(int DescriptorID) : Connection(DescriptorID)
{
    //CurrentPath = GetOutputFromCommand("pwd");
};

void SFSConnection::PreRead()
{
    Write("sfs@instance-" + std::to_string(DescriptorID) + ":/home" + RelativePath + "/$ ");
}

void SFSConnection::PostRead(std::string ReadedData)
{
    char * Data = const_cast<char*>(ReadedData.erase(ReadedData.length() - 1).c_str());
    Log::Common("Descriptor %lu: %s", DescriptorID, Data);

    LastCommandTokens.clear();
    
    char* Token = strtok(Data, " ");
    while(Token != NULL)
    {
        LastCommandTokens.push_back(std::string(Token));
        Token = strtok(NULL, " ");
    }

    CheckCommands();
}

void SFSConnection::CheckCommands()
{
    if(LastCommandTokens.size() == 0) return;

    if(LastCommandTokens[0] == "vdir")
    {
        ShowDirectory();
    }

    else if(LastCommandTokens[0] == "edir")
    {
        AccessDirectory();
    }

    else if(LastCommandTokens[0] == "cdir")
    {
        CreateDirectory();
    }

    else if(LastCommandTokens[0] == "rdir")
    {
        RemoveDirectory();
    }

    else if(LastCommandTokens[0] == "cfile")
    {
        CreateFile();
    }

    else if(LastCommandTokens[0] == "rfile")
    {
        RemoveFile();
    }

    else if(LastCommandTokens[0] == "vfile")
    {
        ShowFile();
    }

    else if(LastCommandTokens[0] == "efile")
    {
        WriteOnFile();
    }
}

void SFSConnection::ShowDirectory()
{
    bool bSuccess;
    if(LastCommandTokens.size() > 1)
    {
        Write(GetOutputFromCommand("ls " + GetCurrentAboslutePath() + LastCommandTokens[1], bSuccess));
        Write("\n");
    }
    else
    {
        Write(GetOutputFromCommand("ls " + GetCurrentAboslutePath(), bSuccess));
        Write("\n");
    }    
}

void SFSConnection::AccessDirectory()
{
    if(LastCommandTokens.size() > 1)
    {
        bool bSuccess;
        std::string Result = GetOutputFromCommand("cd " + GetCurrentAboslutePath() + "/" + LastCommandTokens[1] + " && pwd", bSuccess);
        if(bSuccess)
        {
            RelativePath = ConvertToRelativePath(Result, bSuccess);
        }
    } 
}

void SFSConnection::CreateDirectory()
{
    if(LastCommandTokens.size() > 1)
    {
        bool bSuccess;
        std::string Result = GetOutputFromCommand("mkdir " + GetCurrentAboslutePath() + "/" + LastCommandTokens[1], bSuccess);
        if(bSuccess)
        {
            Write("Diretório " + LastCommandTokens[1]+ " criado com sucesso\n");
            FileManager::GetFileManager()->SweepFiles();
        }
    } 
}

void SFSConnection::RemoveDirectory()
{
    if(LastCommandTokens.size() > 1)
    {
        bool bSuccess;
        std::string Result = GetOutputFromCommand("rmdir " + GetCurrentAboslutePath() + "/" + LastCommandTokens[1], bSuccess);
        if(bSuccess)
        {
            Write("Diretório " + LastCommandTokens[1]+ " removido com sucesso\n");
            FileManager::GetFileManager()->SweepFiles();
        }
    } 
}

void SFSConnection::CreateFile()
{
    if(LastCommandTokens.size() > 1)
    {
        bool bSuccess;
        std::string Result = GetOutputFromCommand("touch " + GetCurrentAboslutePath() + "/" + LastCommandTokens[1], bSuccess);
        if(bSuccess)
        {
            Write("Arquivo " + LastCommandTokens[1]+ " criado com sucesso\n");
            FileManager::GetFileManager()->SweepFiles();
        }
    } 
}


void SFSConnection::RemoveFile()
{
     if(LastCommandTokens.size() > 1)
    {
        bool bSuccess;
        std::string Path = GetCurrentAboslutePath() + "/" + LastCommandTokens[1];
        FileManager* FM = FileManager::GetFileManager();
        Mutex<File>* CFile;
        std::string RelativePath = "." + ConvertToRelativePath(Path, bSuccess);
        if(CFile = FileManager::GetFileByPath(RelativePath))
        {
            CFile->Lock();
            std::string Result = GetOutputFromCommand("rm " + GetCurrentAboslutePath() + "/" + LastCommandTokens[1], bSuccess);
            Write("Arquivo " + LastCommandTokens[1]+ " removido com sucesso\n");
            CFile->Unlock();
            FileManager::GetFileManager()->SweepFiles();
        }
    } 
}

void SFSConnection::ShowFile()
{
    if(LastCommandTokens.size() > 1)
    {
        bool bSuccess;
        std::string Path = GetCurrentAboslutePath() + "/" + LastCommandTokens[1];
        FileManager* FM = FileManager::GetFileManager();
        Mutex<File>* CFile;
        std::string RelativePath = "." + ConvertToRelativePath(Path, bSuccess);
        if(CFile = FileManager::GetFileByPath(RelativePath))
        {
            CFile->Lock();
            std::string Result = GetOutputFromCommand("cat " + Path, bSuccess);
            if(bSuccess)
            {
                Log::Common("Abrindo arquivo %s...", RelativePath.c_str());
                Write(Result + "\n");
                sleep(10);
                Log::Common("Fechando arquivo %s...", RelativePath.c_str());
            }
            CFile->Unlock();
        }       
    } 
}

void SFSConnection::WriteOnFile()
{
    if(LastCommandTokens.size() > 2)
    {
        bool bSuccess;
        std::string Path = GetCurrentAboslutePath() + "/" + LastCommandTokens[1];
        FileManager* FM = FileManager::GetFileManager();
        Mutex<File>* CFile;
        std::string RelativePath = "." + ConvertToRelativePath(Path, bSuccess);
        if(CFile = FileManager::GetFileByPath(RelativePath))
        {
            CFile->Lock();
            std::string x;
            for_each (LastCommandTokens.begin()+2, LastCommandTokens.end(), [&](const std::string str){x += " "+ str;});
            std::string Result = GetOutputFromCommand("echo " + x + " >> " + Path, bSuccess);
            if(bSuccess)
            {
                Log::Common("Escrevendo no arquivo %s...", RelativePath.c_str());
                sleep(5);
                Log::Common("Escrita no arquivo %s terminada!", RelativePath.c_str());
            }
            CFile->Unlock();
        }       
    }
}

std::string SFSConnection::GetOutputFromCommand(std::string Command, bool &Success)
{
    FILE *File;
    int Status;
    char Data[128];
    std::string Return;
    File = popen(Command.c_str(), "r");
    if (File == NULL) Log::Error("Erro no comando por pipe");
    while (fgets(Data, 128, File) != NULL)
    {
        Return.append(Data);
    }
    Success = (pclose(File) == 0);
    Return.erase(Return.end()-1);
    return Return;
}

std::string SFSConnection::ConvertToAbsolutePath(std::string RelativePath)
{
    return AbsolutePath + RelativePath;
}

std::string SFSConnection::ConvertToRelativePath(std::string AbsolutePath, bool &bSucess)
{
    size_t Position = AbsolutePath.find(this->AbsolutePath);
    if(Position != std::string::npos)
    {
        bSucess = true;
        return AbsolutePath.erase(Position, this->AbsolutePath.length());
    }
    else
    {  
        bSucess = false;
        return "";
    }
}

std::string SFSConnection::GetCurrentAboslutePath()
{
    return ConvertToAbsolutePath(RelativePath);
}