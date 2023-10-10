#define PESO_NAO_PONDERADO 0

#include "Grafo.h"

Grafo::Grafo(bool direcionado, int ponderadoId)
{
    /*
    ponderado:
        0: não ponderado
        1: ponderado na aresta
        2: ponderado no vértice
        3: ponderado nos dois
    */
    raizGrafo = NULL;
    this->ponderadoAresta = ponderadoId == 1 || ponderadoId == 3;
    this->ponderadoVertice = ponderadoId == 2 || ponderadoId == 3;
    this->direcionado = direcionado;
}

Grafo::~Grafo()
{
}

void Grafo::AddNoArestaAux(int no)
{
    No *noAdd = procuraId(no);
    if (noAdd == NULL)
    { // no nao existe, logo insere ao fim da lista de nos
        No *listNos = raizGrafo;
        //* navega ate o ultimo no da lista
        while (listNos->getProxNo() != NULL)
        {
            listNos = listNos->getProxNo();
        }
        No *novoNo = new No(no);
        listNos->setProxNo(novoNo); //* adiciona o no na lista de nos
    }
}
void Grafo::AddNoArestaAux(int no1, int no2)
{
    No *noAddAresta = procuraId(no1);
    if (noAddAresta == NULL)
    { // no nao existe, logo insere ao fim da lista de nos
        No *listNos = raizGrafo;
        //* navega ate o ultimo no da lista
        while (listNos->getProxNo() != NULL)
        {
            listNos = listNos->getProxNo();
        }
        No *novoNo = new No(no1);
        listNos->setProxNo(novoNo);                                                //* adiciona o no na lista de nos
        Aresta *ultimaAresta = novoNo->setPrimeiraAresta(no2, PESO_NAO_PONDERADO); //* cria a primeira aresta apontando para o no 2
        novoNo->setUltimaAresta(ultimaAresta);
    }
    else
    {
        // verificar se já existe a aresta ----> criar função
        Aresta *ultimaAresta = noAddAresta->getUltimaAresta();
        if (ultimaAresta == NULL) //* o no não possui nenhuma aresta
        {
            //* cria a primeira aresta e a registra como ultima aresta
            Aresta *arestaUnica = noAddAresta->setPrimeiraAresta(no2, PESO_NAO_PONDERADO);
            noAddAresta->setUltimaAresta(arestaUnica);
        }
        else
        {
            //* caso ja exista uma aresta, adicione uma no final e a defina como ultima aresta
            Aresta *novaUltimaAresta = ultimaAresta->setProxAresta(no2, PESO_NAO_PONDERADO); // criar a funçãvo de adicionar aresta
            noAddAresta->setUltimaAresta(novaUltimaAresta);
        }
    }
}
void Grafo::AddNoArestaAux(int no1, int no2, int peso)
{
    No *noAddAresta = procuraId(no1);
    if (noAddAresta == NULL)
    { // no nao existe, logo insere ao fim da lista de nos
        No *listNos = raizGrafo;
        //* navega ate o ultimo no da lista
        while (listNos->getProxNo() != NULL)
        {
            listNos = listNos->getProxNo();
        }

        No *novoNo = NULL;
        if(ponderadoVertice){
            novoNo = new No(no1, peso);
        }else{
            novoNo = new No(no1, PESO_NAO_PONDERADO);
        }
        listNos->setProxNo(novoNo); //* adiciona o no na lista de nos
        Aresta *ultimaAresta = NULL;
        if (ponderadoAresta)
        {
            ultimaAresta = novoNo->setPrimeiraAresta(no2, peso); //* cria a primeira aresta apontando para o no 2
        }
        else
        {
            ultimaAresta = novoNo->setPrimeiraAresta(no2, PESO_NAO_PONDERADO); //* cria a primeira aresta apontando para o no 2
        }

        novoNo->setUltimaAresta(ultimaAresta);
    }
    else
    {
        // verificar se já existe a aresta ----> criar função
        Aresta *ultimaAresta = noAddAresta->getUltimaAresta();
        if (ultimaAresta == NULL) //* o no não possui nenhuma aresta
        {
            //* cria a primeira aresta e a registra como ultima aresta
            Aresta *arestaUnica = noAddAresta->setPrimeiraAresta(no2, peso);
            noAddAresta->setUltimaAresta(arestaUnica);
        }
        else
        {
            //* caso ja exista uma aresta, adicione uma no final e a defina como ultima aresta
            Aresta *novaUltimaAresta = ultimaAresta->setProxAresta(no2, peso); // criar a funçãvo de adicionar aresta
            noAddAresta->setUltimaAresta(novaUltimaAresta);
        }
    }
}
void Grafo::AddNoAresta(int no1, int no2)
{
    // cout << "adicionando " << no1 << " - " << no2 << endl;
    if (raizGrafo == NULL)
    {
        //* o grafo não possui arestas
        raizGrafo = new No(no1);
    }

    AddNoArestaAux(no1, no2);
    if (!direcionado)
    {
        AddNoArestaAux(no2, no1);
    }
    else
    {
        AddNoArestaAux(no2); //* adiciona o no sem conexões (se rodou tem alguma coisa errada)
    }
    //* é preciso repetir pois todos os nos devem estar na lista e todos os nos devem conter uma lista de arestas conectadas no mesmo
}
void Grafo::AddNoAresta(int no1, int no2, int peso)
{
    // cout << "adicionando " << no1 << " - " << no2 << endl;
    if (raizGrafo == NULL)
    {
        //* o grafo não possui arestas
        if(ponderadoVertice){
            raizGrafo = new No(no1, peso);
        }else{
            raizGrafo = new No(no1);
        }
    }

    AddNoArestaAux(no1, no2, peso);
    if (!direcionado)
    {
        AddNoArestaAux(no2, no1, peso);
    }
    else
    {
        AddNoArestaAux(no2); //* adiciona o no sem conexões (se rodou tem alguma coisa errada)
    }
}

