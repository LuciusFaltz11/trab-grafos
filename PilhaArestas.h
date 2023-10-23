#ifndef PilhaArestasDefined
#define PilhaArestasDefined
#include <iostream>
#include "PilhaArestasElemento.h"
using namespace std;
class PilhaArestas
{
private:
    int nElementos = 0;
    PilhaArestasElemento* ultimaAresta = NULL;
public:
    PilhaArestas(){};
    ~PilhaArestas(){};
    int getNElementos(){
        return nElementos;
    }
    PilhaArestasElemento* Desempilha(){
        PilhaArestasElemento* result = new PilhaArestasElemento(ultimaAresta->getOrigem(), ultimaAresta->getDestino(), NULL);
        PilhaArestasElemento* antUltimaAresta = ultimaAresta;
        ultimaAresta = ultimaAresta->getElementoAnterior();
        delete antUltimaAresta;
        nElementos--;
        return result;
    }
    void Empilha(PilhaArestasElemento* elemento){
        elemento->setElementoAnterior(ultimaAresta);
        ultimaAresta = elemento;
        nElementos++;
    }
};
#endif // PilhaArestasDefined
