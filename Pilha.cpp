#include "Pilha.h"

Pilha::Pilha()
{
    ultimoElemento = NULL;
    nElementos = 0;
}

Pilha::~Pilha()
{
    while (nElementos > 0)
    {
        Desempilha();
    }
}
void Pilha::Empilha(int noId)
{
    if (existe(noId))
    {
        return;
    }
    // cout << "Empilhou o " << noId << endl;
    ultimoElemento = new PilhaElemento(ultimoElemento, noId);
    nElementos++;
}
int Pilha::Desempilha()
{
    if (nElementos == 0)
    {
        return -1;
    }
    int ultimoNo = ultimoElemento->getNo();
    PilhaElemento *ultimoElementoAnterior = ultimoElemento;
    ultimoElemento = ultimoElemento->getProxElemento();
    delete (ultimoElementoAnterior);
    nElementos--;
    // cout << "desempilhou: "<<ultimoNo<<endl;
    return ultimoNo;
}
int Pilha::getNEelementos()
{
    return nElementos;
}
bool Pilha::existe(int idNo)
{
    PilhaElemento *elementoNav = ultimoElemento;
    while (elementoNav != NULL)
    {
        if (elementoNav->getNo() == idNo)
        {
            return true;
        }
        elementoNav = elementoNav->getProxElemento();
    }
    return false;
}