#ifndef NoPilhaNosDefined
#define NoPilhaNosDefined
#include "No.h"

class PilhaNosElemento
{
private:
    No *no;
    PilhaNosElemento *proximoElemento;

public:
    PilhaNosElemento(PilhaNosElemento *proximoElemento, No *no) { this->proximoElemento = proximoElemento; };
    ~PilhaNosElemento(){};
    PilhaNosElemento *getProxElemento() { return proximoElemento; };
    No *getNo() { return no; };
};

#endif // NoPilhaNosDefined
