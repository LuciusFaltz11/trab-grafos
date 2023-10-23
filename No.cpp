#include "No.h"

// No::No(int id)
// {
//     this->grauEntrada = 0;
//     this->grauSaida = 0;
//     this->idNo = id;
//     this->pesoNo = 0; 
//     this->primeiraAresta = NULL;
//     this->ultimaAresta = NULL;
//     this->proximoNo = NULL;
// }
No::No(int id, int peso) {
    this->grauEntrada = 0;
    this->grauSaida = 0;
    this->idNo = id;
    this->pesoNo = peso; 
    this->primeiraAresta = NULL;
    this->ultimaAresta = NULL;
    this->proximoNo = NULL;
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

Aresta* No::setPrimeiraAresta(int primeira, int peso)
{
    Aresta *primeiraAresta = new Aresta(primeira, peso);
    this->primeiraAresta = primeiraAresta;
    return primeiraAresta;
}
Aresta* No::setPrimeiraAresta(Aresta* aresta)
{
    this->primeiraAresta = aresta;
    return primeiraAresta;
}
void No::setUltimaAresta(Aresta *ultimaAresta)
{
    this->ultimaAresta = ultimaAresta;
}

Aresta *No::getUltimaAresta()
{
    return ultimaAresta;
}
