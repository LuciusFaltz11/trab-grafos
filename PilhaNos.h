#ifndef PilhaNosDefined
#define PilhaNosDefined
#include "PilhaNosElemento.h"
class PilhaNos
{
private:
    PilhaNosElemento* ultimoElemento;
public:
    PilhaNos();
    ~PilhaNos();
    void Empilha(No*);
    No* Desempilha();
};

#endif // PilhaNosDefined
