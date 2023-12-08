#ifndef ListaNosDefined
#define ListaNosDefined
#include "No.h"
class ListaNos
{
private:
    No *primeiroElemento = NULL;
    No *ultimoElemento = NULL;
    int nElementos = 0;
public:
    ListaNos(){};
    ~ListaNos(){};
    void AddElemento(int id, int peso, int coordenadaX, int coordenadaY){
        // cria um novo no de acordo com o no passado
        nElementos++;
        No *novoNo = new No(id, peso, coordenadaX, coordenadaY);
        if (primeiroElemento == NULL)
        {
            primeiroElemento = novoNo;
            ultimoElemento = novoNo;
            return;
        }
        ultimoElemento->setProxNo(novoNo);
        ultimoElemento = novoNo;
    };
    No *getPrimeiroElemento(){return primeiroElemento;};
    No *getElemento(int posicao){
        No *elementoNav = primeiroElemento;
        for (int i = 0; i < posicao; i++)
        {
            elementoNav = elementoNav->getProxNo();
        }
        return elementoNav;
    };
    int getNElementos(){return nElementos;};
    void removeElemento(int id){
        No *elementoNav = primeiroElemento;
        No *elementoAnterior = NULL;
        while (elementoNav != NULL)
        {
            if (elementoNav->getId() == id)
            {
                nElementos--;
                if (elementoAnterior == NULL)
                {
                    primeiroElemento = elementoNav->getProxNo();
                    delete elementoNav;
                    return;
                }
                elementoAnterior->setProxNo(elementoNav->getProxNo());
                delete elementoNav;
                return;
            }
            elementoAnterior = elementoNav;
            elementoNav = elementoNav->getProxNo();
        }
    };
    bool possuiElemento(int id){
        No *elementoNav = primeiroElemento;
        while (elementoNav != NULL)
        {
            if (elementoNav->getId() == id)
            {
                return true;
            }
            elementoNav = elementoNav->getProxNo();
        }
        return false;
    };
};

#endif //ListaNosDefined
