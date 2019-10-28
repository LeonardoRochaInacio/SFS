#include <Manager/FileManager.h>
#include <Manager/File.h>
#include <Thread/Mutex.h>
#include <iostream>
#include <string>

FileManager* FileManager::Instance = 0;

FileManager* FileManager::GetFileManager()
{
    if(Instance == 0)
    {
        Instance = new FileManager();
    }

    return Instance;
}

void FileManager::SweepFiles()
{
    FILE *File;
    int Status;
    char Data[256];
    Files.clear();
    std::string Command = "cd sfs-home && find . -name *\\* -print";
    File = popen(Command.c_str(), "r");
    while (fgets(Data, 256, File) != NULL)
    {
        std::string FilePath(Data);
        FilePath.erase(FilePath.end()-1);

        FileManager::GetFileManager()->Files.push_back( std::make_unique<Mutex<class File>>() );

        Files.back()->GetData()->Path = FilePath;
    }

}

Mutex<File>* FileManager::GetFileByPath(std::string Path)
{
    FileManager* FM = FileManager::GetFileManager();
    for(int i = 0; i < FM->Files.size(); i++)
    {
        if(FM->Files[i]->GetData()->Path == Path)
        {
            return FM->Files[i].get();
        }
    }

    return nullptr;
}

FileManager::FileManager(){}