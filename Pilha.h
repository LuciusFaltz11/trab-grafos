#ifndef PilhaNosDefined
#define PilhaNosDefined
#include "PilhaElemento.h"
class Pilha
{
private:
    PilhaElemento* ultimoElemento;
    int nElementos;
    bool existe(int idNo);
public:
    Pilha();
    ~Pilha();
    void Empilha(int noId);
    int Desempilha();
    int getNEelementos();
};

#endif // PilhaNosDefined
