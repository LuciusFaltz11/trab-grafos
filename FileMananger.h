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

    /*
func Read:
    Read(string fileName) fileName: nomeDoArquivo.txt
        Imprime o aquivo no console linha por linha

    Read(string fileName, void (*func)(string, Grafo*), Grafo *grafo)
        -> filename: nomeDoArquivo.txt
        -> void (*func)(string, Grafo*): função do tipo void function(string linha, Grafo *grafoParaConstrução)
        -> grafo: grafo para construção

        chama a função passada a cada linha passando como argumento a linha do arquivo e o grafo para construção
    */
    void Read(string fileName);
    void Read(string fileName, void (*func)(string, Grafo*), Grafo *grafo);
};
