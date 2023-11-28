#ifndef NO_H
#define NO_H
#include "Aresta.h"
#include <iostream>

using namespace std;

class No
{

private:
    int idNo;
    int pesoNo;
    int grauEntrada;
    int grauSaida;
    No *proximoNo;
    Aresta *primeiraAresta;
    Aresta *ultimaAresta;

    int coordenadaX;
    int coordenadaY;

public:
    // No(int id);
    No();
    No(int id, int coordenadaX, int coordenadaY);
    No(int id, int peso);
    int getId();
    No *getProxNo();
    void setProxNo(No *prox);
    Aresta *setPrimeiraAresta(int primeira, int peso);
    Aresta *setPrimeiraAresta(Aresta *aresta);
    Aresta *getPrimeiraAresta();
    Aresta *getUltimaAresta();
    void setUltimaAresta(Aresta *ultimaAresta);
    void setId(int id) { this->idNo = id; };
};

#endif