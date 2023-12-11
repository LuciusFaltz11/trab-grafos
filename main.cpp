#define DEBUG false
#define RESET "\033[0m"
#define BOLDGREEN "\033[1m\033[32m"

#include <iostream>
#include <sstream>
#include <random>

#include <chrono>
#include <ctime>
#include <cmath>

#include <fstream>
#include <filesystem>

#include <vector>
#include <algorithm>
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
#include "ListaJuncao.h"

//============================================================//
const int mesclarRotasIteracoes = 1;
const int iteracoesReativo = 2500;
const int iteracoesRandomizado = 500;
const int iteracoesParaMelhoraFinalDeRotas = 1000;
const int repeticoesMediaAdaptativo = 5;
//============================================================//




float **matrizDistancias = NULL;
float calcularDistanciaNos(No *no1, No *no2)
{
    if (DEBUG)
    {

        std::cout << "Calculando distancia entre " << no1->getId() << " e " << no2->getId() << endl;
        std::cout << "No1 X = " << no1->getCoordenadaX() << " Y = " << no1->getCoordenadaY() << endl;
        std::cout << "No2 X = " << no2->getCoordenadaX() << " Y = " << no2->getCoordenadaY() << endl;
    }
    float distancia = sqrt(pow(no1->getCoordenadaX() - no2->getCoordenadaX(), 2) + pow(no1->getCoordenadaY() - no2->getCoordenadaY(), 2));

    if (DEBUG)
        std::cout << "Distancia entre " << no1->getId() << " e " << no2->getId() << " = " << distancia << endl;
    return distancia;
}
float **criaMatrizDistancias(Grafo *grafo)
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
    return matriz;
}
/*
converte a linha do arquivo para (int, int) e chama a função de construção no grafo
se a linha so contiver um inteiro, a mesma representa o numero de nos do grafo (de acordo com a descrição do arquivo readme.txt)
*/
bool estaContido(Rota *rota1, Rota *rota2)
{
    if (rota1 == NULL || rota2 == NULL)
    {
        return false;
    }

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
void constroiGrafo(string linha, Grafo *grafo, int tipoArquivo)
{
    if (tipoArquivo == 1)
    {
        istringstream iss(linha);
        float num1, num2;
        int peso;
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
                std::cout << "O grafo possui " << num1 << " nos. " << endl;
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
        std::cout << "Arquivo '" << fileLocation << "' nao encontrado! " << endl;
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
        std::cout << "Arquivo '" << fileLocation << "' nao encontrado! " << endl;
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
            if (linha.find("EOF") != string::npos)
            {
                return 1;
            }
        }
        if (linha.find("No of trucks: ") != string::npos)
        {
            string noOfTrucksStr = linha.substr(linha.find("No of trucks: ") + 14);
            noOfTrucksStr = noOfTrucksStr.substr(0, noOfTrucksStr.find(","));
            int noOfTrucks = stoi(noOfTrucksStr);
            return noOfTrucks;
        }
    }
    return 0;
}
void controiGrafoTipo2(string fileLocation, Grafo *grafo)
{
    fstream file;
    file.open(fileLocation, ios::in);
    if (!file)
    {
        std::cout << "Arquivo '" << fileLocation << "' nao encontrado! " << endl;
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

        // int totalNos = 0;
        // while (linha.find("CAPACITY") != 0)
        // {
        //     getline(file, linha);
        //     if (linha.find("CAPACITY") != string::npos)
        //     {
        //         string dimension = linha.substr(linha.find(":") + 1);
        //         totalNos = stoi(dimension);
        //     }
        // }
        // grafo->setTotalNos(totalNos);

        std::cout << "Características do arquivo: " << endl;
        while (linha.find("NODE_COORD_SECTION") != 0)
        {
            getline(file, linha);
            std::cout << linha << endl;
        }
        getline(file, linha);
        int lastId = 0;
        while (linha.find("DEMAND_SECTION") != 0)
        {
            istringstream iss(linha);
            int id;
            float x, y;
            iss >> id;
            iss >> x;
            iss >> y;
            lastId = id;
            grafo->AddNoCoord(id, x, y);
            getline(file, linha);
        }
        grafo->setTotalNos(lastId + 1);

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
            std::cout << "Adicionando aresta entre " << noNav->getId() << " e " << noNav2->getId() << endl;
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
        std::cout << "Pegando o primeiro elemento" << endl;
    No *noNav1 = rota1->getPrimeiroElemento();
    No *noNav2 = rota2->getPrimeiroElemento();

    if (DEBUG)
        std::cout << "Verificando se a segunda rota inclui a primeira" << endl;
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
        std::cout << "Verificando se a primeira rota inclui a segunda" << endl;
        std::cout << "Rota 2: ";
        rota2->imprime();
    }
    while (noNav2 != NULL)
    {
        if (DEBUG)
        {
            std::cout << "noNav2->getId() = " << noNav2->getId() << endl;
            std::cout << "rota1->possuiNo(noNav2->getId()) = " << rota1->possuiNo(noNav2->getId()) << endl;
        }
        if (!rota1->possuiNo(noNav2->getId()))
        {

            return false;
        }
        noNav2 = noNav2->getProxNo();
    }
    if (DEBUG)
    {
        std::cout << "Ta retornando que as rotas sao equivalentes!" << endl;
        std::cout << "Rota 1: ";
        rota1->imprime();
        std::cout << "Rota 2: ";
        rota2->imprime();
    }
    return true;
}

int randomRange(int min, int max)
{
    // Generate a random number between min and max (inclusive)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(min, max);
    return dis(gen);
}

bool avaliarCapacidadeMaximaRota(Rota *rota, Rota *novaRota)
{
    int capMax = rota->getCapacidade();
    int capacidadeAtual = 0;

    return (capacidadeAtual <= capMax);
}

bool compararDistancias(const std::pair<float, No *> &a, const std::pair<float, No *> &b)
{
    return a.first < b.first;
}

