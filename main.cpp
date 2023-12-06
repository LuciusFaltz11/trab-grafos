#define DEBUG false
#define RESET "\033[0m"
#define BOLDGREEN "\033[1m\033[32m"

#include <iostream>
#include <sstream>
#include <random>

#include <chrono>
#include <ctime>
// #include <cmath>

#include <fstream>
#include <filesystem>
// #include <cstdlib>

#include "Grafo.h"
#include "FileMananger.h"

#include "ListaEconomias.h"
#include "Economia.h"
#include "ListaNos.h"
#include "No.h"
#include "ListaRotas.h"
#include "Rota.h"
#include "SeletorAlfa.h"

// using namespace std;
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
int getCapacidadeCaminhao(string fileLocation)
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
        // ...
        // DEMAND_SECTION

        string linha;

        int capacidade = 0;
        while (linha.find("CAPACITY") != 0)
        {
            getline(file, linha);
        }
        if (linha.find("CAPACITY") != string::npos)
        {
            string capacidadeStr = linha.substr(linha.find(":") + 1);
            capacidade = stoi(capacidadeStr);
        }
        return capacidade;
    }
    return 0;
}
int getNoOfTrucks(string fileLocation)
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
        // ...
        // DEMAND_SECTION

        string linha;
        getline(file, linha);
        while (linha.find("No of trucks") == string::npos)
        {
            getline(file, linha);
        }
        if (linha.find("No of trucks: ") != string::npos)
        {
            string noOfTrucksStr = linha.substr(linha.find("No of trucks: ") + 14);
            noOfTrucksStr = noOfTrucksStr.substr(0, noOfTrucksStr.find(","));
            // cout << "noOfTrucksStr = >> " << noOfTrucksStr << endl;
            int noOfTrucks = stoi(noOfTrucksStr);
            return noOfTrucks;
        }
        // cout << "linha => > > " << linha << endl;
    }
    return 0;
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
        // ...
        // DEMAND_SECTION

        string linha;

        int totalNos = 0;
        while (linha.find("CAPACITY") != 0)
        {
            getline(file, linha);
            if (linha.find("CAPACITY") != string::npos)
            {
                string dimension = linha.substr(linha.find(":") + 1);
                totalNos = stoi(dimension);
            }
        }
        grafo->setTotalNos(totalNos);

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
            grafo->AddNoCoord(id, x, y);
            getline(file, linha);
        }
        getline(file, linha);
        while (linha.find("DEPOT_SECTION") != 0)
        {
            istringstream iss(linha);
            int id, peso;
            iss >> id;
            iss >> peso;
            grafo->AddPesoAoNo(id, peso);
            getline(file, linha);
        }
    }
}

