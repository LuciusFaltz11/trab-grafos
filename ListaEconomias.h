#ifndef ListaEconomiasDefined
#define ListaEconomiasDefined
#include "ListaElemento.h"
#include "Economia.h"
class ListaEconomias
{
private:
    Economia *primeiroElemento = NULL;
    Economia *ultimoElemento = NULL;
    ListaEconomias *proxElemento = NULL;
    int nElementos;

public:
    ListaEconomias(){};
    ~ListaEconomias(){};
    void AddElemento(Economia *elemento)
    {
        if (primeiroElemento == NULL)
        {
            primeiroElemento = elemento;
            ultimoElemento = primeiroElemento;
            return;
        }
        ultimoElemento->setProxElemento(elemento);
        ultimoElemento = elemento;
    };
    Economia *getPrimeiroElemento() { return primeiroElemento; };
    void removePrimeiroElemento()
    {
        Economia* aux = primeiroElemento;
        primeiroElemento = primeiroElemento->getProxElemento();
        delete aux;
    };
    Economia *getElemento(int posicao)
    {
        Economia *elementoNav = primeiroElemento;
        for (int i = 0; i < posicao; i++)
        {
            elementoNav = elementoNav->getProxElemento();
        }
        return elementoNav;
    };
    int getNElementos() { return nElementos; };
    // void setNElementos(int nElementos){this->nElementos = nElementos;};
    void setProxElemento(ListaEconomias *prox) { proxElemento = prox; };
    ListaEconomias *getProxElemento() { return proxElemento; };
    void imprime()
    {
        Economia *elementoNav = primeiroElemento;
        while (elementoNav != NULL)
        {
            cout << elementoNav->getValor() << " ";
            elementoNav = elementoNav->getProxElemento();
        }
        cout << endl;
    };
    void sort(){
        Economia *elementoNav = primeiroElemento;
        Economia *elementoNav2 = primeiroElemento;
        Economia *aux;
        while (elementoNav != NULL)
        {
            elementoNav2 = primeiroElemento;
            while (elementoNav2 != NULL)
            {
                if (elementoNav->getValor() < elementoNav2->getValor())
                {
                    aux = elementoNav->getProxElemento();
                    elementoNav->setProxElemento(elementoNav2->getProxElemento());
                    elementoNav2->setProxElemento(aux);
                }
                elementoNav2 = elementoNav2->getProxElemento();
            }
            elementoNav = elementoNav->getProxElemento();
        }
    }
};

#endif // ListaEconomiasDefined
