#include "No.h"

No::No(int id)
{
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->idNo = id;
    this->pesoNo = 0; 
    this->primeiraAresta = NULL;
    this->ultimaAresta = NULL;
    this->proximoNo = NULL;
}
No::No(int id, int peso) {
    
}

int No::getId()
{
    return this->idNo;
}

void No::setProxNo(No *prox)
{
    this->proximoNo = prox;
}

No *No::getProxNo()
{
    return proximoNo;
}

Aresta *No::getPrimeiraAresta()
{
    return this->primeiraAresta;
}

Aresta* No::setPrimeiraAresta(int primeira)
{
    Aresta *primeiraAresta = new Aresta(primeira);
    this->primeiraAresta = primeiraAresta;
    return primeiraAresta;
}
void No::setUltimaAresta(Aresta *ultimaAresta)
{
    this->ultimaAresta = ultimaAresta;
}

Aresta *No::getUltimaAresta()
{
    return ultimaAresta;
    // Aresta *listArestas = primeiraAresta;
    // while (listArestas != NULL)
    // {
    //     listArestas = listArestas->getProxAresta();
    // }
    // return listArestas;
}