void preencheGrafo(Grafo *grafo)
{
    // função que pega os nós do grafo e preenche o grafo com as arestas com peso igual a distancia entre os nos
    No *noNav = grafo->getRaizGrafo();
    No *noNav2 = grafo->getRaizGrafo();
    while (noNav != NULL)
    {
        while (noNav2 != NULL)
        {
            cout << "Adicionando aresta entre " << noNav->getId() << " e " << noNav2->getId() << endl;
            if (noNav->getId() != noNav2->getId())
            {
                // double distancia = sqrt(pow(noNav->getCoordenadaX() - noNav2->getCoordenadaX(), 2) + pow(noNav->getCoordenadaY() - noNav2->getCoordenadaY(), 2));
                double distancia = 0;
                grafo->AddNoAresta(noNav->getId(), noNav2->getId(), distancia);
            }
            noNav2 = noNav2->getProxNo();
        }
        noNav = noNav->getProxNo();
        noNav2 = grafo->getRaizGrafo();
    }
}
bool rotasSaoEquivalentes(Rota *rota1, Rota *rota2)
{
    // if (rota1->getNElementos() != rota2->getNElementos())
    // {
    //     return false;
    // }
    if (DEBUG)
        cout << "Pegando o primeiro elemento" << endl;
    No *noNav1 = rota1->getPrimeiroElemento();
    No *noNav2 = rota2->getPrimeiroElemento();

    if (DEBUG)
        cout << "Verificando se a segunda rota inclui a primeira" << endl;
    while (noNav1 != NULL)
    {
        if (!rota2->possuiNo(noNav1->getId()))
        {
            return false;
        }
        noNav1 = noNav1->getProxNo();
    }
    if (DEBUG)
    {
        cout << "Verificando se a primeira rota inclui a segunda" << endl;
        cout << "Rota 2: ";
        rota2->imprime();
    }
    while (noNav2 != NULL)
    {
        if (DEBUG)
        {
            cout << "noNav2->getId() = " << noNav2->getId() << endl;
            cout << "rota1->possuiNo(noNav2->getId()) = " << rota1->possuiNo(noNav2->getId()) << endl;
        }
        if (!rota1->possuiNo(noNav2->getId()))
        {

            return false;
        }
        noNav2 = noNav2->getProxNo();
    }
    if (DEBUG)
    {
        cout << "Ta retornando que as rotas sao equivalentes!" << endl;
        cout << "Rota 1: ";
        rota1->imprime();
        cout << "Rota 2: ";
        rota2->imprime();
    }
    return true;
}
float calcularDistanciaNos(No *no1, No *no2)
{
    if (DEBUG)
    {

        cout << "Calculando distancia entre " << no1->getId() << " e " << no2->getId() << endl;
        cout << "No1 X = " << no1->getCoordenadaX() << " Y = " << no1->getCoordenadaY() << endl;
        cout << "No2 X = " << no2->getCoordenadaX() << " Y = " << no2->getCoordenadaY() << endl;
    }
    float distancia = sqrt(pow(no1->getCoordenadaX() - no2->getCoordenadaX(), 2) + pow(no1->getCoordenadaY() - no2->getCoordenadaY(), 2));

    if (DEBUG)
        cout << "Distancia entre " << no1->getId() << " e " << no2->getId() << " = " << distancia << endl;
    return distancia;
}

bool avaliarCapacidadeMaximaRota(Rota *rota, Rota *novaRota)
{
    int capMax = rota->getCapacidade();
    int capacidadeAtual = 0;

    return (capacidadeAtual <= capMax);
}

