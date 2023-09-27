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
    fileMananger.Read(selectedFileName, &constroiGrafo, &grafo); //* le o arquivo chamando a função constroiGrafo a cada linha

    int input;
    do
    {
        cout << "Digite -1 para sair" << endl;
        cout << "Digite o id do no que vc deseja informacoes: ";
        cin >> input;
        if (input >= 0)
        {
            No *noSelecionado = grafo.procuraId(input);
            if (noSelecionado == NULL)
            {
                cout << "O no selecionado não esta no grafo! " << endl;
            }
            else
            {

                cout << "O no selecionado esta no grafo." << endl;
                if (noSelecionado->getPrimeiraAresta() == NULL)
                {
                    cout << "Não possui arestas conectadas!" << endl;
                    break;
                }
                else
                {

                    Aresta *arestaP = noSelecionado->getPrimeiraAresta();
                    cout << "O no esta conectado aos nos: ";
                    while (arestaP != NULL)
                    {
                        cout << arestaP->getDestino() << ", ";
                        arestaP = arestaP->getProxAresta();
                    }
                    cout << endl;
                }
            }
        }
    } while (input != -1);

    return 0;
}
