#include <iostream>
#include <sstream>

#include <chrono>
#include <ctime>

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
    //! sistema de seleção de arquivo
    FileMananger fileMananger;
    char tipoAresta;
    cout << "O grafo é ponderado? (s/n)" << endl;
    cin >> tipoAresta;
    cout << "Selecione o arquivo para leitura: " << endl;
    fileMananger.ListAvailableFiles(tipoGrafo);


    char direcionado;
    cout << "O grafo é direcionado? (s/n)" << endl;
    cin >> direcionado;

    int selectedFileIndex = -1;
    cin >> selectedFileIndex;
    string selectedFileName = fileMananger.GetFileNameByIndex(selectedFileIndex);
    if (selectedFileName.compare("") == 0)
    {
        cout << "índice de arquivo invalido!" << endl;
        return 1;
    }
    cout << "Arquivo selecionado: " << selectedFileName << endl;
    //! fim do sistema de seleção de arquivo

    auto start = chrono::system_clock::now(); //! inicio de codigo para contagem de tempo de execução

    if(tipoGrafo == 's'){
        Grafo grafo(direcionado, tipoAresta);
    } else Grafo grafo(true);

    fileMananger.Read(selectedFileName, &constroiGrafo, &grafo); //* le o arquivo chamando a função constroiGrafo a cada linha

    //! fim de cogio de contagem de tempo de execução
    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    time_t end_time = chrono::system_clock::to_time_t(end);
    cout << "grafo criado em: " << elapsed_seconds.count() << " s"
         << endl;
    //!=================================================

    int input;
    do
    {
        cout << "\n\n\nDigite -1 para sair" << endl;
        cout << "Digite o id do no que vc deseja informacoes: ";
        cin >> input;
        if (input >= 0)
        {
            auto start = chrono::system_clock::now(); //! inicio de codigo para contagem de tempo de execução

            No *noSelecionado = grafo.procuraId(input);

            if (noSelecionado == NULL)
            {
                cout << "O no selecionado nao esta no grafo! " << endl;
            }
            else
            {

                cout << "O no selecionado esta no grafo." << endl;
                if (noSelecionado->getPrimeiraAresta() == NULL)
                {
                    cout << "Não possui arestas conectadas!" << endl;
                }
                else
                {

                    Aresta *arestaP = noSelecionado->getPrimeiraAresta();
                    cout << "O no esta diretamente conectado aos nos: ";
                    while (arestaP != NULL)
                    {
                        cout << arestaP->getDestino() << ", ";
                        arestaP = arestaP->getProxAresta();
                    }
                    cout << endl;

                    cout << "Fecho transitivo direto deste vértice: ";
                    grafo.buscaProfundidade(input);
                }
            }
            //! fim de cogio de contagem de tempo de execução
            auto end = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;
            time_t end_time = chrono::system_clock::to_time_t(end);
            cout << "\ntempo de execucao: " << elapsed_seconds.count() << " s"
                 << endl;
            //!=================================================
        }
    } while (input != -1);

    return 0;
}