Rota *mesclarRotas(Rota *rota1, Rota *rota2)
{
    if (DEBUG)
    {
        cout << "Mesclando rotas: " << endl;
        cout << "Rota 1: ";
        rota1->imprime();
        cout << "Rota 2: ";
        rota2->imprime();
    }

    //* rota começa no ponto inicial
    Rota *novaRota = new Rota(rota1->getCapacidade());
    // novaRota->AddElemento(1, 0, 0, 0);
    novaRota->AddElemento(
        rota1->getPrimeiroElemento()->getId(),   //* tem que ser o 1
        rota1->getPrimeiroElemento()->getPeso(), //* tem que ser 0
        rota1->getPrimeiroElemento()->getCoordenadaX(),
        rota1->getPrimeiroElemento()->getCoordenadaY());
    Rota *rotaReferencia = rota1->somaRota(rota2);

    if (DEBUG)
    {
        cout << "Somando rotas: " << endl;
        rotaReferencia->imprime();
        cout << "Mesclando rotas: " << endl;
    }
    No *noNav1 = rota1->getPrimeiroElemento();
    No *noNav2 = rota2->getPrimeiroElemento();

    while (!rotasSaoEquivalentes(novaRota, rotaReferencia))
    {
        if (DEBUG)
            cout << "As rotas não são equivalentes!" << endl;
        //* adicionar o no da rota mais proximo ao ultimo no da rota
        float menorDistancia = 9999999;
        No *noMaisProximo = NULL;
        noNav1 = rota1->getPrimeiroElemento();
        noNav2 = rota2->getPrimeiroElemento();
        while (noNav1 != NULL)
        {
            if (novaRota->possuiNo(noNav1->getId()))
            {
                if (DEBUG)
                    cout << "O no " << noNav1->getId() << " ja esta na rota" << endl;
                noNav1 = noNav1->getProxNo();
                continue;
            }
            float distancia = calcularDistanciaNos(novaRota->getUltimoElemento(), noNav1);

            if (DEBUG)
                cout << "Distancia entre = " << distancia << endl;
            if (distancia < menorDistancia)
            {
                menorDistancia = distancia;
                noMaisProximo = noNav1;
            }
            noNav1 = noNav1->getProxNo();
        }
        while (noNav2 != NULL)
        {
            if (novaRota->possuiNo(noNav2->getId()))
            {
                if (DEBUG)
                    cout << "O no " << noNav2->getId() << " ja esta na rota" << endl;
                noNav2 = noNav2->getProxNo();
                continue;
            }
            float distancia = calcularDistanciaNos(novaRota->getUltimoElemento(), noNav2);
            if (DEBUG)
                cout << "Distancia entre" << distancia << endl;
            if (distancia < menorDistancia)
            {
                menorDistancia = distancia;
                noMaisProximo = noNav2;
            }
            noNav2 = noNav2->getProxNo();
        }

        if (DEBUG)
        {
            cout << "Fim de procura pelo no mais proximo" << endl;
            cout << "nova rota: ";
            novaRota->imprime();
            cout << "rota referencia: ";
            rotaReferencia->imprime();
            cout << "Menor distancia: " << menorDistancia << endl;
            cout << "No mais proximo: " << noMaisProximo->getId() << endl;
        }
        novaRota->AddElemento(noMaisProximo->getId(), noMaisProximo->getPeso(), noMaisProximo->getCoordenadaX(), noMaisProximo->getCoordenadaY());
    }
    // novaRota->AddElemento(1, 0, 0, 0);
    novaRota->AddElemento(
        rota1->getPrimeiroElemento()->getId(),   //* tem que ser o 1
        rota1->getPrimeiroElemento()->getPeso(), //* tem que ser 0
        rota1->getPrimeiroElemento()->getCoordenadaX(),
        rota1->getPrimeiroElemento()->getCoordenadaY());
    if (DEBUG)
    {
        cout << "Rota mesclada com distancia " << novaRota->getDistanciaTotal() << endl;
        novaRota->imprime();
    }
    return novaRota;
}
ListaEconomias *calculaEconomias(ListaRotas *listaRotas)
{
    if (DEBUG)
        cout << "Calcular economias" << endl;
    ListaEconomias *economias = new ListaEconomias(listaRotas->getCapacidade());

    Rota *rotaNav = listaRotas->getPrimeiroElemento();
    int noNavI = 0;
    int noNav2I = 0;
    while (rotaNav != NULL)
    {
        Rota *rotaNav2 = listaRotas->getPrimeiroElemento();
        noNavI++;
        noNav2I = 0;
        while (rotaNav2 != NULL)
        {
            noNav2I++;
            if (rotaNav == rotaNav2)
            {
                rotaNav2 = rotaNav2->getProxElemento();
                continue;
            }
            if (DEBUG)
            {
                cout << "Calculando economia entre: " << endl;
                rotaNav->imprime();
                rotaNav2->imprime();
            }
            // Verificar se a junção das duas rotas excede a capacidade máxima

            Rota *novaRota = mesclarRotas(rotaNav, rotaNav2);
            if (DEBUG)
            {

                cout << "resultado: " << endl;
                novaRota->imprime();
            }

            if (novaRota->getCapacidadeAtual() > novaRota->getCapacidade())
            {
                rotaNav2 = rotaNav2->getProxElemento();

                continue;
            }
            float valorEconomizado = rotaNav->getDistanciaTotal() + rotaNav2->getDistanciaTotal() - novaRota->getDistanciaTotal();

            if (DEBUG)
                cout << "Valor economizado: " << valorEconomizado << endl;

            Economia *novaEconomia = new Economia(novaRota, valorEconomizado);
            economias->AddElemento(novaEconomia);

            rotaNav2 = rotaNav2->getProxElemento();
        }
        rotaNav = rotaNav->getProxElemento();
    }

    return economias;
}

bool estaContido(Rota *rota1, Rota *rota2)
{
    No *noNav = rota1->getPrimeiroElemento();
    while (noNav != NULL)
    {
        if (!rota2->possuiNo(noNav->getId()))
        {
            return false;
        }
        noNav = noNav->getProxNo();
    }
    return true;
}

