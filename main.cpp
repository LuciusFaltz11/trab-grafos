#define RESET "\033[0m"
#define BOLDGREEN "\033[1m\033[32m"

#include <iostream>
#include <sstream>

#include <chrono>
#include <ctime>

#include <fstream>
#include <cstdlib>

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
    int num1, num2, peso;
    if (iss >> num1)
    {
        if (iss >> num2)
        {
            if (num1 == num2)
            //! verifica se tem self loop
            {
                return;
            }
            if (grafo->getPonderadoAresta() || grafo->getPonderadoVertice())
            {
                iss >> peso;
                grafo->AddNoAresta(num1, num2, peso);
            }
            else
            {
                grafo->AddNoAresta(num1, num2);
            }
        }
        else
        {

            cout << "O grafo possui " << num1 << " nos. " << endl;
        }
    }
    return;
}

void menuOpcoes(){
    cout << "Escolha uma opcao: " << endl;
    cout << "[ 0 ] cancelar " << endl;
    cout << "[ 1 ] nos diretamente conectados " << endl;
    cout << "[ 2 ] fecho transitivo direto " << endl;
    cout << "[ 3 ] fecho transitivo indireto " << endl;
    cout << "[ 4 ] arvore dada pela ordem de caminhamento em profundidade " << endl;
    cout << "[ 5 ] arvore dada pela ordem de caminhamento em profundidade com DreampufFile " << endl;
}

int main(int argc, char const *argv[])
{
    //! sistema de seleção de arquivo
    FileMananger fileMananger;

    char grafoPonderado = ' ';
    do
    {
        cout << "O grafo e ponderado? (s/n)" << endl;
        cin >> grafoPonderado;
    } while (grafoPonderado != 's' && grafoPonderado != 'n');
    int ponderadoId = 0;
    if (grafoPonderado == 's')
    {
        do
        {
            cout << "Qual o tipo de ponderacao?" << endl;
            cout << "1: ponderado na aresta" << endl;
            cout << "2: ponderado no vertice" << endl;
            cout << "3: ponderado nos dois" << endl;
            cin >> ponderadoId;
        } while (ponderadoId < 0 || ponderadoId > 3);
    }
    if (ponderadoId == 3)
    {
        cout << "Grafos ponderados nos vértices e arestas ainda não são suportados!" << endl;
        return 1;
    }

    cout << "Selecione o arquivo para leitura: " << endl;
    fileMananger.ListAvailableFiles(grafoPonderado);
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

    char direcionado = ' ';
    do
    {
        cout << "O grafo e direcionado? (s/n) ";
        cin >> direcionado;
    } while (direcionado != 's' && direcionado != 'n');

    auto start = chrono::system_clock::now(); //! inicio de codigo para contagem de tempo de execução

    Grafo grafo(direcionado == 's', ponderadoId);
    fileMananger.Read(selectedFileName, &constroiGrafo, &grafo); //* le o arquivo chamando a função constroiGrafo a cada linha

    //! fim de cogio de contagem de tempo de execução
    auto end = chrono::system_clock::now();
    chrono::duration<double> elapsed_seconds = end - start;
    time_t end_time = chrono::system_clock::to_time_t(end);
    cout << "grafo criado em: " << BOLDGREEN << elapsed_seconds.count() << " s" << RESET
         << endl;
    //!=================================================

    cout << "O grafo é: " << endl;
    cout << "Ponderado nas arestas " << grafo.getPonderadoAresta() << endl;
    cout << "Ponderado nos vertices " << grafo.getPonderadoVertice() << endl;
    int input;
    do
    {
        cout << "\n\n\nDigite -1 para sair" << endl;
        cout << "Digite o id do no que vc deseja informacoes: ";
        cin >> input;
        if (input < 0)
        {
            continue;
        }

        auto start = chrono::system_clock::now(); //! inicio de codigo para contagem de tempo de execução

        No *noSelecionado = grafo.procuraId(input);

        if (noSelecionado == NULL)
        {
            cout << "O no selecionado nao esta no grafo! " << endl;
            continue;
        }
        cout << "O no selecionado esta no grafo." << endl;

        if (noSelecionado->getPrimeiraAresta() == NULL)
        {
            cout << "Não possui arestas conectadas!" << endl;
            continue;
        }

        int opcao = 0;
        menuOpcoes();
        cin >> opcao;
        switch (opcao)
        {
        case 1:{
            cout << "O no esta diretamente conectado aos nos:";
            Lista *conectado = grafo.getArestasNo(input);
            conectado->iterate([](int id)
                            { cout << id << " "; }); //* essa coisa esquisita é uma lambda function. É meio que um jeito de declarar uma função dentro de outra função em cpp.
            cout << endl;
        }
            break;
        case 2:{
            cout << "Fecho transitivo direto deste vértice: ";
            Lista *fechoTransitivoDireto = grafo.buscaProfundidade(input);
            fechoTransitivoDireto->iterate([](int id)
                                           { cout << id << ", "; });
            cout << endl;
            grafo.generateDreampufFile("grafo.dat");
        }
        case 3:{
            cout << "Fecho transitivo indireto deste vértice: ";
            Grafo *grafoInvertido = grafo.inverteArestasDirecionadas();
            Lista *fechoTransitivoIndireto = grafoInvertido->buscaProfundidade(input);
            fechoTransitivoIndireto->iterate([](int id)
                                           { cout << id << ", "; });
            cout << endl;
            grafoInvertido->generateDreampufFile("grafoInvertido.dat");
            delete grafoInvertido;
        }
        case 4:{
            cout << "Arvore dada pela ordem de caminhamento em profundidade: " << endl;
            grafo.arvoreProfundidade(input);            
            cout << endl;
        }
        case 5:{
            cout << "Arvore dada pela ordem de caminhamento em profundidade com DreampufFile: " << endl;
            grafo.arvoreProfundidade(input, true);            
            cout << endl;
        }

        default:
            break;
        }



        //! fim de cogio de contagem de tempo de execução
        auto end = chrono::system_clock::now();
        chrono::duration<double> elapsed_seconds = end - start;
        time_t end_time = chrono::system_clock::to_time_t(end);
        cout << "\ntempo de execucao: " << BOLDGREEN << elapsed_seconds.count() << " s" << RESET
             << endl;
        //!=================================================

        

    } while (input != -1);

    return 0;
}

