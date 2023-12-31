#ifndef ListaRotaDefined
#define ListaRotaDefined
// #include "ListaElemento.h"
#include "Rota.h"
class ListaRotas
{
private:
    Rota *primeiroElemento = NULL;
    Rota *ultimoElemento = NULL;
    int nElementos;

public:
    ListaRotas(){};
    ~ListaRotas(){};
    void AddElemento(Rota *elemento){
        nElementos++;
        if (primeiroElemento == NULL)
        {
            primeiroElemento = elemento;
            ultimoElemento = primeiroElemento;
            return;
        }
        ultimoElemento->setProxElemento(elemento);
        ultimoElemento = elemento;
    };
    void removePrimeiroElemento(){
        Rota* aux = primeiroElemento;
        primeiroElemento = primeiroElemento->getProxElemento();
        delete aux;
    };
    void removeElemento(Rota* rotaRemover){
        Rota* elementoNav = primeiroElemento;
        Rota* elementoAnterior = primeiroElemento;
        while (elementoNav != NULL)
        {
            if (elementoNav == rotaRemover)
            {
                nElementos--;
                if (elementoNav == primeiroElemento)
                {
                    primeiroElemento = primeiroElemento->getProxElemento();
                    delete elementoNav;
                    return;
                }
                if (elementoNav == ultimoElemento)
                {
                    ultimoElemento = elementoAnterior;
                    ultimoElemento->setProxElemento(NULL);
                    delete elementoNav;
                    return;
                }
                elementoAnterior->setProxElemento(elementoNav->getProxElemento());
                delete elementoNav;
                return;
            }
            elementoAnterior = elementoNav;
            elementoNav = elementoNav->getProxElemento();
        }
    };
    Rota *getPrimeiroElemento(){return primeiroElemento;};
    Rota *getElemento(int posicao){
        Rota *elementoNav = primeiroElemento;
        for (int i = 0; i < posicao; i++)
        {
            elementoNav = elementoNav->getProxElemento();
        }
        return elementoNav;
    };
    int getNElementos(){return nElementos;};
    void imprime(){
        Rota *elementoNav = primeiroElemento;
        int i = 0;
        while (elementoNav != NULL)
        {
            cout << "[ rota " << i << " ] :  ";
            elementoNav->imprime();
            elementoNav = elementoNav->getProxElemento();
            i++;
        }
        cout << endl;
    };
};

#endif // ListaRotaDefined