No *selecionaNoAleatorioEntreTopN(No *novoNo, Rota *rotaReferencia, float percentual, Rota *rotaOrigem)
{
    std::vector<std::pair<float, No *>> distancias;

    // Calcula as distâncias de novoNo para todos os nós em rotaReferencia
    for (No *no = rotaReferencia->getPrimeiroElemento(); no != nullptr; no = no->getProxNo())
    {
        if (!rotaOrigem->possuiNo(no->getId()) && novoNo != no)
        {
            float distancia = calcularDistanciaNos(novoNo, no);
            distancias.push_back(std::make_pair(distancia, no));
        }
    }

    // Ordena os nós com base na distância
    std::sort(distancias.begin(), distancias.end(), compararDistancias);

    //* imprime distancias
    // std::cout << "Distancias: " << endl;
    // for (int i = 0; i < distancias.size(); i++)
    // {
    //     std::cout << distancias[i].first << endl;
    // }

    No *noSelecionado = distancias[percentual * (distancias.size() - 1)].second;

    return noSelecionado;
}
No *selecionaNoMenorDistancia(Rota *rotaOrigem, Rota *rotaReferencia, ListaNos *excluidos = NULL)
{
    float menorDistancia = 9999999;
    No *noMaisProximo = NULL;
    No *noNav = rotaReferencia->getPrimeiroElemento();
    while (noNav != NULL)
    {
        if (rotaOrigem->possuiNo(noNav->getId()))
        {
            if (DEBUG)
                std::cout << "O no " << noNav->getId() << " ja esta na rota" << endl;
            noNav = noNav->getProxNo();
            continue;
        }
        if (excluidos != NULL && excluidos->possuiElemento(noNav->getId()))
        {
            noNav = noNav->getProxNo();
            continue;
        }

        float distancia = calcularDistanciaNos(rotaOrigem->getUltimoElemento(), noNav);

        if (DEBUG)
            std::cout << "Distancia entre = " << distancia << endl;
        if (distancia < menorDistancia)
        {
            menorDistancia = distancia;
            noMaisProximo = noNav;
        }
        noNav = noNav->getProxNo();
    }
    return noMaisProximo;
}
Rota *mesclarRotasRand2(Rota *rota1, Rota *rota2)
{
    if (DEBUG)
    {
        std::cout << "Mesclando rotas: " << endl;
        std::cout << "Rota 1: ";
        rota1->imprime();
        std::cout << "Rota 2: ";
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
        std::cout << "Somando rotas: " << endl;
        rotaReferencia->imprime();
        std::cout << "Mesclando rotas: " << endl;
    }
    No *rotaNav = rota1->getPrimeiroElemento();
    // No *noNav2 = rota2->getPrimeiroElemento();

    while (!rotasSaoEquivalentes(novaRota, rotaReferencia))
    {
        ListaNos *selecionados = new ListaNos();
        No *noSelecionado1 = selecionaNoMenorDistancia(novaRota, rotaReferencia);

        // std::cout << "parou aqui 2" << endl;
        for (int i = 0; i < 20; i++)
        {
            No *noSelecionado2 = selecionaNoMenorDistancia(novaRota, rotaReferencia, selecionados);
            if (noSelecionado2 != NULL)
            {
                selecionados->AddElemento(noSelecionado2->getId(), noSelecionado2->getPeso(), noSelecionado2->getCoordenadaX(), noSelecionado2->getCoordenadaY());
            }
        }
        // selecionados->AddElemento(noSelecionado1->getId(), noSelecionado1->getPeso(), noSelecionado1->getCoordenadaX(), noSelecionado1->getCoordenadaY());

        // noSelecionado1 = selecionaNoMenorDistancia(novaRota, rotaReferencia, selecionados);

        //* escolhe um dos nos aleatoriamente para adiciona-lo a rota
        // std::cout << "parou aqui 1" << endl;
        int randNum = randomRange(0, selecionados->getNElementos() - 1);
        novaRota->AddElemento(selecionados->getElemento(randNum)->getId(), selecionados->getElemento(randNum)->getPeso(), selecionados->getElemento(randNum)->getCoordenadaX(), selecionados->getElemento(randNum)->getCoordenadaY());

        // novaRota->AddElemento(noMaisProximo->getId(), noMaisProximo->getPeso(), noMaisProximo->getCoordenadaX(), noMaisProximo->getCoordenadaY());
    }
    // novaRota->AddElemento(1, 0, 0, 0);
    novaRota->AddElemento(
        rota1->getPrimeiroElemento()->getId(),   //* tem que ser o 1
        rota1->getPrimeiroElemento()->getPeso(), //* tem que ser 0
        rota1->getPrimeiroElemento()->getCoordenadaX(),
        rota1->getPrimeiroElemento()->getCoordenadaY());
    if (DEBUG)
    {
        std::cout << "Rota mesclada com distancia " << novaRota->getDistanciaTotal() << endl;
        novaRota->imprime();
    }
    // delete rotaReferencia;

    return novaRota;
}
Rota *mesclarRotas(Rota *rota1, Rota *rota2, int numeroIteracoes = mesclarRotasIteracoes)
{
    if (numeroIteracoes == 1)
    {
        // std::cout << "Mesclar rotas simples" << endl;
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
            std::cout << "Somando rotas: " << endl;
            rotaReferencia->imprime();
            std::cout << "Mesclando rotas: " << endl;
        }
        No *noNav1 = rota1->getPrimeiroElemento();
        No *noNav2 = rota2->getPrimeiroElemento();

        while (!rotasSaoEquivalentes(novaRota, rotaReferencia))
        {
            if (DEBUG)
                std::cout << "As rotas não são equivalentes!" << endl;
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
                        std::cout << "O no " << noNav1->getId() << " ja esta na rota" << endl;
                    noNav1 = noNav1->getProxNo();
                    continue;
                }
                // float distancia = calcularDistanciaNos(novaRota->getUltimoElemento(), noNav1);
                float distancia = matrizDistancias[novaRota->getUltimoElemento()->getId()][noNav1->getId()];
                // (novaRota->getUltimoElemento(), noNav1);

                if (DEBUG)
                    std::cout << "Distancia entre = " << distancia << endl;
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
                        std::cout << "O no " << noNav2->getId() << " ja esta na rota" << endl;
                    noNav2 = noNav2->getProxNo();
                    continue;
                }
                float distancia = matrizDistancias[novaRota->getUltimoElemento()->getId()][noNav2->getId()];

                // float distancia = calcularDistanciaNos(novaRota->getUltimoElemento(), noNav2);
                if (DEBUG)
                    std::cout << "Distancia entre" << distancia << endl;
                if (distancia < menorDistancia)
                {
                    menorDistancia = distancia;
                    noMaisProximo = noNav2;
                }
                noNav2 = noNav2->getProxNo();
            }

            if (DEBUG)
            {
                std::cout << "Fim de procura pelo no mais proximo" << endl;
                std::cout << "nova rota: ";
                novaRota->imprime();
                std::cout << "rota referencia: ";
                rotaReferencia->imprime();
                std::cout << "Menor distancia: " << menorDistancia << endl;
                std::cout << "No mais proximo: " << noMaisProximo->getId() << endl;
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
            std::cout << "Rota mesclada com distancia " << novaRota->getDistanciaTotal() << endl;
            novaRota->imprime();
        }
        return novaRota;
    }
    else
    {

        Rota *melhorRota = NULL;
        float melhorDistancia = 9999999999;
        for (int i = 0; i < mesclarRotasIteracoes; i++)
        {
            Rota *novaRota = mesclarRotasRand2(rota1, rota2);
            std::cout << "retornou a nova rota = " << (novaRota == NULL) << endl;
            if (novaRota->getDistanciaTotal() < melhorDistancia)
            {
                melhorDistancia = novaRota->getDistanciaTotal();
                melhorRota = novaRota;
                // std::cout << "Melhor distancia = " << melhorDistancia << endl;
            }
        }
        melhorRota->imprime();
        return melhorRota;
    }
}
float calcularDistanciaRotas(Rota *rota1, Rota *rota2)
{
    return sqrt(pow(rota1->getAvgPosX() - rota2->getAvgPosX(), 2) + pow(rota1->getAvgPosY() - rota2->getAvgPosY(), 2));
}
ListaJuncao *criaListaJuncao(ListaRotas *listaRotas)
{
    ListaJuncao *listaJuncao = new ListaJuncao();
    Rota *rotaNav = listaRotas->getPrimeiroElemento();
    int rota1I = 0;
    int rota2I = 0;
    while (rotaNav != NULL)
    {
        Rota *rotaNav2 = listaRotas->getPrimeiroElemento();
        rota1I++;
        rota2I = 0;
        while (rotaNav2 != NULL)
        {
            rota2I++;
            if (rota1I == rota2I)
            {
                rota2I = 0;
                break;
            }
            float distancia = calcularDistanciaRotas(rotaNav, rotaNav2);
            listaJuncao->AddElemento(rotaNav, rotaNav2, distancia, NULL);
            rotaNav2 = rotaNav2->getProxElemento();
        }
        rotaNav = rotaNav->getProxElemento();
    }
    return listaJuncao;
}

