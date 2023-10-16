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
        No *novoNo = new No(no, PESO_NAO_PONDERADO);
        listNos->setProxNo(novoNo); //* adiciona o no na lista de nos
    }
}
void Grafo::AddNoArestaAux(int no1, int no2, int peso)
{
    AddNoArestaAux(no2); //* adiciona o segundo no para que o primeiro aponte para ele
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
        if (ponderadoVertice)
        {
            novoNo = new No(no1, peso);
        }
        else
        {
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
        raizGrafo = new No(no1, PESO_NAO_PONDERADO);
    }

    AddNoArestaAux(no1, no2, PESO_NAO_PONDERADO);
    if (!direcionado)
    {
        AddNoArestaAux(no2, no1, PESO_NAO_PONDERADO);
    }
    // else
    // {
    // AddNoArestaAux(no2, PESO_NAO_PONDERADO); //* adiciona o no sem conexões (se rodou tem alguma coisa errada)
    // }
    //* é preciso repetir pois todos os nos devem estar na lista e todos os nos devem conter uma lista de arestas conectadas no mesmo
}
void Grafo::AddNoAresta(int no1, int no2, int peso)
{
    // cout << "adicionando " << no1 << " - " << no2 << endl;
    if (raizGrafo == NULL)
    {
        //* o grafo não possui arestas
        if (ponderadoVertice)
        {
            raizGrafo = new No(no1, peso);
        }
        else
        {
            raizGrafo = new No(no1, PESO_NAO_PONDERADO);
        }
    }

    if (ponderadoAresta)
    {
        AddNoArestaAux(no1, no2, peso);
    }
    else
    {
        AddNoArestaAux(no1, no2, PESO_NAO_PONDERADO);
    }
    if (!direcionado)
    {
        if (ponderadoAresta)
        {
            AddNoArestaAux(no2, no1, peso);
        }
        else
        {
            AddNoArestaAux(no2, no1, PESO_NAO_PONDERADO);
        }
        // AddNoArestaAux(no2, no1, peso);
    }
    // else
    // {
    //     AddNoArestaAux(no2); //* adiciona o no sem conexões (se rodou tem alguma coisa errada)
    // }
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

Lista *Grafo::buscaProfundidade(int id)
{
    Pilha pilhaVisitar;
    pilhaVisitar.Empilha(id);

    Lista *visitados = new Lista;

    while (pilhaVisitar.getNEelementos() > 0)
    {
        int idNavega = pilhaVisitar.Desempilha();
        visitados->AddElemento(idNavega);

        No *noNavega = procuraId(idNavega);
        Aresta *arestaNavega = noNavega->getPrimeiraAresta();
        while (arestaNavega != NULL)
        {
            if (!visitados->contem(arestaNavega->getDestino()))
                pilhaVisitar.Empilha(arestaNavega->getDestino());
            arestaNavega = arestaNavega->getProxAresta();
        } //* coloca os elementos conectados a raiz na pilha
    }

    return visitados;
}

Lista *Grafo::getArestasNo(int id)
{
    Lista *result = new Lista;
    Aresta *arestaP = procuraId(id)->getPrimeiraAresta();
    while (arestaP->getProxAresta() != NULL)
    {
        result->AddElemento(arestaP->getDestino());
        arestaP = arestaP->getProxAresta();
    }
    return result;
}

bool Grafo::iterate(bool (*func)(int, int))
{

    No *nosNav = raizGrafo;

    while (nosNav != NULL)
    {
        Aresta *arestaNav = nosNav->getPrimeiraAresta();
        while (arestaNav != NULL)
        {
            cout << nosNav->getId() << " , " << arestaNav->getDestino() << endl;
            if (func(nosNav->getId(), arestaNav->getDestino()))
            {
                return true;
            }
            arestaNav = arestaNav->getProxAresta();
        }
        nosNav = nosNav->getProxNo();
    }
    return false;
}
void Grafo::generateDreampufFile(string filename)
{

    ofstream outdata; // outdata is like cin

    outdata.open(filename); // opens the file

    if (!outdata)
    { // file couldn't be opened
        cerr << "Error: file could not be opened" << endl;
        return;
    }

    if (direcionado)
    {
        outdata << "di";
    }
    outdata << "graph G {" << endl;

    No *nosNav = raizGrafo;

    while (nosNav != NULL)
    {
        Aresta *arestaNav = nosNav->getPrimeiraAresta();
        while (arestaNav != NULL)
        {
            if (nosNav->getId() == arestaNav->getDestino())
            {
                cout << "tem self loop no no " << nosNav->getId() << endl;
            }
            if (direcionado)
            {
                outdata << nosNav->getId() << " -> " << arestaNav->getDestino() << ";" << endl;
            }
            else
            {
                outdata << nosNav->getId() << " -- " << arestaNav->getDestino() << ";" << endl;
            }
            arestaNav = arestaNav->getProxAresta();
        }
        nosNav = nosNav->getProxNo();
    }
    outdata << "}" << endl;
    outdata.close();
}