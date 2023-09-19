#ifndef NO_H
#define NO_H
#include "Aresta.h"
#include <iostream>

using namespace std;

class No{

private:
    int idNo;
    int pesoNo;
    int grauEntrada;
    int grauSaida;
    No* proximoNo;
    Aresta* primeiraAresta;

public:
    No(int id);
    No(int id, int peso);
    int getId();
    No* getProxNo();
    void setProxNo(No* prox);
    void setPrimeiraAresta(int primeira);
    Aresta* getPrimeiraAresta();
    Aresta* getUltimaAresta();


};



#endif