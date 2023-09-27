#include "Grafo.h"

Grafo::Grafo()
{
    unico = 0;
    duplo = 0;
    raizGrafo = NULL;
}

Grafo::~Grafo()
{
}

void Grafo::AddNoAresta(int no)
{
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
        listNos->setProxNo(novoNo);     //* adiciona o no na lista de nos
        Aresta* ultimaAresta = novoNo->setPrimeiraAresta(no2); //* cria a primeira aresta apontando para o no 2
        novoNo->setUltimaAresta(ultimaAresta);
    }
    else
    {
        // verificar se já existe a aresta ----> criar função
        Aresta *ultimaAresta = noAddAresta->getUltimaAresta();
        if (ultimaAresta == NULL) //* o no não possui nenhuma aresta
        {
            //* cria a primeira aresta e a registra como ultima aresta
            Aresta *arestaUnica = noAddAresta->setPrimeiraAresta(no2);
            noAddAresta->setUltimaAresta(arestaUnica);
        }
        else
        {
            //* caso ja exista uma aresta, adicione uma no final e a defina como ultima aresta
            Aresta *novaUltimaAresta = ultimaAresta->setProxAresta(no2); // criar a funçãvo de adicionar aresta
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

    AddNoArestaAux(no1,no2);
    AddNoArestaAux(no2,no1);
    //* é preciso repetir pois todos os nos devem estar na lista e todos os nos devem conter uma lista de arestas conectadas no mesmo
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