ListaJuncao *listaJuncaoCarregada = new ListaJuncao();
void limpaListaJuncaoCarregada()
{
    std::vector<Juncao *> elementosParaRemover;
    Juncao *juncaoNav1 = listaJuncaoCarregada->getPrimeiroElemento();
    Juncao *juncaoNav2;

    while (juncaoNav1 != NULL)
    {
        juncaoNav2 = listaJuncaoCarregada->getPrimeiroElemento();
        while (juncaoNav2 != NULL)
        {
            if (juncaoNav1 != juncaoNav2 && juncaoNav1->mesclada != NULL && estaContido(juncaoNav2->mesclada, juncaoNav1->mesclada))
            {
                elementosParaRemover.push_back(juncaoNav2);
            }
            juncaoNav2 = juncaoNav2->getProxJuncao();
        }
        juncaoNav1 = juncaoNav1->getProxJuncao();
    }

    for (Juncao *juncao : elementosParaRemover)
    {
        listaJuncaoCarregada->RemoveElementoBaseadoNaRota(juncao);
    }
}

void esvaziaListaJuncaoCarregada()
{
    Juncao *juncaoNav = listaJuncaoCarregada->getPrimeiroElemento();
    Juncao *proxJuncao;

    while (juncaoNav != NULL)
    {
        proxJuncao = juncaoNav->getProxJuncao();
        listaJuncaoCarregada->RemoveElementoBaseadoNaRota(juncaoNav);
        juncaoNav = proxJuncao;
    }
}
ListaEconomias *calculaEconomias(ListaRotas *listaRotas, ListaEconomias *economiasAnteriores, float alfa = -1)
{
    Rota *rotaNav1 = listaRotas->getPrimeiroElemento();
    Rota *rotaNav2 = listaRotas->getPrimeiroElemento();

    if (listaJuncaoCarregada->getNElementos() > 5000)
    {
        esvaziaListaJuncaoCarregada();
    }
    if (DEBUG)
        std::cout << "Calcular economias" << endl;
    ListaEconomias *economias = new ListaEconomias(listaRotas->getCapacidade());

    ListaJuncao *listaJuncao = criaListaJuncao(listaRotas);

    //* para os elementos da lista de rotas que ja estiverem carregados na lista juncao, nao calcular novamente
    if (listaJuncaoCarregada != NULL)
    {
        std::vector<Juncao *> elementosParaRemover;
        Juncao *juncaoNav = listaJuncaoCarregada->getPrimeiroElemento();
        // Juncao *juncaoNav = nullptr;

        // std::cout << "listaJuncaoCarregada->getNElementos() " << listaJuncaoCarregada->getNElementos() << endl;
        //! nao é aqui que ta travando
        // std::cout << "juncaoNav == NULL " << (juncaoNav == NULL) << endl;

        while (juncaoNav != NULL)
        {
            Juncao *prevJuncaoNav = juncaoNav;
            juncaoNav = juncaoNav->getProxJuncao();
            listaJuncao->RemoveElementoBaseadoNaRota(prevJuncaoNav);
        }
    }
    // std::cout << "Numero de economias a serem carregadas: " << listaJuncao->getNElementos() << endl;

    // for (int i = 0; i < listaJuncao->getNElementos(); i++)
    Juncao *juncaoNav = listaJuncao->getPrimeiroElemento();
    // std::cout << "listaJuncao->getNElementos()"<< listaJuncao->getNElementos() << endl;

    //! ta travando aqui
    // std::cout << "parou aqui 1" << endl;
    int iteracao = 0;
    const int maxIteracoes = listaJuncao->getNElementos();
    while (juncaoNav != NULL)
    {
        iteracao++;
        if (iteracao > maxIteracoes)
        {
            // std::cout << "\033[31m===============================================\033[0m" << std::endl;
            // std::cout << "\033[31mIteracao maior que maxIteracoes\033[0m" << std::endl;
            // std::cout << "\033[31m===============================================\033[0m" << std::endl;
            break;
        }
        Rota *rota1 = juncaoNav->rota1;
        Rota *rota2 = juncaoNav->rota2;

        if (rota1->getCapacidadeAtual() + rota2->getCapacidadeAtual() > rota1->getCapacidade())
        {

            listaJuncaoCarregada->AddElemento(rota1,
                                              rota2,
                                              0,
                                              NULL);
            // juncaoNav = juncaoNav->getProxJuncao();
        }
        else
        {
            Rota *novaRota = mesclarRotas(rota1, rota2);
            listaJuncaoCarregada->AddElemento(rota1,
                                              rota2,
                                              0,
                                              novaRota);
        }
        juncaoNav = juncaoNav->getProxJuncao();
    }
    // std::cout << "Saiu do loop de junções " <<endl;

    // std::cout << "Carregando economias " << endl;

    rotaNav1 = listaRotas->getPrimeiroElemento();
    rotaNav2 = listaRotas->getPrimeiroElemento();
    int i = 0;
    int rotaNav1I = 0;
    int rotaNav2I = 0;

    while (rotaNav1 != NULL)
    {
        rotaNav1I++;
        rotaNav2 = listaRotas->getPrimeiroElemento();
        while (rotaNav2 != NULL)
        {
            rotaNav2I++;

            if (rotaNav1I == rotaNav2I)
            {
                rotaNav2I = 0;
                rotaNav2 = listaRotas->getPrimeiroElemento();
                break;
            }
            Juncao *juncao = listaJuncaoCarregada->getJuncaoDasRotas(rotaNav1, rotaNav2);
            if (juncao->mesclada != NULL)
            {
                float valorEconomizado = rotaNav1->getDistanciaTotal() + rotaNav2->getDistanciaTotal() - juncao->mesclada->getDistanciaTotal();
                economias->AddElemento(new Economia(
                    juncao->mesclada,
                    valorEconomizado));
            }
            i++;
            rotaNav2 = rotaNav2->getProxElemento();
        }
        rotaNav1 = rotaNav1->getProxElemento();
    }
    // std::cout << "parou aqui 4" << endl;

    //* limpar lista de juncao carregada

    // std::cout << "listaJuncaoCarregada->getNElementos() depois da limpa: " << listaJuncaoCarregada->getNElementos() << endl;
    //  for (int i = 0; i < listaJuncaoCarregada->getNElementos(); i++)
    //  {
    //      if (
    //          !listaRotas->contains(listaJuncaoCarregada->getJuncao(i)->mesclada)

    //         )
    //     {
    //         if (listaJuncaoCarregada->getJuncao(i)->mesclada != NULL)
    //         {
    //             float valorEconomizado = listaJuncaoCarregada->getJuncao(i)->rota1->getDistanciaTotal() + listaJuncaoCarregada->getJuncao(i)->rota2->getDistanciaTotal() - listaJuncaoCarregada->getJuncao(i)->mesclada->getDistanciaTotal();

    //             economias->AddElemento(new Economia(
    //                 listaJuncaoCarregada->getJuncao(i)->mesclada,
    //                 valorEconomizado));
    //         }
    //     }
    // }
    // std::cout << "Numero de economias: " << economias->getNElementos() << endl;
    // std::cout << "deveria ser: " << i << endl;
    // economias->imprime();

    // limpaListaJuncaoCarregada();
    return economias;

    // int offset = 0;
    // if (alfa == -1 || true)
    // {
    //     ListaJuncao *juncoesExcluidas = new ListaJuncao();
    //     ListaJuncao *juncoesAdicionadas = new ListaJuncao();

//============================================================//    //         if (juncaoMaisProxima == NULL)
    //         {
    //             break;
    //         }
    //         Rota *rota1 = juncaoMaisProxima->rota1;
    //         Rota *rota2 = juncaoMaisProxima->rota2;
//============================================================//


    //         if (juncaoMaisProxima == NULL)
    //         if (rota1->getCapacidadeAtual() + rota2->getCapacidadeAtual() > rota1->getCapacidade())
    //         {
    //             juncoesExcluidas->AddElemento(
    //                 juncaoMaisProxima->rota1,
    //                 juncaoMaisProxima->rota2,
    //                 juncaoMaisProxima->distancia,
    //                 NULL);
    //             offset++;
    //             continue;
    //         }
    //         else
    //         {
    //             Rota *novaRota = mesclarRotas(rota1, rota2);
    //             float valorEconomizado = rota1->getDistanciaTotal() + rota2->getDistanciaTotal() - novaRota->getDistanciaTotal();
    //             juncoesAdicionadas->AddElemento(juncaoMaisProxima->rota1,
    //                                             juncaoMaisProxima->rota2,
    //                                             juncaoMaisProxima->distancia,
    //                                             novaRota);
    //             juncoesExcluidas->AddElemento(juncaoMaisProxima->rota1,
    //                                           juncaoMaisProxima->rota2,
    //                                           juncaoMaisProxima->distancia,
    //                                           novaRota);
    //             economias->AddElemento(new Economia(novaRota, valorEconomizado));
    //         }
    //     }

    //     return economias;
    // }

    // return economias;
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
    outdata.close();
}

