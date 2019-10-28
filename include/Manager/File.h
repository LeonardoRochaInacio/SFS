#pragma once
#include <string>
#include <pthread.h>

class File
{

public:

    File();

    File(std::string Path);

    std::string Path = "";    
    
};
