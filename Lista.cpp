#include "Lista.h"

Lista::Lista()
{
    primeiroElemento = NULL;
    nElementos = 0;
}

Lista::~Lista()
{
    deleteAux(primeiroElemento);
}
void Lista::deleteAux(ListaElemento *elemento)
{
    if (elemento->getProxElemento() != NULL)
    {
        deleteAux(elemento->getProxElemento());
    }
    delete (elemento->getProxElemento());
}
void Lista::AddElemento(int elemento)
{
    if (contem(elemento))
    {
        return;
    }
    nElementos++;
    if (primeiroElemento == NULL)
    {
        primeiroElemento = new ListaElemento(elemento);
        return;
    }
    ListaElemento *elementoNav = primeiroElemento;
    while (elementoNav->getProxElemento() != NULL)
    {
        elementoNav = elementoNav->getProxElemento();
    }
    elementoNav->setProxElemento(new ListaElemento(elemento));
}

bool Lista::contem(int elemento)
{
    ListaElemento *elementoNav = primeiroElemento;
    while (elementoNav != NULL)
    {
        if (elementoNav->getValue() == elemento)
        {
            return true;
        }
        elementoNav = elementoNav->getProxElemento();
    }
    return false;
}
int Lista::getNElementos()
{
    return nElementos;
}