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
void constroiGrafo(string linha, Grafo *grafo, int tipoArquivo)
{
    if (tipoArquivo == 1)
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
                grafo->setTotalNos(num1);
                cout << "O grafo possui " << num1 << " nos. " << endl;
            }
        }
    }
    return;
}
void controiGrafoTipo2(string fileLocation, Grafo *grafo)
{
    fstream file;
    file.open(fileLocation, ios::in);
    if (!file)
    {
        cout << "Arquivo '" << fileLocation << "' nao encontrado! " << endl;
    }
    else
    {
        // NAME : A-n34-k5
        // COMMENT : (Augerat et al, No of trucks: 5, Optimal value: 778)
        // TYPE : CVRP
        // DIMENSION : 34
        // EDGE_WEIGHT_TYPE : EUC_2D
        // CAPACITY : 100
        // NODE_COORD_SECTION
        // 1 41 49
        // ...
        // DEMAND_SECTION

        //     for (int i = 0; i < 20; i++)
        //     {
        //         string linha;
        //         getline(file, linha);
        //         cout << linha << " linha.find('NODE_COORD_SECTION') => " << linha.find("NODE_COORD_SECTION") << endl;
        //     }
        // return;

        string linha;
        cout << "Características do arquivo: " << endl;
        while (linha.find("NODE_COORD_SECTION") != 0)
        {
            getline(file, linha);
            cout << linha << endl;
        }
        getline(file, linha);
        while (linha.find("DEMAND_SECTION") != 0)
        {
            istringstream iss(linha);
            int id, x, y;
            iss >> id;
            iss >> x;
            iss >> y;
            // cout << id << " " << x << " " << y << endl;
            grafo->AddNoCoord(id, x, y);
            getline(file, linha);
        }
    }
}

void menuOpcoes()
{
    cout << "Escolha uma opcao: " << endl;
    cout << "[ 0 ] cancelar " << endl;
    cout << "[ 1 ] nos diretamente conectados " << endl;
    cout << "[ 2 ] fecho transitivo direto " << endl;
    cout << "[ 3 ] fecho transitivo indireto " << endl;
    cout << "[ 4 ] arvore dada pela ordem de caminhamento em profundidade " << endl;
    cout << "[ 5 ] arvore dada pela ordem de caminhamento em profundidade com DreampufFile " << endl;
    cout << "[ 6 ] arvore minima kruskal " << endl;
}

string selecionarArquivo(bool grafoPonderado)
{
    //! sistema de seleção de arquivo
    FileMananger fileMananger;

    cout << "Selecione o arquivo para leitura: " << endl;
    fileMananger.ListAvailableFiles(grafoPonderado);
    int selectedFileIndex = -1;
    cin >> selectedFileIndex;
    string selectedFileName = fileMananger.GetFileNameByIndex(selectedFileIndex);
    if (selectedFileName.compare("") == 0)
    {
        cout << "índice de arquivo invalido!" << endl;
        return "";
    }
    cout << "Arquivo selecionado: " << selectedFileName << endl;
    return selectedFileName;
    //! fim do sistema de seleção de arquivo
}

