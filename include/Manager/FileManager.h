#pragma once
#include <memory>
#include <vector>
#include <Manager/File.h>
#include <Thread/Mutex.h>

/**
 * @brief  Classe singleton para gerenciar o file system como objeto unico.
 */
class FileManager
{

private:

    FileManager();
    static FileManager* Instance;

public:

    static FileManager* GetFileManager();
    void SweepFiles();
    static Mutex<File>* GetFileByPath(std::string Path);

    std::vector<    std::unique_ptr<    Mutex<File>     >    > Files;

};
