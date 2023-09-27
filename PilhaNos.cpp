#include "PilhaNos.h"

PilhaNos::PilhaNos()
{
    ultimoElemento = NULL;
}

PilhaNos::~PilhaNos()
{
}
void PilhaNos::Empilha(No *no)
{
    ultimoElemento = new PilhaNosElemento(ultimoElemento, no);
}
No *PilhaNos::Desempilha()
{
    No *ultimoNo = ultimoElemento->getNo();
    PilhaNosElemento *ultimoElementoAnterior = ultimoElemento;
    ultimoElemento = ultimoElemento->getProxElemento();
    delete ultimoElemento;
    return ultimoNo;
}