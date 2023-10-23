#ifndef NoPilhaNosDefined
#define NoPilhaNosDefined
#include "No.h"

class PilhaElemento
{
private:
    int noId;
    PilhaElemento *proximoElemento;

public:
    PilhaElemento(PilhaElemento *proximoElemento, int noId) { this->proximoElemento = proximoElemento; 
    this->noId = noId;
    };
    ~PilhaElemento(){};
    PilhaElemento *getProxElemento() { return proximoElemento; };
    int getNo() { return noId; };
};

#endif // NoPilhaNosDefined