void incluiMergeNasRotas(Rota *novaRota, ListaRotas *listaRotas)
{
    // Remover todas as rotas da lista de rotas que foram mescladas e estão contidas na nova rota

    Rota *rotaNav = listaRotas->getPrimeiroElemento();

    while (rotaNav != NULL)
    {
        if (estaContido(rotaNav, novaRota))
        {
            listaRotas->removeElemento(rotaNav);
        }
        rotaNav = rotaNav->getProxElemento();
    }
    listaRotas->AddElemento(novaRota);
}

void generateGraphvizFile(Grafo *grafo, ListaRotas *rotas, string filePathName = "./out/graphviz.txt")
{
    ofstream outdata; // outdata is like cin

    outdata.open(filePathName); // opens the file

    outdata << "graph G {" << endl;
    outdata << "layout=\"fdp\";" << endl;
    No *noNav = grafo->getRaizGrafo();
    while (noNav != NULL)
    {
        outdata << noNav->getId() << " [pos=\"" << noNav->getCoordenadaX() << "," << noNav->getCoordenadaY() << "!\", width=0, height=0, fillcolor=green, style=filled];" << endl;
        noNav = noNav->getProxNo();
    }
    Rota *rotaNav = rotas->getPrimeiroElemento();

    // criar uma array de string de cores para as rotas
    string cores[10] = {"red", "blue", "green", "yellow", "orange", "purple", "pink", "brown", "black", "gray"};
    int corAtual = 0;

    while (rotaNav != NULL)
    {
        No *noNav = rotaNav->getPrimeiroElemento();
        while (noNav != NULL)
        {
            No *noNav2 = noNav->getProxNo();
            if (noNav2 != NULL)
            {
                outdata << noNav->getId() << " -- " << noNav2->getId() << "[color=\"" << cores[corAtual % 10] << "\" , penwidth=15]"
                        << ";" << endl;
            }
            noNav = noNav->getProxNo();
        }
        corAtual++;
        rotaNav = rotaNav->getProxElemento();
    }
    outdata << "}" << endl;
}

float **matrizDistancias(Grafo *grafo)
{
    float **matriz = new float *[grafo->getTotalNos()];
    for (int i = 0; i < grafo->getTotalNos(); i++)
    {
        matriz[i] = new float[grafo->getTotalNos()];
    }

    for (int i = 0; i < grafo->getTotalNos(); i++)
    {
        for (int j = 0; j < grafo->getTotalNos(); j++)
        {
            matriz[i][j] = 0;
        }
    }

    No *noNav = grafo->getRaizGrafo();
    while (noNav != NULL)
    {
        No *noNav2 = grafo->getRaizGrafo();
        while (noNav2 != NULL)
        {
            if (noNav->getId() != noNav2->getId())
            {
                matriz[noNav->getId() - 1][noNav2->getId() - 1] = calcularDistanciaNos(noNav, noNav2);
            }
            noNav2 = noNav2->getProxNo();
        }
        noNav = noNav->getProxNo();
    }

    // teste
    cout << "Matriz de distancias: " << endl;
    for (int i = 0; i < grafo->getTotalNos(); i++)
    {
        for (int j = 0; j < grafo->getTotalNos(); j++)
        {
            cout << matriz[i][j] << " ";
        }
        cout << endl;
    }
    // fim-teste

    return matriz;
}
float calculateCustoTotal(ListaRotas *listaRotas)
{
    float custoTotal = 0;
    Rota *rotaNav = listaRotas->getPrimeiroElemento();
    while (rotaNav != NULL)
    {
        custoTotal += rotaNav->getDistanciaTotal();
        rotaNav = rotaNav->getProxElemento();
    }
    return custoTotal;
}

void geraLogDasRotas(ListaRotas *rotas, string filePathName = "./out/LogsRotas.txt")
{
    ofstream outdata; // outdata is like cin

    // cout << "filePathName = > " << endl;
    // cout << filePathName << endl;
    outdata.open(filePathName, std::ios_base::app); // opens the file
    int rotaIndex = 0;
    Rota *rotaNav = rotas->getPrimeiroElemento();
    while (rotaNav != NULL)
    {
        outdata << "==============================================" << endl;
        outdata << "Rota " << rotaIndex << ": ";
        No *noNav = rotaNav->getPrimeiroElemento();
        while (noNav != NULL)
        {
            outdata << noNav->getId() << " ";
            noNav = noNav->getProxNo();
        }
        outdata << endl;
        outdata << "Distancia total: " << rotaNav->getDistanciaTotal() << endl;
        outdata << "Capacidade total: " << rotaNav->getCapacidade() << endl;
        outdata << "Capacidade atual: " << rotaNav->getCapacidadeAtual() << endl;
        outdata << "==============================================" << endl;
        rotaIndex++;
        rotaNav = rotaNav->getProxElemento();
    }
    outdata << "Custo total: " << calculateCustoTotal(rotas) << endl;
    outdata.close();
}