// float **criaMatrizDistancias(Grafo *grafo)
// {
//     float **matriz = new float *[grafo->getTotalNos()];
//     for (int i = 0; i < grafo->getTotalNos(); i++)
//     {
//         matriz[i] = new float[grafo->getTotalNos()];
//     }

//     for (int i = 0; i < grafo->getTotalNos(); i++)
//     {
//         for (int j = 0; j < grafo->getTotalNos(); j++)
//         {
//             matriz[i][j] = 0;
//         }
//     }

//     No *noNav = grafo->getRaizGrafo();
//     while (noNav != NULL)
//     {
//         No *noNav2 = grafo->getRaizGrafo();
//         while (noNav2 != NULL)
//         {
//             if (noNav->getId() != noNav2->getId())
//             {
//                 matriz[noNav->getId() - 1][noNav2->getId() - 1] = calcularDistanciaNos(noNav, noNav2);
//             }
//             noNav2 = noNav2->getProxNo();
//         }
//         noNav = noNav->getProxNo();
//     }
//     return matriz;
// }
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

    // std::cout << "filePathName = > " << endl;
    // std::cout << filePathName << endl;
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

ListaRotas *algoritmoClarkeWright(Grafo *grafo, int capacidadeCaminhao, int quantidadeRotas, string testeName = "teste", float alfa = -1, bool ehSimples = false)
{

    if (alfa != -1 && alfa < 0 && alfa > 1)
    {
        std::cout << "Alfa deve ser um valor entre 0 e 1" << endl;
        exit(1);
        return NULL;
    }

    ListaRotas *rotas = new ListaRotas(capacidadeCaminhao);

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

            std::cout << "------------------------------------------------------" << endl;
            std::cout << "Adicionando no " << noNav->getId() << endl;
            std::cout << "nonav x = " << noNav->getCoordenadaX() << " y = " << noNav->getCoordenadaY() << endl;

            std::cout << "== Criando a rota ==" << endl;
        }

        Rota *newRota = new Rota(capacidadeCaminhao);

        if (DEBUG)
        {
            std::cout << "newRota == NULL: " << (newRota->getUltimoElemento() == NULL) << endl;
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
            std::cout << "newRota == NULL: " << (newRota->getUltimoElemento() == NULL) << endl;
            std::cout << "criado com sucesso" << endl;
        }

        noNav = noNav->getProxNo();
    }
    if (DEBUG)
    {

        std::cout << "fim do loop " << endl;

        std::cout << "Rotas criadas: " << endl;
        Rota *rotaNav = rotas->getPrimeiroElemento();
        while (rotaNav != NULL)
        {
            rotaNav->imprime();
            rotaNav = rotaNav->getProxElemento();
        }
    }

    if (DEBUG)
    {
        std::cout << "==============================================" << endl;
        std::cout << "========== < Calculando economias > ==========" << endl;
        std::cout << "==============================================" << endl;
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
        std::cout << "rotas->getNElementos() = " << rotas->getNElementos() << endl;
    // while (rotas->getNElementos() >= 10)
    // ListaEconomias *economias = NULL;
    ListaEconomias *economias;
    while (rotas->getNElementos() >= quantidadeRotas)
    {

        //! o loop infinito ta no calcula economias
        economias = calculaEconomias(rotas, economias);

        iteracao++;
        if (ehSimples && iteracao % 10 == 0)
        {
            std::cout << "[" << iteracao << "] quantidade de economias = " << economias->getNElementos() << endl;
        }
        if (economias->getNElementos() == 0)
        {
            if (DEBUG)
            {
                std::cout << "Não há mais economias a serem feitas!" << endl;
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
            std::cout << "economias: " << endl;
            economias->imprime();
            std::cout << "Rotas antes do merge: " << endl;
            rotas->imprime();
        }

        //! aqui mudar para randomizado
        // incluiMergeNasRotas(economias->getPrimeiroElemento()->getRota(), rotas);

        // std::cout << "Eu to certo" << endl;
        if (alfa != -1)
        {
            int k = randomRange(0, alfa * (economias->getNElementos() - 2));
            Rota *rotaToMerge = economias->getElemento(k)->cloneRota();
            incluiMergeNasRotas(rotaToMerge, rotas);
        }
        else
            incluiMergeNasRotas(economias->getMaiorEconomia()->cloneRota(), rotas);

        // delete economias;
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
        if (DEBUG)
        {

            std::cout << "Rotas depois do merge: " << endl;
            rotas->imprime();
        }
    }

    /*

    ! estou removendo todos os deletes em desespero


    */

    // generateGraphvizFile(grafo, rotas, "./out/" + testeName + "/graphviz.txt");
    // geraLogDasRotas(rotas, "./out/" + testeName + "/LogsRotas.txt");
    // outdata.open("./out/" + testeName + "/LogsRotas.txt", std::ios_base::app);
    // std::cout << "O custo total foi de: " << calculateCustoTotal(rotas) << endl;
    // outdata.close();

    return rotas;
}
struct ResultadoClarkeWrightRandomizado
{
    float custoMedio;
    float alfa;
    ListaRotas *melhorResultado;
};

ResultadoClarkeWrightRandomizado ClarkeWrightRandomizado(Grafo *grafo, string nomeTeste, int capacidade, int nRotas, float alfa, int nIteracoes, ListaRotas *setMelhorResultado = NULL)
{

    ListaRotas *melhorResultado;
    if (setMelhorResultado != NULL)
    {
        melhorResultado = setMelhorResultado;
    }
    else
    {
        melhorResultado = new ListaRotas(capacidade);
    }
    float somaCusto = 0;

    float melhorCusto = 99999999;
    for (int i = 0; i < nIteracoes; i++)
    {
        // auto start = chrono::system_clock::now(); //! inicio de codigo para contagem de tempo de execução

        // limpaListaJuncaoCarregada();

        // if(nIteracoes > 1)
        //     esvaziaListaJuncaoCarregada();
        // listaJuncaoCarregada = new ListaJuncao();
        if (nIteracoes > 1)
        {
            std::cout << "iteracaoRandomizado: " << i << " / " << nIteracoes << endl;

            // std::cout << "listaJuncaoCarregada->getNElementos(): " << listaJuncaoCarregada->getNElementos() << endl;
        }
        ListaRotas *resultado = algoritmoClarkeWright(grafo, capacidade, nRotas, nomeTeste, alfa);
        float custo = calculateCustoTotal(resultado);

        somaCusto += custo;

        if (custo < melhorCusto)
        {
            melhorCusto = custo;
            melhorResultado = resultado;
        }
        else
        {
            // delete resultado;
        }
        // //! fim de cogio de contagem de tempo de execução
        // auto end = chrono::system_clock::now();
        // chrono::duration<double> elapsed_seconds = end - start;
        // time_t end_time = chrono::system_clock::to_time_t(end);
        // std::cout << "tempo de execução: " << BOLDGREEN << elapsed_seconds.count() << " s" << RESET
        //           << endl;
        // //!=================================================

        // if (nIteracoes > 1)
        //     std::cout << "Custo da iteracao " << i << " com alfa: " << alfa << " = " << custo << endl;
    }
    if (nIteracoes > 1)
    {

        std::cout << "Menor custo: " << BOLDGREEN << melhorCusto << RESET << endl;
    }

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

ClarkeWrightReativoResultado ClarkeWrightReativo(Grafo *grafo, string nomeTeste, int capacidade, int nRotas, int nIteracoes)
{

    SeletorAlfa *seletorAlfa = new SeletorAlfa();
    float menorCusto = 99999999;
    ListaRotas *melhorResultado = new ListaRotas(capacidade);
    for (int iteracao = 0; iteracao < nIteracoes; iteracao++)
    {
        esvaziaListaJuncaoCarregada();
        // listaJuncaoCarregada = new ListaJuncao();
        if (iteracao % 10 == 0)
        {
            std::cout << "iteracao reativo: " << iteracao << " / " << nIteracoes << endl;
        }
        int alfaSelecionadoIndex = seletorAlfa->selecionarAlfaIndex();
        float alfaSelecionado = seletorAlfa->getAlfa(alfaSelecionadoIndex);
        //* nessa implementação, selecionamos um alfa e rodamos o algorítimo randomizado com o mesmo alfa 30 vezes
        //* e pegamos a media dos custos para atualizar a probabilidade do alfa

        // ResultadoClarkeWrightRandomizado resultado = ClarkeWrightRandomizado(grafo, nomeTeste, capacidade, nRotas, alfaSelecionado, 1, new ListaRotas(capacidade));

        ResultadoClarkeWrightRandomizado resultado = ClarkeWrightRandomizado(grafo, nomeTeste, capacidade, nRotas, alfaSelecionado, repeticoesMediaAdaptativo, new ListaRotas(capacidade));

        if (calculateCustoTotal(resultado.melhorResultado) < menorCusto)
        {
            if (false) //! mudar para true caso queira gerar os graphviz e os logs de todas as melhores rotas
            {
                ofstream outdata;                                      // outdata is like cin
                outdata.open("./out/" + nomeTeste + "/LogsAlfas.txt"); // opens the file
                outdata << " ========================================================= " << endl;
                outdata << "iteracao: " << iteracao << endl;
                outdata << "Resultado da iteracao: " << resultado.custoMedio << endl;
                outdata << "Melhor resultado da iteracao: " << calculateCustoTotal(resultado.melhorResultado) << endl;
                for (int i = 0; i < seletorAlfa->getNAlfas(); i++)
                {
                    outdata << "[ " << seletorAlfa->getAlfa(i) << " ] probabilidade = " << seletorAlfa->getProbabilidade(i) << endl;
                }
                outdata.close();
                generateGraphvizFile(grafo, melhorResultado, "./out/" + nomeTeste + "/Graphviz" + std::to_string(iteracao) + ".txt");
                geraLogDasRotas(melhorResultado, "./out/" + nomeTeste + "/LogsRotas" + std::to_string(iteracao) + ".txt");
            }
            menorCusto = calculateCustoTotal(resultado.melhorResultado);
            melhorResultado = resultado.melhorResultado;
        }
        seletorAlfa->salvarResultado(alfaSelecionadoIndex, resultado.custoMedio);
        if (iteracao % 10 == 0)
        {
            seletorAlfa->atualizarProbabilidade();
            seletorAlfa->imprimir();
        }
        // seletorAlfa->atualizarProbabilidade(alfaSelecionadoIndex, resultado.custoMedio);
        if (iteracao % 500 == 0 && false)
        {
            ofstream outdata;                                      // outdata is like cin
            outdata.open("./out/" + nomeTeste + "/LogsAlfas.txt"); // opens the file
            outdata << " ========================================================= " << endl;
            outdata << "iteracao: " << iteracao << endl;
            outdata << "Resultado da iteracao: " << resultado.custoMedio << endl;
            outdata << "Melhor resultado da iteracao: " << calculateCustoTotal(resultado.melhorResultado) << endl;
            for (int i = 0; i < seletorAlfa->getNAlfas(); i++)
            {
                outdata << "[ " << seletorAlfa->getAlfa(i) << " ] probabilidade = " << seletorAlfa->getProbabilidade(i) << endl;
            }
            outdata.close();
            generateGraphvizFile(grafo, melhorResultado, "./out/" + nomeTeste + "/Graphviz" + std::to_string(iteracao) + ".txt");
            geraLogDasRotas(melhorResultado, "./out/" + nomeTeste + "/LogsRotas" + std::to_string(iteracao) + ".txt");
        }
    }
    // generateGraphvizFile(grafo, melhorResultado, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
    // geraLogDasRotas(melhorResultado, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");
    ClarkeWrightReativoResultado resultado;
    resultado.melhorResultado = melhorResultado;
    resultado.seletorAlfa = seletorAlfa;
    return resultado;
}

void menuOpcoes()
{
    std::cout << "Escolha uma opcao: " << endl;
    std::cout << "[ 0 ] cancelar " << endl;
    std::cout << "[ 1 ] nos diretamente conectados " << endl;
    std::cout << "[ 2 ] fecho transitivo direto " << endl;
    std::cout << "[ 3 ] fecho transitivo indireto " << endl;
    std::cout << "[ 4 ] arvore dada pela ordem de caminhamento em profundidade " << endl;
    std::cout << "[ 5 ] arvore dada pela ordem de caminhamento em profundidade com DreampufFile " << endl;
    std::cout << "[ 6 ] arvore minima kruskal " << endl;
}

string selecionarArquivo(bool grafoPonderado)
{
    //! sistema de seleção de arquivo
    FileMananger fileMananger;

    std::cout << "Selecione o arquivo para leitura: " << endl;
    fileMananger.ListAvailableFiles(grafoPonderado);
    int selectedFileIndex = -1;
    cin >> selectedFileIndex;
    string selectedFileName = fileMananger.GetFileNameByIndex(selectedFileIndex);
    if (selectedFileName.compare("") == 0)
    {
        std::cout << "índice de arquivo invalido!" << endl;
        return "";
    }
    std::cout << "Arquivo selecionado: " << selectedFileName << endl;
    return selectedFileName;
    //! fim do sistema de seleção de arquivo
}

void optimizeRota(ListaRotas *listaRotas)
{
    int i = 0;
    Rota *rotaNav = listaRotas->getPrimeiroElemento();
    while (rotaNav != NULL)
    {
        std::cout << "Otimizando rota " << i << endl;
        rotaNav = mesclarRotas(rotaNav, rotaNav, 1000);
        rotaNav = rotaNav->getProxElemento();
        i++;
    }
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
        // std::cout << "Argumentos passados: " << endl;
        // for (int i = 0; i < argc; i++)
        // {
        //     std::cout << "argv[" << i << "] = " << (argv[i]) << endl;
        // }
        arquivoEntrada = argv[1];
        arquivoSaida = argv[2];
        direcionado = (argv[3][0] == '1');
        ponderadoAresta = (argv[4][0] == '1');
        ponderadoNo = (argv[5][0] == '1');
        nomeTeste = argv[6];
        alfa = atof(argv[7]);

        // std::cout << "Arquivo de entrada: " << arquivoEntrada << endl;
        // std::cout << "Arquivo de saida: " << arquivoSaida << endl;
        // std::cout << "O grafo e direcionado? " << direcionado << endl;
        // std::cout << "O grafo e ponderado nas arestas? " << ponderadoAresta << endl;
        // std::cout << "O grafo e ponderado nos nos? " << ponderadoNo << endl;
        // std::cout << "Nome do teste: " << nomeTeste << endl;

        ofstream outdata; // outdata is like cin
        outdata.open("./out/" + nomeTeste + "/InformacoesGerais.txt");
        outdata << "Nome do teste: " << nomeTeste << endl;
        outdata << "Arquivo de entrada: " << arquivoEntrada << endl;
        outdata << "Argumentos passados: ";
        for (int i = 1; i < argc; i++)
        {
            outdata << (argv[i]) << " ";
        }
        outdata << endl;
        outdata.close();
    }
    else
    {
        tipoGrafo = 1;
        char grafoPonderado = ' ';
        do
        {
            std::cout << "O grafo e ponderado? (s/n)" << endl;
            cin >> grafoPonderado;
        } while (grafoPonderado != 's' && grafoPonderado != 'n');
        int ponderadoId = 0;
        if (grafoPonderado == 's')
        {
            do
            {
                std::cout << "Qual o tipo de ponderacao?" << endl;
                std::cout << "1: ponderado na aresta" << endl;
                std::cout << "2: ponderado no vertice" << endl;
                // std::cout << "3: ponderado nos dois" << endl;
                cin >> ponderadoId;
            } while (ponderadoId < 1 || ponderadoId > 2);
        }
        if (ponderadoId == 1)
        {
            ponderadoAresta = true;
        }
        else if (ponderadoId == 2)
        {
            ponderadoNo = true;
        }
        // else if (ponderadoId == 3)
        // {
        //     std::cout << "Grafos ponderados nos vértices e arestas ainda não são suportados!" << endl;
        //     return 1;
        // }

        if (ponderadoId != 0)
        {
            arquivoEntrada = "./files/grafosPonderados/" + selecionarArquivo(true);
        }
        else
        {
            arquivoEntrada = "./files/grafosNaoPonderados/" + selecionarArquivo(false);
        }

        char direcionadoChar = ' ';
        do
        {
            std::cout << "O grafo e direcionado? (s/n) ";
            cin >> direcionadoChar;
        } while (direcionadoChar != 's' && direcionadoChar != 'n');
        direcionado = (direcionadoChar == 's');
    }

    while (tipoGrafo != 1 && tipoGrafo != 2)
    {
        std::cout << "tipo grafo == 2: " << (tipoGrafo == 2) << endl;
        std::cout << "Qual o tipo de arquivo do grafo grafo?" << endl;
        std::cout << "1: grafo primeira parte" << endl;
        std::cout << "2: grafo segunda parte" << endl;
        cin >> tipoGrafo;
    };

    FileMananger fileMananger;
    Grafo grafo(direcionado == 's', ponderadoAresta, ponderadoNo);
    ClarkeWrightReativoResultado resultadoReativo;
    ResultadoClarkeWrightRandomizado resultadoRandomizado;

    auto start = chrono::system_clock::now(); //! inicio de codigo para contagem de tempo de execução
    if (tipoGrafo == 2)
    {
        //* gera um grafo sem arestas, apenas com os nos e suas coordenadas
        controiGrafoTipo2(arquivoEntrada, &grafo);
        matrizDistancias = criaMatrizDistancias(&grafo);
        if (nomeTeste.find("Randomizado") != string::npos)
        {
            std::cout << BOLDGREEN << "Algoritmo Clarke-Wright Randomizado" << RESET << endl;
            int capacideCaminhao = getCapacidadeCaminhao(arquivoEntrada);
            ResultadoClarkeWrightRandomizado resultadoRandomizado = ClarkeWrightRandomizado(&grafo, nomeTeste, capacideCaminhao, getNoOfTrucks(arquivoEntrada), alfa, iteracoesRandomizado); //! MUDAR PARA 500

            generateGraphvizFile(&grafo, resultadoRandomizado.melhorResultado, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
            geraLogDasRotas(resultadoRandomizado.melhorResultado, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");

            std::cout << "O custo medio foi de: " << resultadoRandomizado.custoMedio << endl;
            std::cout << "O alfa foi de: " << resultadoRandomizado.alfa << endl;
            std::cout << "O melhor custo foi de: " << calculateCustoTotal(resultadoRandomizado.melhorResultado) << endl;
            std::cout << "O melhor resultado foi: " << endl;
            resultadoRandomizado.melhorResultado->imprime();

            float custoAnterior = calculateCustoTotal(resultadoRandomizado.melhorResultado);
            std::cout << "optimizando rotas..." << endl;

            ListaRotas *rotasOtimizadas = new ListaRotas(capacideCaminhao);
            Rota *rotaNav = resultadoRandomizado.melhorResultado->getPrimeiroElemento();
            while (rotaNav != NULL)
            {
                float melhorDistancia = 9999999;
                Rota *melhorRota = rotaNav;
                for (int i = 0; i < iteracoesParaMelhoraFinalDeRotas; i++)
                {
                    Rota *novaRota = mesclarRotasRand2(rotaNav, rotaNav);
                    if (novaRota->getDistanciaTotal() < melhorDistancia)
                    {
                        melhorDistancia = novaRota->getDistanciaTotal();
                        melhorRota = novaRota;
                        // std::cout << "Melhor distancia = " << melhorDistancia << endl;
                    }
                }
                rotasOtimizadas->AddElemento(melhorRota);
                rotaNav = rotaNav->getProxElemento();
            }

            std::cout << "fim de otimizacao de rota final" << endl;
            float custoFinal = calculateCustoTotal(rotasOtimizadas);
            std::cout << "A melhora foi de: " << BOLDGREEN << custoAnterior - custoFinal << RESET << endl;

            if (custoFinal < custoAnterior)
            {
                generateGraphvizFile(&grafo, rotasOtimizadas, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
                geraLogDasRotas(rotasOtimizadas, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");
            }
            else
            {
                generateGraphvizFile(&grafo, resultadoRandomizado.melhorResultado, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
                geraLogDasRotas(resultadoRandomizado.melhorResultado, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");
            }

            //! fim de cogio de contagem de tempo de execução
            auto end = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;
            time_t end_time = chrono::system_clock::to_time_t(end);
            std::cout << "tempo de execução: " << BOLDGREEN << elapsed_seconds.count() << " s" << RESET
                      << endl;
            //!=================================================

            ofstream outdata;
            outdata.open("./out/" + nomeTeste + "/Resultado.txt");
            // outdata << "O custo total foi de: " << calculateCustoTotal(resultadoRandomizado.melhorResultado) << endl;
            if (custoFinal < custoAnterior)
                outdata << "O custo total foi de:: " << custoFinal << endl;
            else
                outdata << "O custo total foi de:: " << custoAnterior << endl;
            outdata << "O tempo gasto foi de " << elapsed_seconds.count() << " s" << endl;
            outdata << "A o alfa foi de " << resultadoRandomizado.alfa << endl;

            outdata.close();
        }
        else if (nomeTeste.find("Reativo") != string::npos)
        {
            std::cout << BOLDGREEN << "Algoritmo Clarke-Wright Reativo" << RESET << endl;
            int capacideCaminhao = getCapacidadeCaminhao(arquivoEntrada);
            resultadoReativo = ClarkeWrightReativo(&grafo, nomeTeste, capacideCaminhao, getNoOfTrucks(arquivoEntrada), iteracoesReativo);
            std::cout << "O melhor resultado da iteração foi: " << endl;
            resultadoReativo.melhorResultado->imprime();

            //! fim de cogio de contagem de tempo de execução
            auto end = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;
            time_t end_time = chrono::system_clock::to_time_t(end);
            std::cout << "tempo de execução: " << BOLDGREEN << elapsed_seconds.count() << " s" << RESET
                      << endl;
            //!=================================================

            float custoAnterior = calculateCustoTotal(resultadoReativo.melhorResultado);
            std::cout << "optimizando rotas..." << endl;

            ListaRotas *rotasOtimizadas = new ListaRotas(capacideCaminhao);
            Rota *rotaNav = resultadoReativo.melhorResultado->getPrimeiroElemento();
            while (rotaNav != NULL)
            {
                float melhorDistancia = 9999999;
                Rota *melhorRota = rotaNav;
                for (int i = 0; i < iteracoesParaMelhoraFinalDeRotas; i++)
                {
                    Rota *novaRota = mesclarRotasRand2(rotaNav, rotaNav);
                    if (novaRota->getDistanciaTotal() < melhorDistancia)
                    {
                        melhorDistancia = novaRota->getDistanciaTotal();
                        melhorRota = novaRota;
                        // std::cout << "Melhor distancia = " << melhorDistancia << endl;
                    }
                }
                rotasOtimizadas->AddElemento(melhorRota);
                rotaNav = rotaNav->getProxElemento();
            }

            std::cout << "fim de otimizacao de rota final" << endl;
            float custoFinal = calculateCustoTotal(rotasOtimizadas);
            std::cout << "A melhora foi de: " << BOLDGREEN << custoAnterior - custoFinal << RESET << endl;

            if (custoFinal < custoAnterior)
            {
                generateGraphvizFile(&grafo, rotasOtimizadas, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
                geraLogDasRotas(rotasOtimizadas, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");
            }
            else
            {
                generateGraphvizFile(&grafo, resultadoReativo.melhorResultado, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
                geraLogDasRotas(resultadoReativo.melhorResultado, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");
            }

            // generateGraphvizFile(&grafo, resultadoReativo.melhorResultado, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
            // geraLogDasRotas(resultadoReativo.melhorResultado, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");

            ofstream outdata;
            outdata.open("./out/" + nomeTeste + "/Resultado.txt");
            // outdata << "O custo total foi de: " << custoAnterior << endl;
            // outdata << "O otimizado foi de: " << custoFinal << endl;
            if (custoFinal < custoAnterior)
                outdata << "O custo total foi de:: " << custoFinal << endl;
            else
                outdata << "O custo total foi de:: " << custoAnterior << endl;
            outdata << "O tempo gasto foi de " << elapsed_seconds.count() << " s" << endl;
            outdata << "Alfa final: " << endl;
            for (int i = 0; i < resultadoReativo.seletorAlfa->getNAlfas(); i++)
            {
                outdata << "[ " << resultadoReativo.seletorAlfa->getAlfa(i) << " ] probabilidade = " << resultadoReativo.seletorAlfa->getProbabilidade(i) << endl;
            }

            outdata.close();
        }
        else
        {
            std::cout << BOLDGREEN << "Algoritmo Clarke-Wright guloso simples" << RESET << endl;
            int capacideCaminhao = getCapacidadeCaminhao(arquivoEntrada);
            ListaRotas *resultado = algoritmoClarkeWright(&grafo, capacideCaminhao, getNoOfTrucks(arquivoEntrada), nomeTeste, -1, true);
            std::cout << "O melhor resultado da iteração foi: " << endl;
            resultado->imprime();

            float custoAnterior = calculateCustoTotal(resultado);
            std::cout << "optimizando rotas..." << endl;

            ListaRotas *rotasOtimizadas = new ListaRotas(capacideCaminhao);
            Rota *rotaNav = resultado->getPrimeiroElemento();
            while (rotaNav != NULL)
            {
                float melhorDistancia = 9999999;
                Rota *melhorRota = rotaNav;
                for (int i = 0; i < iteracoesParaMelhoraFinalDeRotas; i++)
                {
                    Rota *novaRota = mesclarRotasRand2(rotaNav, rotaNav);
                    if (novaRota->getDistanciaTotal() < melhorDistancia)
                    {
                        melhorDistancia = novaRota->getDistanciaTotal();
                        melhorRota = novaRota;
                        // std::cout << "Melhor distancia = " << melhorDistancia << endl;
                    }
                }
                rotasOtimizadas->AddElemento(melhorRota);
                rotaNav = rotaNav->getProxElemento();
            }

            std::cout << "fim de otimizacao de rota final" << endl;
            float custoFinal = calculateCustoTotal(rotasOtimizadas);
            std::cout << "A melhora foi de: " << BOLDGREEN << custoAnterior - custoFinal << RESET << endl;


            if (custoFinal < custoAnterior)
            {
                generateGraphvizFile(&grafo, rotasOtimizadas, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
                geraLogDasRotas(rotasOtimizadas, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");
            }
            else
            {
                generateGraphvizFile(&grafo, resultado, "./out/" + nomeTeste + "/MelhorGraphviz.txt");
                geraLogDasRotas(resultado, "./out/" + nomeTeste + "/MelhorLogsRotas.txt");
            }

            //! fim de cogio de contagem de tempo de execução
            auto end = chrono::system_clock::now();
            chrono::duration<double> elapsed_seconds = end - start;
            time_t end_time = chrono::system_clock::to_time_t(end);
            std::cout << "tempo de execução: " << BOLDGREEN << elapsed_seconds.count() << " s" << RESET
                      << endl;
            //!=================================================

            ofstream outdata;
            outdata.open("./out/" + nomeTeste + "/Resultado.txt");
            if (custoFinal < custoAnterior)
                outdata << "O custo total foi de:: " << custoFinal << endl;
            else
                outdata << "O custo total foi de:: " << custoAnterior << endl;
            outdata << "O tempo gasto foi de " << elapsed_seconds.count() << " s" << endl;


            outdata.close();
        }
        return 0;
    }
    else
    {
        fileMananger.Read(arquivoEntrada, &constroiGrafo, &grafo, tipoGrafo); //* le o arquivo chamando a função constroiGrafo a cada linha
        grafo.generateDreampufFile("grafo.txt");
    }

    // std::cout << "O grafo é: " << endl;
    // std::cout << "Ponderado nas arestas " << grafo.getPonderadoAresta() << endl;
    // std::cout << "Ponderado nos vertices " << grafo.getPonderadoVertice() << endl;

    int input;
    do
    {
        if (tipoGrafo == 2)
        {
            return 0;
        }
        std::cout << "\n\n\nDigite -1 para sair" << endl;
        std::cout << "Digite o id do no que vc deseja informacoes: ";
        cin >> input;
        if (input < 0)
        {
            continue;
        }

        auto start = chrono::system_clock::now(); //! inicio de codigo para contagem de tempo de execução

        No *noSelecionado = grafo.procuraId(input);

        if (noSelecionado == NULL)
        {
            std::cout << "O no selecionado nao esta no grafo! " << endl;
            continue;
        }
        std::cout << "O no selecionado esta no grafo." << endl;

        if (noSelecionado->getPrimeiraAresta() == NULL)
        {
            std::cout << "Não possui arestas conectadas!" << endl;
            continue;
        }

        int opcao = 0;
        menuOpcoes();
        cin >> opcao;
        switch (opcao)
        {
        case 1:
        {
            std::cout << "O no esta diretamente conectado aos nos:";
            Lista *conectado = grafo.getArestasNo(input);
            conectado->iterate([](int id)
                               { std::cout << id << " "; }); //* essa coisa esquisita é uma lambda function. É meio que um jeito de declarar uma função dentro de outra função em cpp.
            std::cout << endl;
        }
        break;
        case 2:
        {
            std::cout << "Fecho transitivo direto deste vértice: ";
            Lista *fechoTransitivoDireto = grafo.buscaProfundidade(input);
            fechoTransitivoDireto->iterate([](int id)
                                           { std::cout << id << ", "; });
            std::cout << endl;
        }
        break;
        case 3:
        {
            std::cout << "Fecho transitivo indireto deste vértice: ";
            if (direcionado == 's')
            {

                Grafo *grafoInvertido = grafo.inverteArestasDirecionadas();
                Lista *fechoTransitivoIndireto = grafoInvertido->buscaProfundidade(input);
                fechoTransitivoIndireto->iterate([](int id)
                                                 { std::cout << id << ", "; });
                std::cout << endl;
                grafoInvertido->generateDreampufFile("grafoInvertido.dat");
                // delete grafoInvertido;
            }
            else
            {
                std::cout << "operacao invalida por ser grafo não ordenado! " << endl;
            }
        }
        break;

        case 4:
        {
            std::cout << "Arvore dada pela ordem de caminhamento em profundidade: " << endl;
            grafo.arvoreProfundidade(input);
            std::cout << endl;
        }
        break;

        case 5:
        {
            std::cout << "Arvore dada pela ordem de caminhamento em profundidade com DreampufFile: " << endl;
            grafo.arvoreProfundidade(input, true);
            std::cout << endl;
        }
        break;
        case 6:
        {
            int vertice;
            Lista *vertices = new Lista();
            std::cout << "Selecione os vertices que deseja na arvore minima: " << endl;
            do
            {
                std::cout << "Digite o id do vertice: (digite -1 para sair)";
                cin >> vertice;
                if (vertice == -1)
                {
                    break;
                }
                if (grafo.procuraId(vertice) == NULL)
                {
                    std::cout << "O vertice selecionado nao esta no grafo! " << endl;
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
        std::cout << "\ntempo de execucao: " << BOLDGREEN << elapsed_seconds.count() << " s" << RESET
                  << endl;
        //!=================================================

    } while (input != -1);

    return 0;
}
