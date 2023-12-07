#ifndef NO_H
#define NO_H
#include "Aresta.h"
#include <iostream>
// using namespace std;

class No
{

private:
    int idNo;
    int pesoNo;
    int grauEntrada;
    int grauSaida;
    No *proximoNo = NULL;
    Aresta *primeiraAresta = NULL;
    Aresta *ultimaAresta = NULL;

    float coordenadaX;
    float coordenadaY;

public:
    // No(int id);
    No();
    No(int id, int peso, float coordenadaX, float coordenadaY);
    No(int id, float coordenadaX, float coordenadaY);
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
    void setPeso(int peso);
    int getPeso(){return pesoNo;};
    float getCoordenadaX(){return coordenadaX;};
    float getCoordenadaY(){return coordenadaY;};
};

#endif