int randomRange(int min, int max)
{
    // Generate a random number between min and max (inclusive)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

ListaRotas *algoritmoClarkeWright(Grafo *grafo, string testeName = "teste", float alfa = -1)
{

    const int capacidadeCaminhao = 100;
    const int quantidadeRotas = 5;

    if (alfa != -1 && alfa < 0 && alfa > 1)
    {
        return NULL;
    }

    // Clarke-Wright
    // 1. Crie uma lista de rotas vazia.
    // 2. Para cada par de nós, calcule a economia de mesclagem.
    // 3. Classifique as economias de mesclagem em ordem decrescente.
    // 4. Para cada economia de mesclagem, verifique se os nós podem ser mesclados.
    // 5. Se os nós puderem ser mesclados, mesclar os nós e adicionar a rota à lista de rotas.
    // 6. Se os nós não puderem ser mesclados, verifique se os nós podem ser adicionados a uma rota existente.
    // 7. Se os nós puderem ser adicionados a uma rota existente, adicione os nós à rota.
    // 8. Se os nós não puderem ser adicionados a uma rota existente, crie uma nova rota com os nós.
    // 9. Retorne a lista de rotas.

    ListaRotas *rotas = new ListaRotas(capacidadeCaminhao);
    ListaEconomias *economias = new ListaEconomias(capacidadeCaminhao);

    No *noNav = grafo->getRaizGrafo();
    No *origem = new No(
        grafo->getRaizGrafo()->getId(),
        grafo->getRaizGrafo()->getPeso(),
        grafo->getRaizGrafo()->getCoordenadaX(),
        grafo->getRaizGrafo()->getCoordenadaY());
    while (noNav != NULL)
    {

        if (noNav->getId() == 1)
        {
            noNav = noNav->getProxNo();
            continue;
        }
        if (DEBUG)
        {

            cout << "------------------------------------------------------" << endl;
            cout << "Adicionando no " << noNav->getId() << endl;
            cout << "nonav x = " << noNav->getCoordenadaX() << " y = " << noNav->getCoordenadaY() << endl;

            cout << "== Criando a rota ==" << endl;
        }

        Rota *newRota = new Rota(capacidadeCaminhao);

        if (DEBUG)
        {
            cout << "newRota == NULL: " << (newRota->getUltimoElemento() == NULL) << endl;
        }
        // newRota->AddElemento(1, 0, 0, 0);
        newRota->AddElemento(
            origem->getId(),   //* tem que ser o 1
            origem->getPeso(), //* tem que ser 0
            origem->getCoordenadaX(),
            origem->getCoordenadaY());

        newRota->AddElemento(
            noNav->getId(),
            noNav->getPeso(),
            noNav->getCoordenadaX(),
            noNav->getCoordenadaY());

        // newRota->AddElemento(1, 0, 0, 0);
        newRota->AddElemento(
            origem->getId(),   //* tem que ser o 1
            origem->getPeso(), //* tem que ser 0
            origem->getCoordenadaX(),
            origem->getCoordenadaY());

        rotas->AddElemento(newRota);

        if (DEBUG)
        {
            cout << "newRota == NULL: " << (newRota->getUltimoElemento() == NULL) << endl;
            cout << "criado com sucesso" << endl;
        }

        noNav = noNav->getProxNo();
    }
    if (DEBUG)
    {

        cout << "fim do loop " << endl;

        cout << "Rotas criadas: " << endl;
        Rota *rotaNav = rotas->getPrimeiroElemento();
        while (rotaNav != NULL)
        {
            rotaNav->imprime();
            rotaNav = rotaNav->getProxElemento();
        }
    }

    if (DEBUG)
    {
        cout << "==============================================" << endl;
        cout << "========== < Calculando economias > ==========" << endl;
        cout << "==============================================" << endl;
    }

    //* Aqui ta a parte do guloso em si, ordenando as economias e pegando a melhor delas
    const bool mostrarLog = false; //* se quiser ver o arquivo de log, mudar para true (mais lento)
    ofstream outdata;              // outdata is like cin
    if (mostrarLog)
    {
        outdata.open("./out/" + testeName + "/Logs.txt"); // opens the file
    }

    int iteracao = 0;

    if (DEBUG)
        cout << "rotas->getNElementos() = " << rotas->getNElementos() << endl;
    // while (rotas->getNElementos() >= 10)
    while (rotas->getNElementos() >= quantidadeRotas)
    {
        economias = calculaEconomias(rotas);
        if (economias->getNElementos() == 0)
        {
            if (DEBUG)
            {
                cout << "Não há mais economias a serem feitas!" << endl;
            }
            break;
        }
        //! Essa parte so fica comentada pro algorítimo guloso simples
        if (alfa != -1)
        {
            economias->sort();
        }

        if (DEBUG)
        {
            if (mostrarLog)
            {
                outdata << "iteracao: " << iteracao << " => ";
            }
            cout << "economias: " << endl;
            economias->imprime();
            cout << "Rotas antes do merge: " << endl;
            rotas->imprime();
        }

        //! aqui mudar para randomizado
        // incluiMergeNasRotas(economias->getPrimeiroElemento()->getRota(), rotas);

        if (alfa != -1)
        {
            int k = randomRange(0, alfa * (economias->getNElementos() - 1));
            Rota *rotaToMerge = economias->getElemento(k)->getRota();
            incluiMergeNasRotas(rotaToMerge, rotas);
        }
        else
            incluiMergeNasRotas(economias->getMaiorEconomia()->getRota(), rotas);

        // fim-teste
        if (mostrarLog)
        {
            outdata << "iteracao: " << iteracao << " => ";
            Rota *rotaNav = rotas->getPrimeiroElemento();
            while (rotaNav != NULL)
            {
                outdata << rotaNav->getDistanciaTotal() << " ";
                rotaNav = rotaNav->getProxElemento();
            }
            outdata << endl;
        }
        iteracao++;
        if (DEBUG)
        {

            cout << "Rotas depois do merge: " << endl;
            rotas->imprime();
        }
    }

    // generateGraphvizFile(grafo, rotas, "./out/" + testeName + "/graphviz.txt");
    // geraLogDasRotas(rotas, "./out/" + testeName + "/LogsRotas.txt");
    outdata.open("./out/" + testeName + "/LogsRotas.txt", std::ios_base::app);
    // cout << "O custo total foi de: " << calculateCustoTotal(rotas) << endl;
    outdata.close();
    return rotas;
}
struct ResultadoClarkeWrightRandomizado
{
    float custoMedio;
    float alfa;
    ListaRotas *melhorResultado;
};

ResultadoClarkeWrightRandomizado ClarkeWrightRandomizado(Grafo *grafo, string nomeTeste, int capacidade, float alfa)
{
    const int nIteracoes = 30;
    ListaRotas *melhorResultado = new ListaRotas(capacidade);
    float somaCusto = 0;

    float melhorCusto = 99999999;
    for (int i = 0; i < nIteracoes; i++)
    {
        ListaRotas *resultado = algoritmoClarkeWright(grafo, nomeTeste, alfa);
        float custo = calculateCustoTotal(resultado);
        cout << BOLDGREEN << "Custo da iteracao " << i << " com alfa: " << alfa << " = " << custo << RESET << endl;
        somaCusto += custo;
        if (custo < melhorCusto)
        {
            if (false) //! mudar para true caso queira gerar os graphviz e os logs de todas as melhores rotas
            {

                generateGraphvizFile(grafo, melhorResultado, "./out/" + nomeTeste + "/Graphviz" + std::to_string(i) + ".txt");
                geraLogDasRotas(melhorResultado, "./out/" + nomeTeste + "/LogsRotas" + std::to_string(i) + ".txt");
            }
            melhorCusto = custo;
            melhorResultado = resultado;
        }
    }

    cout << BOLDGREEN << "A media do algorítimo de ClarkeWriteRandomizado para o alfa = " << alfa << " foi de " << somaCusto / nIteracoes << RESET << endl;

    // return somaCusto / nIteracoes;
    ResultadoClarkeWrightRandomizado resultado;
    resultado.custoMedio = somaCusto / nIteracoes;
    resultado.alfa = alfa;
    resultado.melhorResultado = melhorResultado;
    return resultado;
}
struct ClarkeWrightReativoResultado
{
    ListaRotas *melhorResultado;
    SeletorAlfa *seletorAlfa;
};

ClarkeWrightReativoResultado ClarkeWrightReativo(Grafo *grafo, string nomeTeste, int capacidade)
{
    ofstream outdata;                                      // outdata is like cin
    outdata.open("./out/" + nomeTeste + "/LogsAlfas.txt"); // opens the file

    SeletorAlfa *seletorAlfa = new SeletorAlfa();
    float menorCusto = 99999999;
    ListaRotas *melhorResultado = new ListaRotas(capacidade);
    for (int iteracao = 0; iteracao < 1000; iteracao++)
    {
        int alfaSelecionadoIndex = seletorAlfa->selecionarAlfaIndex();
        float alfaSelecionado = seletorAlfa->getAlfa(alfaSelecionadoIndex);
        //* nessa implementação, selecionamos um alfa e rodamos o algorítimo randomizado com o mesmo alfa 30 vezes
        //* e pegamos a media dos custos para atualizar a probabilidade do alfa
        ResultadoClarkeWrightRandomizado resultado = ClarkeWrightRandomizado(grafo, nomeTeste, capacidade, alfaSelecionado);
        if (calculateCustoTotal(resultado.melhorResultado) < menorCusto)
        {
            menorCusto = calculateCustoTotal(resultado.melhorResultado);
            melhorResultado = resultado.melhorResultado;
        }
        seletorAlfa->atualizarProbabilidade(alfaSelecionadoIndex, resultado.custoMedio);
        outdata << " ========================================================= " << endl;
        outdata << "iteracao: " << iteracao << endl;
        outdata << "Resultado da iteracao: " << resultado.custoMedio << endl;
        outdata << "Melhor resultado da iteracao: " << calculateCustoTotal(resultado.melhorResultado) << endl;
        if (iteracao % 100 == 0)
        {
            for (int i = 0; i < seletorAlfa->getNAlfas(); i++)
            {
                outdata << "[ " << seletorAlfa->getAlfa(i) << " ] probabilidade = " << seletorAlfa->getProbabilidade(i) << endl;
            }
            generateGraphvizFile(grafo, melhorResultado, "./out/" + nomeTeste + "/Graphviz" + std::to_string(iteracao) + ".txt");
            geraLogDasRotas(melhorResultado, "./out/" + nomeTeste + "/LogsRotas" + std::to_string(iteracao) + ".txt");
        }
    }
    generateGraphvizFile(grafo, melhorResultado, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
    geraLogDasRotas(melhorResultado, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");
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
    // ex.: ./a ./A-n34-k5.txt ./out 0 1 1 teste
    //* alfa = -1 : algoritmo guloso simples
    // ex.: g++ *.cpp && ./a ./A-n34-k5.txt ./out 0 1 1 teste
    // <arquivo_entrada> <arquivo_saida> <Opc_Direc> <Opc_Peso_Aresta> <Opc_Peso_Nos> <nome_teste>

    string arquivoEntrada = "";
    string arquivoSaida = "";
    bool direcionado = false;
    bool ponderadoAresta = false;
    bool ponderadoNo = false;
    string nomeTeste = "";
    float alfa = -1;

    int tipoGrafo = 0;
    //* se o programa for chamado com argumentos, o programa ignora o sistema de seleção de arquivo
    if (argc == 8)
    {
        tipoGrafo = 2;
        // cout << "Argumentos passados: " << endl;
        // for (int i = 0; i < argc; i++)
        // {
        //     cout << "argv[" << i << "] = " << (argv[i]) << endl;
        // }
        arquivoEntrada = argv[1];
        arquivoSaida = argv[2];
        direcionado = (argv[3][0] == '1');
        ponderadoAresta = (argv[4][0] == '1');
        ponderadoNo = (argv[5][0] == '1');
        nomeTeste = argv[6];
        alfa = atof(argv[7]);

        // cout << "Arquivo de entrada: " << arquivoEntrada << endl;
        // cout << "Arquivo de saida: " << arquivoSaida << endl;
        // cout << "O grafo e direcionado? " << direcionado << endl;
        // cout << "O grafo e ponderado nas arestas? " << ponderadoAresta << endl;
        // cout << "O grafo e ponderado nos nos? " << ponderadoNo << endl;
        // cout << "Nome do teste: " << nomeTeste << endl;

        ofstream outdata; // outdata is like cin
        outdata.open("./out/" + nomeTeste + "/LogsRotas.txt", std::ios_base::app);
        outdata << "Nome do teste: " << nomeTeste << endl;
        outdata << "Arquivo de entrada: " << arquivoEntrada << endl;
        outdata.close();
    }
    else
    {
        tipoGrafo = 1;
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

    while (tipoGrafo != 1 && tipoGrafo != 2)
    {
        cout << "tipo grafo == 2: " << (tipoGrafo == 2) << endl;
        cout << "Qual o tipo de arquivo do grafo grafo?" << endl;
        cout << "1: grafo primeira parte" << endl;
        cout << "2: grafo segunda parte" << endl;
        cin >> tipoGrafo;
    };

    FileMananger fileMananger;
    Grafo grafo(direcionado == 's', ponderadoAresta, ponderadoNo);
    ClarkeWrightReativoResultado resultado;
    ClarkeWrightReativoResultado melhorResultado;

    auto start = chrono::system_clock::now(); //! inicio de codigo para contagem de tempo de execução
    if (tipoGrafo == 2)
    {
        //* gera um grafo sem arestas, apenas com os nos e suas coordenadas
        controiGrafoTipo2(arquivoEntrada, &grafo);
        if (nomeTeste.find("Randomizado") != string::npos)
        {
            cout << BOLDGREEN << "Algoritmo Clarke-Wright Randomizado" << RESET << endl;
            int capacideCaminhao = getCapacidadeCaminhao(arquivoEntrada);
            ResultadoClarkeWrightRandomizado resultado = ClarkeWrightRandomizado(&grafo, nomeTeste, capacideCaminhao, alfa);

            generateGraphvizFile(&grafo, resultado.melhorResultado, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
            geraLogDasRotas(resultado.melhorResultado, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");

            cout << "O custo medio foi de: " << resultado.custoMedio << endl;
            cout << "O alfa foi de: " << resultado.alfa << endl;
            cout << "O melhor resultado foi: " << endl;
            resultado.melhorResultado->imprime();
        }
        if (nomeTeste.find("Reativo") != string::npos)
        {
            cout << BOLDGREEN << "Algoritmo Clarke-Wright Reativo" << RESET << endl;
            int capacideCaminhao = getCapacidadeCaminhao(arquivoEntrada);
            resultado = ClarkeWrightReativo(&grafo, nomeTeste, capacideCaminhao);
            cout << "O melhor resultado da iteração foi: " << endl;
            resultado.melhorResultado->imprime();
            if (calculateCustoTotal(resultado.melhorResultado) < calculateCustoTotal(melhorResultado.melhorResultado))
            {
                melhorResultado = resultado;
            }
            // algoritmoClarkeWright(&grafo, nomeTeste);
        }
        // algoritmoClarkeWright(&grafo, nomeTeste, alfa);
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
    ofstream outdata;
    outdata.open("./out/" + nomeTeste + "/Resultado.txt", std::ios_base::app);
    outdata << "O custo total foi de: " << calculateCustoTotal(melhorResultado.melhorResultado) << endl;
    outdata << "O tempo gasto foi de " << elapsed_seconds.count() << " s" << endl;
    outdata.close();

    // cout << "O grafo é: " << endl;
    // cout << "Ponderado nas arestas " << grafo.getPonderadoAresta() << endl;
    // cout << "Ponderado nos vertices " << grafo.getPonderadoVertice() << endl;

    int input;
    do
    {
        if (tipoGrafo == 2)
        {
            return 0;
        }
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
