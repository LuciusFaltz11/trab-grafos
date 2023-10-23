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
    ultimoNo = NULL;
    this->ponderadoAresta = ponderadoId == 1 || ponderadoId == 3;
    this->ponderadoVertice = ponderadoId == 2 || ponderadoId == 3;
    this->direcionado = direcionado;
    this->ponderadoId = ponderadoId;
}

Grafo::~Grafo()
{
    No *noNav = raizGrafo;
    No *prevNo;
    Aresta *prevAresta;
    while (noNav != NULL)
    {
        prevNo = noNav;
        noNav = noNav->getProxNo();
        while (prevNo->getPrimeiraAresta() != NULL)
        {
            prevAresta = prevNo->getPrimeiraAresta();
            prevNo->setPrimeiraAresta(prevAresta->getProxAresta());
            delete prevAresta;
        }
        delete prevNo;
    }
}

void Grafo::AddNo(int no)
{
    No *noAdd = procuraId(no);
    if (noAdd == NULL)
    {
        No *novoNo = new No(no, PESO_NAO_PONDERADO);
        ultimoNo->setProxNo(novoNo); //* adiciona o no na lista de nos
        ultimoNo = novoNo;
    }
}
void Grafo::AddNoArestaAux(int no1, int no2, int peso)
{
    AddNo(no2); //* adiciona o segundo no para que o primeiro aponte para ele
    No *noAddAresta = procuraId(no1);
    if (noAddAresta == NULL)
    { // no nao existe, logo insere ao fim da lista de nos

        No *novoNo = NULL;
        if (ponderadoVertice)
        {
            novoNo = new No(no1, peso);
        }
        else
        {
            novoNo = new No(no1, PESO_NAO_PONDERADO);
        }
        ultimoNo->setProxNo(novoNo); //* adiciona o no na lista de nos
        ultimoNo = novoNo;
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
    if (raizGrafo == NULL)
    {
        //* o grafo não possui arestas
        raizGrafo = new No(no1, PESO_NAO_PONDERADO);
        ultimoNo = raizGrafo;
    }

    AddNoArestaAux(no1, no2, PESO_NAO_PONDERADO);
    if (!direcionado)
    {
        AddNoArestaAux(no2, no1, PESO_NAO_PONDERADO);
    }
}
void Grafo::AddNoAresta(int no1, int no2, int peso)
{
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
        ultimoNo = raizGrafo;
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
        // cout << "idNavega: " << idNavega << " noNavega: " << (noNavega == NULL?"null":"ok") << endl;
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

    outdata.open("out/" + filename); // opens the file

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
    cout << "Dreampuf File " << filename << " criado no local out/" << filename << "." << endl;
}

Grafo *Grafo::inverteArestasDirecionadas()
{
    if (!direcionado)
    {
        return NULL;
    }
    Grafo *newGrafo = new Grafo(direcionado, ponderadoId);

    No *nosNav = raizGrafo;

    while (nosNav != NULL)
    {
        Aresta *arestaNav = nosNav->getPrimeiraAresta();
        while (arestaNav != NULL)
        {
            if (ponderadoAresta)
            {
                newGrafo->AddNoAresta(arestaNav->getDestino(), nosNav->getId(), arestaNav->getPeso());
            }
            else if (ponderadoVertice)
            {
                cout << "não suportado atualmente!" << endl;
                exit(1);
            }
            else
            {
                newGrafo->AddNoAresta(arestaNav->getDestino(), nosNav->getId());
            }
            arestaNav = arestaNav->getProxAresta();
        }
        nosNav = nosNav->getProxNo();
    }
    return newGrafo;
}

void Grafo::arvoreProfundidade(int id, bool generateDreampufFile)
{
    if (!generateDreampufFile)
    {
        arvoreProfundidade(id);
        return;
    }
    ofstream outdata;
    outdata.open("out/arvoreProfundidade.dat");

    if (!outdata)
    {
        cerr << "Error: file could not be opened" << endl;
        return;
    }
    outdata << "digraph G {" << endl;

    //* faz uma busca em profundidade e adiciona arestas com destinos nao visitados ao novo grafo

    PilhaArestas pilhaVisitarArestas;
    pilhaVisitarArestas.Empilha(new PilhaArestasElemento(id, id, NULL));
    Lista adicionados;
    adicionados.AddElemento(id);

    Grafo arvoreProfundidade(true, 0);

    bool retorno = true;

    while (pilhaVisitarArestas.getNElementos() > 0)
    {
        PilhaArestasElemento *arestaElemento = pilhaVisitarArestas.Desempilha();
        int idNavega = arestaElemento->getDestino();

        No *noNavega = procuraId(idNavega);
        Aresta *arestaNavega = noNavega->getPrimeiraAresta();

        retorno = true;
        while (arestaNavega != NULL)
        {
            if (!adicionados.contem(arestaNavega->getDestino()))
            {
                retorno = false;
                arvoreProfundidade.AddNoAresta(idNavega, arestaNavega->getDestino());
                outdata << idNavega << " -> " << arestaNavega->getDestino() << ";" << endl;
                adicionados.AddElemento(arestaNavega->getDestino());
                pilhaVisitarArestas.Empilha(new PilhaArestasElemento(
                    idNavega,
                    arestaNavega->getDestino(), NULL));
            }
            arestaNavega = arestaNavega->getProxAresta();
        } //* coloca os elementos conectados a raiz na pilha

        if (retorno && pilhaVisitarArestas.getNElementos() > 0)
        {
            PilhaArestasElemento *origem = pilhaVisitarArestas.Desempilha();
            pilhaVisitarArestas.Empilha(origem);
            cout << "RETORNO : " << idNavega << "->" << origem->getOrigem() << endl;
            arvoreProfundidade.AddNoAresta(idNavega, origem->getOrigem());
            outdata << idNavega << " -> " << origem->getOrigem() << " [color=green,constraint=false];" << endl;
        }
        delete arestaElemento;
    }
    outdata << "}" << endl;
    outdata.close();
    cout << "Dreampuf File arvoreProfundidade.dat criado no local out/arvoreProfundidade.dat." << endl;
}
void Grafo::arvoreProfundidade(int id)
{
    //* faz uma busca em profundidade e adiciona arestas com destinos nao visitados ao novo grafo

    PilhaArestas pilhaVisitarArestas;
    pilhaVisitarArestas.Empilha(new PilhaArestasElemento(id, id, NULL));
    Lista adicionados;
    adicionados.AddElemento(id);

    Grafo arvoreProfundidade(true, 0);

    bool retorno = true;

    while (pilhaVisitarArestas.getNElementos() > 0)
    {
        PilhaArestasElemento *arestaElemento = pilhaVisitarArestas.Desempilha();
        int idNavega = arestaElemento->getDestino();

        No *noNavega = procuraId(idNavega);
        Aresta *arestaNavega = noNavega->getPrimeiraAresta();

        retorno = true;
        while (arestaNavega != NULL)
        {
            if (!adicionados.contem(arestaNavega->getDestino()))
            {
                retorno = false;
                arvoreProfundidade.AddNoAresta(idNavega, arestaNavega->getDestino());
                adicionados.AddElemento(arestaNavega->getDestino());
                pilhaVisitarArestas.Empilha(new PilhaArestasElemento(
                    idNavega,
                    arestaNavega->getDestino(), NULL));
            }
            arestaNavega = arestaNavega->getProxAresta();
        } //* coloca os elementos conectados a raiz na pilha

        if (retorno && pilhaVisitarArestas.getNElementos() > 0)
        {
            PilhaArestasElemento *origem = pilhaVisitarArestas.Desempilha();
            pilhaVisitarArestas.Empilha(origem);
            cout << "RETORNO : " << idNavega << "->" << origem->getOrigem() << endl;
        }
        delete arestaElemento;
    }
}

void Grafo::arvoreMinimaKruskal()
{
    ListaOrdenaAresta *listaAresta = new ListaOrdenaAresta();
    criaListaOrdenadaAresta(listaAresta, direcionado);
    listaAresta->imprimeListaOrdenada();
}

void Grafo::criaListaOrdenadaAresta(ListaOrdenaAresta *lista, bool direcionado)
{
    cout << "Criando lista de arestas" << endl;
    Aresta *arestaGrafo;
    No *noGrafo = raizGrafo;
    if (direcionado)
    {
        while (noGrafo != NULL)
        {
            arestaGrafo = noGrafo->getPrimeiraAresta();
            while (arestaGrafo != NULL)
            {
                lista->addAresta(noGrafo->getId(), arestaGrafo->getDestino(), arestaGrafo->getPeso());
                arestaGrafo = arestaGrafo->getProxAresta();
            }
            noGrafo = noGrafo->getProxNo();
        }
    }
    else
    {
        while (noGrafo != NULL)
        {
            arestaGrafo = noGrafo->getPrimeiraAresta();
            while (arestaGrafo != NULL)
            {
                if (!(lista->existeAresta(noGrafo->getId(), arestaGrafo->getDestino()) || lista->existeAresta(arestaGrafo->getDestino(), noGrafo->getId())))
                {
                    lista->addAresta(noGrafo->getId(), arestaGrafo->getDestino(), arestaGrafo->getPeso());
                }
                arestaGrafo = arestaGrafo->getProxAresta();
            }
            noGrafo = noGrafo->getProxNo();
        }
    }
}

int Grafo::contabilizaArestas(bool direcionado){
    int numArestas = 0;
    No *noGrafo = raizGrafo;

    if(direcionado){

        return numArestas;
    }

    if(!direcionado){
        while (noGrafo != NULL){
        Aresta *arestaGrafo = noGrafo->getPrimeiraAresta();
        while (arestaGrafo != NULL){
            numArestas++;
            arestaGrafo = arestaGrafo->getProxAresta();
        }
        noGrafo = noGrafo->getProxNo();
        }
        return numArestas;
    }
}

const int INFINITO = 1e9;
void Grafo::getCaminhoMaisCurtoDjkstra(int idNo1, int idNo2){
    ListaOrdenaAresta *listaDeArestas = new ListaOrdenaAresta();
    criaListaOrdenadaAresta(listaDeArestas, this->direcionado);
    // int distancia[];
    // bool visitados[];
    //inicializacaoDosVetores
    // for(int i = 0; i < ; i++){
    //     distancia[i] = INFINITO;
    //     visitados[i] = false;
    // }

    // distancia[idNo1] = 0;
}