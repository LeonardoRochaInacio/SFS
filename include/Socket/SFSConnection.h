#pragma once
#include <Socket/Connection.h>
#include <string>
#include <vector>

class SFSConnection : public Connection
{
    
private:

    std::string AbsolutePath = "/home/leonardo/Arqui-SO/sfs-home";
    std::string RelativePath = "";
    std::vector<std::string> LastCommandTokens;

public:

    SFSConnection(int DescriptorID);

protected:

    std::string GetOutputFromCommand(std::string Command, bool &Success);
    std::string ConvertToAbsolutePath(std::string AbsolutePath);
    std::string ConvertToRelativePath(std::string AbsolutePath, bool &bSucess);
    std::string GetCurrentAboslutePath();
    void CheckCommands();
    virtual void PreRead() override;
    virtual void PostRead(std::string ReadedData) override;
    void ShowDirectory();
    void AccessDirectory();
    void CreateDirectory();
    void RemoveDirectory();
    void CreateFile();
    void RemoveFile();
    void ShowFile();
    void WriteOnFile();
};