No *Grafo::procuraId(int id)
{ // se encontrar o no, retorna um ponteiro para procurar a lista de aresta em outras funcoes
    No *nosGrafo = raizGrafo;

    while (nosGrafo != NULL)
    {
        if (nosGrafo->getId() == id)
        {
            return nosGrafo;
        }

        nosGrafo = nosGrafo->getProxNo();
    }
    return NULL;
}

void Grafo::buscaProfundidade(int id)
{
    Pilha pilhaVisitar;
    pilhaVisitar.Empilha(id);

    Lista visitados;

    // int idNavega = id;
    while (pilhaVisitar.getNEelementos() > 0)
    {
        int idNavega = pilhaVisitar.Desempilha();
        visitados.AddElemento(idNavega);
        // cout << idNavega << " ";

        // cout << "visitando o no " << idNavega << endl;
        No *noNavega = procuraId(idNavega);
        // if (arestaNavega == NULL)
        // {
        // cout << "O no " << idNavega << " não leva a ninguem" << endl;
        // }
        Aresta *arestaNavega = noNavega->getPrimeiraAresta();
        while (arestaNavega != NULL)
        {
            if (!visitados.contem(arestaNavega->getDestino()))
                pilhaVisitar.Empilha(arestaNavega->getDestino());
            arestaNavega = arestaNavega->getProxAresta();
        } //* coloca os elementos conectados a raiz na pilha
        // cout << "Visitado: " << noNavega->getId() << endl;
        // idNavega = pilhaVisitar.Desempilha();
    }

    No *nosGrafo = raizGrafo;

    cout << "Esse no não se conecta a ";
    while (nosGrafo != NULL)
    {
        if (!visitados.contem(nosGrafo->getId()))
        {
            cout << nosGrafo->getId() << " ";
        }

        nosGrafo = nosGrafo->getProxNo();
    }
    cout << endl;

    cout << "Esse no se conecta a " << visitados.getNElementos() << endl;
}
