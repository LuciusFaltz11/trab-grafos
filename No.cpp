#include "No.h"

int No::getId(){
    return this->idNo;
}

void No::setProxNo(No* prox){
    this->proximoNo = prox;
}

No* No::getProxNo(){
    return proximoNo;
}

Aresta* No::getPrimeiraAresta(){
    return this->primeiraAresta;
}

void No::setPrimeiraAresta(int primeira){
    Aresta primeira(int);
    this->primeiraAresta = primeira;
}

Aresta* No::getUltimaAresta(){
    Aresta* listArestas = primeiraAresta;
    while(listArestas != NULL){
        listArestas = listArestas->getProxAresta();
    }
    return listArestas;
}
