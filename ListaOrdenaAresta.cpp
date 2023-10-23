#include "ListaOrdenaAresta.h"
#include <iostream>

using namespace std;

ListaOrdenaAresta::ListaOrdenaAresta()
{
    primeira = NULL;
    ultima = NULL;
    totalArestas = 0;
}

ListaOrdenaAresta::~ListaOrdenaAresta()
{
    ArestaLista *arestaDeleta;
    while (primeira != NULL)
    {
        arestaDeleta = primeira;
        primeira = primeira->getProxima();
        delete arestaDeleta;
        totalArestas--;
    }
}

bool ListaOrdenaAresta::existeAresta(int origemNovo, int destinoNovo)
{
    ArestaLista *aux = primeira;
    while (aux != NULL)
    {
        if (aux->getOrigem() == origemNovo && aux->getDestino() == destinoNovo)
        {
            return true;
        }
        aux = aux->getProxima();
    }
    return false;
}

void ListaOrdenaAresta::addAresta(int origem, int destino, int peso)
{
    ArestaLista *aux = primeira;
    while (aux != NULL)
    {
        if (aux->getOrigem() == origem && aux->getDestino() == destino)
        {
            return;
        }
        aux = aux->getProxima();
    }

    // A aresta não existe
    ArestaLista *novaAresta = new ArestaLista(origem, destino, peso);
    if (ultima == NULL)
    {
        primeira = novaAresta;
        ultima = primeira;
    }
    else
    {
        ultima->setProxima(novaAresta);
        ultima = ultima->getProxima();
    }
    totalArestas++;
}

void ListaOrdenaAresta::imprimeListaOrdenada()
{
    ArestaLista *aux = primeira;
    while (aux != NULL)
    {
        cout << "origem: " << aux->getOrigem() << " destino: " << aux->getDestino() << " peso: " << aux->getPeso() << endl;
        aux = aux->getProxima();
    }
}

void ListaOrdenaAresta::merge(ArestaLista *&cabeca, ArestaLista *esquerda, ArestaLista *direita)
{
    ArestaLista *merged = nullptr;
    ArestaLista *current = nullptr;

    while (esquerda != nullptr && direita != nullptr)
    {
        if (esquerda->getPeso() <= direita->getPeso())
        {
            if (merged == nullptr)
            {
                merged = esquerda;
                current = esquerda;
            }
            else
            {
                current->setProxima(esquerda);
                current = esquerda;
            }
            esquerda = esquerda->getProxima();
        }
        else
        {
            if (merged == nullptr)
            {
                merged = direita;
                current = direita;
            }
            else
            {
                current->setProxima(direita);
                current = direita;
            }
            direita = direita->getProxima();
        }
    }

    if (esquerda != nullptr)
    {
        if (merged == nullptr)
        {
            merged = esquerda;
        }
        else
        {
            current->setProxima(esquerda);
        }
    }
    else if (direita != nullptr)
    {
        if (merged == nullptr)
        {
            merged = direita;
        }
        else
        {
            current->setProxima(direita);
        }
    }

    cabeca = merged;
}

void ListaOrdenaAresta::mergeSort(ArestaLista *&cabeca)
{
    if (cabeca == nullptr || cabeca->getProxima() == nullptr)
    {
        return;
    }

    ArestaLista *esquerda = nullptr;
    ArestaLista *direita = nullptr;
    split(cabeca, esquerda, direita);

    mergeSort(esquerda);
    mergeSort(direita);

    merge(cabeca, esquerda, direita);
}

void ListaOrdenaAresta::split(ArestaLista *origem, ArestaLista *&frente, ArestaLista *&tras)
{
    ArestaLista *rapido = origem->getProxima();
    ArestaLista *lento = origem;

    while (rapido != nullptr)
    {
        rapido = rapido->getProxima();
        if (rapido != nullptr)
        {
            lento = lento->getProxima();
            rapido = rapido->getProxima();
        }
    }

    frente = origem;
    tras = lento->getProxima();
    lento->setProxima(nullptr);
}

void ListaOrdenaAresta::ordenaLista()
{
    mergeSort(primeira);
}

int ListaOrdenaAresta::getTotalArestas(){
    return this->totalArestas;
}