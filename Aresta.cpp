#include "Aresta.h"

Aresta::Aresta(int destino)
{
    peso = 0;
    proxAresta = NULL;
    this->destino = destino;
}

Aresta::~Aresta()
{
}
Aresta* Aresta::setProxAresta(int idNo)
{
    // cout << "Criou aresta apontando para o no " << idNo << endl;
    Aresta* novaAresta = new Aresta(idNo);
    proxAresta = novaAresta;
    return novaAresta;
}
