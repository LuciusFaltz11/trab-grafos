#ifndef ListaDefined
#define ListaDefined
#include "ListaElemento.h"
class Lista
{
private:
    ListaElemento *primeiroElemento;
    void deleteAux(ListaElemento *elemento);
    int nElementos;

public:
    Lista();
    ~Lista();
    void AddElemento(int elemento);
    bool contem(int elemento);
    int getNElementos();
};

#endif // ListaDefined
