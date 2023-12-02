#ifndef ListaDefined
#define ListaDefined
#include "ListaElemento.h"
class Lista
{
private:
    ListaElemento *primeiroElemento;
    ListaElemento *ultimoElemento;
    void deleteAux(ListaElemento *elemento);
    int nElementos;
    Lista *proxElemento;
public:
    Lista();
    ~Lista();
    Lista *getProxElemento(){return proxElemento;};
    void setProxElemento(Lista *prox){proxElemento = prox;};
    void AddElemento(int elemento);
    bool contem(int elemento);
    int getNElementos();
    void iterate(void (*func)(int));
    ListaElemento *getPrimeiroElemento();
    void unirListas(Lista &novaLista);
    void imprime();
    int getTamanho(){return nElementos;};
    void sort(bool crescente);
    int getElemento(int posicao);
    ListaElemento *getElementoLista(int posicao){
        ListaElemento *elementoNav = primeiroElemento;
        for (int i = 0; i < posicao; i++)
        {
            elementoNav = elementoNav->getProxElemento();
        }
        return elementoNav;
    };
};

#endif // ListaDefined
