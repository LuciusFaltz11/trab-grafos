#include <iostream>
#include <sstream>

using namespace std;
#include "FileMananger.h"
#include "Grafo.h"

/*
converte a linha do arquivo para (int, int) e chama a função de construção no grafo
se a linha so contiver um inteiro, a mesma representa o numero de nos do grafo (de acordo com a descrição do arquivo readme.txt)
*/
void constroiGrafo(string linha, Grafo *grafo)
{
    istringstream iss(linha);
    int num1, num2;

    if (iss >> num1)
    {
        if (iss >> num2)
        {
            grafo->AddNoAresta(num1, num2);
        }
        else
        {
            
            cout << "O grafo possui " << num1 << " nos. " << endl;
        }
    }
    return;
}

int main(int argc, char const *argv[])
{
    FileMananger fileMananger;

    cout << "Selecione o arquivo para leitura: " << endl;
    fileMananger.ListAvailableFiles();
    int selectedFileIndex = -1;
    cin >> selectedFileIndex;
    string selectedFileName = fileMananger.GetFileNameByIndex(selectedFileIndex);
    if (selectedFileName.compare("") == 0)
    {
        cout << "índice de arquivo invalido!" << endl;
        return 1;
    }
    cout << "Arquivo selecionado: " << selectedFileName << endl;
    Grafo grafo;
    fileMananger.Read(selectedFileName, &constroiGrafo, &grafo);
    cout << "Numedo de arestas: " << grafo.duplo << endl;
    return 0;
}
