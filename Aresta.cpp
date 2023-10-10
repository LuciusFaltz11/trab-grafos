#include "Aresta.h"

Aresta::Aresta(int destino, int peso)
{
    peso = 0;
    proxAresta = NULL;
    this->destino = destino;
}

Aresta::~Aresta()
{
}
Aresta* Aresta::setProxAresta(int idNo, int peso)
{
    // cout << "Criou aresta apontando para o no " << idNo << endl;
    Aresta* novaAresta = new Aresta(idNo, peso);
    proxAresta = novaAresta;
    return novaAresta;
}
