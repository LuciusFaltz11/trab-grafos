#include "Lista.h"

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
    std::cout << "Deletado elemento da lista: nElementos = " << nElementos << std::endl;
}
// void Lista::deleteAux(ListaElemento *elemento)
// {
//     if (elemento->getProxElemento() != NULL)
//     {
//         deleteAux(elemento->getProxElemento());
//     }
//     delete (elemento->getProxElemento());
// }
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