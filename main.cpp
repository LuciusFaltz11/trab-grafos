#include <iostream>

using namespace std;
#include "FileMananger.h"

int main(int argc, char const *argv[])
{
    FileMananger fileMananger;

    cout << "Selecione o arquivo para leitura: " << endl;
    fileMananger.ListAvailableFiles();
    int selectedFileIndex = -1;
    cin >> selectedFileIndex;

    if (fileMananger.GetFileNameByIndex(selectedFileIndex).compare("") == 0)
    {
        cout << "índice de arquivo invalido!" << endl;
        return 1;
    }
    cout << "Arquivo selecionado: " << fileMananger.GetFileNameByIndex(selectedFileIndex) << endl;

    return 0;
}
