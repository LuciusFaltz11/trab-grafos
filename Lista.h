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

public:
    Lista();
    ~Lista();
    void AddElemento(int elemento);
    bool contem(int elemento);
    int getNElementos();
    void iterate(void (*func)(int));
    ListaElemento *getPrimeiroElemento();
    void unirListas(Lista &novaLista);
    void imprime();
};

#endif // ListaDefined
