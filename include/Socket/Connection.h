#pragma once
#include <Thread/Threadable.h>
#include <unistd.h>
#include <strings.h>

#define BUFFER_SIZE 512

class Connection : public Threadable<void*>
{

private:

    Connection() = delete;    
    char DataBuffer[BUFFER_SIZE];

protected:

    int DescriptorID;

public:

    Connection(int DescriptorID);
    void Read();
    void Write(std::string Data);
    ~Connection();

protected:

    virtual void* ThreadRun() override;
    virtual void PostRead(std::string ReadedData){};
    virtual void PreRead(){};

private:

    bool CheckSocketConnection();

};