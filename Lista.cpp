#include "Lista.h"
#include <iostream>

using namespace std;

Lista::Lista()
{
    primeiroElemento = NULL;
    ultimoElemento = NULL;
    nElementos = 0;
}

Lista::~Lista()
{
    // deleteAux(primeiroElemento);
    ListaElemento *elementoNav;
    while (primeiroElemento != NULL)
    {
        elementoNav = primeiroElemento;
        primeiroElemento = primeiroElemento->getProxElemento();
        delete elementoNav;
        nElementos--;
    }
    // std::cout << "Deletado elemento da lista: nElementos = " << nElementos << std::endl;
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
        ultimoElemento = primeiroElemento;
        return;
    }
    // ListaElemento *elementoNav = primeiroElemento;
    // while (elementoNav->getProxElemento() != NULL)
    // {
    //     elementoNav = elementoNav->getProxElemento();
    // }
    ultimoElemento->setProxElemento(new ListaElemento(elemento));
    // ultimoElemento->getProxElemento()->setAnteElemento(ultimoElemento);
    ultimoElemento = ultimoElemento->getProxElemento();
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

void Lista::iterate(void (*func)(int))
{
    ListaElemento *elementoNav = primeiroElemento;
    while (elementoNav != NULL)
    {
        func(elementoNav->getValue());
        elementoNav = elementoNav->getProxElemento();
    }
}

ListaElemento *Lista::getPrimeiroElemento()
{
    return primeiroElemento;
}

void Lista::sort(bool crescente)
{
    ListaElemento *elementoNav = primeiroElemento;
    ListaElemento *elementoNav2 = primeiroElemento;
    int aux;
    while (elementoNav != NULL)
    {
        elementoNav2 = primeiroElemento;
        while (elementoNav2 != NULL)
        {
            if (crescente)
            {
                if (elementoNav->getValue() < elementoNav2->getValue())
                {
                    aux = elementoNav->getValue();
                    elementoNav->setValue(elementoNav2->getValue());
                    elementoNav2->setValue(aux);
                }
            }
            else
            {
                if (elementoNav->getValue() > elementoNav2->getValue())
                {
                    aux = elementoNav->getValue();
                    elementoNav->setValue(elementoNav2->getValue());
                    elementoNav2->setValue(aux);
                }
            }
            elementoNav2 = elementoNav2->getProxElemento();
        }
        elementoNav = elementoNav->getProxElemento();
    }
}

int Lista::getElemento(int posicao)
{
    ListaElemento *elementoNav = primeiroElemento;
    for (int i = 0; i < posicao; i++)
    {
        elementoNav = elementoNav->getProxElemento();
    }
    return elementoNav->getValue();
}

/*
void Lista::unirListas(Lista &novaLista)
{
    if (novaLista.getPrimeiroElemento() == NULL)
    {
        return;
    }

    if (primeiroElemento == NULL)
    {
        primeiroElemento = novaLista.getPrimeiroElemento();
        ultimoElemento = novaLista.getPrimeiroElemento();
        nElementos = novaLista.getNElementos();
    }
    else
    { // adicionar as duas listas
        ultimoElemento->setProxElemento(novaLista.getPrimeiroElemento());
        ultimoElemento = novaLista.ultimoElemento;
        nElementos += novaLista.getNElementos();
    }

    novaLista.nElementos = 0;
}

void Lista::imprime()
{
    ListaElemento *nos = primeiroElemento;

    while (nos != NULL)
    {
        cout << nos->getValue() << endl;
        nos = nos->getProxElemento();
    }
}
*/