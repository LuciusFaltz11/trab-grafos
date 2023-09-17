#include <string>
#include <fstream>
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;
using namespace std;

class FileMananger
{
private:
    fstream file;
    string path = "";
public:
    FileMananger();
    ~FileMananger();
    void ListAvailableFiles(); //* lista todos os arquivos dentro da pasta ./files
    string GetFileNameByIndex(int index);
    void Read(string fileName);    

};