int main(int argc, char const *argv[])
{
    //* estrutura de argumentos:
    // <arquivo_entrada> <arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos>

    string arquivoEntrada = "";
    string arquivoSaida = "";
    bool direcionado = false;
    bool ponderadoAresta = false;
    bool ponderadoNo = false;

    //* se o programa for chamado com argumentos, o programa ignora o sistema de seleção de arquivo
    if (argc == 6)
    {
        arquivoEntrada = argv[1];
        arquivoSaida = argv[2];
        direcionado = (argv[3] == "1");
        ponderadoAresta = (argv[4] == "1");
        ponderadoNo = (argv[5] == "1");
        cout << "Arquivo de entrada: " << arquivoEntrada << endl;
        cout << "Arquivo de saida: " << arquivoSaida << endl;
        cout << "O grafo e direcionado? " << direcionado << endl;
        cout << "O grafo e ponderado nas arestas? " << ponderadoAresta << endl;
        cout << "O grafo e ponderado nos nos? " << ponderadoNo << endl;
    }
    else
    {
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
        if (ponderadoId == 1)
        {
            ponderadoAresta = true;
        }
        else if (ponderadoId == 2)
        {
            ponderadoNo = true;
        }
        else if (ponderadoId == 3)
        {
            cout << "Grafos ponderados nos vértices e arestas ainda não são suportados!" << endl;
            return 1;
        }
        if (ponderadoId != 0)
        {
            arquivoEntrada = "./files/grafosPonderados/" + selecionarArquivo(grafoPonderado == 's');
        }
        else
        {
            arquivoEntrada = "./files/grafosNaoPonderados/" + selecionarArquivo(grafoPonderado == 's');
        }

        char direcionadoChar = ' ';
        do
        {
            cout << "O grafo e direcionado? (s/n) ";
            cin >> direcionadoChar;
        } while (direcionadoChar != 's' && direcionadoChar != 'n');
        direcionado = (direcionadoChar == 's');
    }


    int tipoGrafo = 0;
    do
    {
        cout << "Qual o tipo de arquivo do grafo grafo?" << endl;
        cout << "1: grafo primeira parte" << endl;
        cout << "2: grafo segunda parte" << endl;
        cin >> tipoGrafo;
    } while (tipoGrafo != 1 && tipoGrafo != 2);

    FileMananger fileMananger;
    Grafo grafo(direcionado == 's', ponderadoAresta, ponderadoNo);

    auto start = chrono::system_clock::now(); //! inicio de codigo para contagem de tempo de execução
    if (tipoGrafo == 2)
    {
        //* gera um grafo sem arestas, apenas com os nos e suas coordenadas
        controiGrafoTipo2(arquivoEntrada, &grafo);
    }
    else
    {
        fileMananger.Read(arquivoEntrada, &constroiGrafo, &grafo, tipoGrafo); //* le o arquivo chamando a função constroiGrafo a cada linha
        grafo.generateDreampufFile("grafo.dat");
    }
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
        case 1:
        {
            cout << "O no esta diretamente conectado aos nos:";
            Lista *conectado = grafo.getArestasNo(input);
            conectado->iterate([](int id)
                               { cout << id << " "; }); //* essa coisa esquisita é uma lambda function. É meio que um jeito de declarar uma função dentro de outra função em cpp.
            cout << endl;
        }
        break;
        case 2:
        {
            cout << "Fecho transitivo direto deste vértice: ";
            Lista *fechoTransitivoDireto = grafo.buscaProfundidade(input);
            fechoTransitivoDireto->iterate([](int id)
                                           { cout << id << ", "; });
            cout << endl;
        }
        break;
        case 3:
        {
            cout << "Fecho transitivo indireto deste vértice: ";
            if (direcionado == 's')
            {

                Grafo *grafoInvertido = grafo.inverteArestasDirecionadas();
                Lista *fechoTransitivoIndireto = grafoInvertido->buscaProfundidade(input);
                fechoTransitivoIndireto->iterate([](int id)
                                                 { cout << id << ", "; });
                cout << endl;
                grafoInvertido->generateDreampufFile("grafoInvertido.dat");
                delete grafoInvertido;
            }
            else
            {
                cout << "operacao invalida por ser grafo não ordenado! " << endl;
            }
        }
        break;

        case 4:
        {
            cout << "Arvore dada pela ordem de caminhamento em profundidade: " << endl;
            grafo.arvoreProfundidade(input);
            cout << endl;
        }
        break;

        case 5:
        {
            cout << "Arvore dada pela ordem de caminhamento em profundidade com DreampufFile: " << endl;
            grafo.arvoreProfundidade(input, true);
            cout << endl;
        }
        break;
        case 6:
        {
            int vertice;
            Lista *vertices = new Lista();
            cout << "Selecione os vertices que deseja na arvore minima: " << endl;
            do
            {
                cout << "Digite o id do vertice: (digite -1 para sair)";
                cin >> vertice;
                if (vertice == -1)
                {
                    break;
                }
                if (grafo.procuraId(vertice) == NULL)
                {
                    cout << "O vertice selecionado nao esta no grafo! " << endl;
                }
                else
                {
                    vertices->AddElemento(vertice);
                }
            } while (vertice != -1);
            grafo.arvoreMinimaKruskal(vertices);
        }
        break;

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
