#ifndef EconomiaDefined
#define EconomiaDefined
#include "ListaElemento.h"
#include "No.h"
#include "Rota.h"
class Economia
{
private:
    float valor = 0;
    Rota* rota;
    Economia *proxElemento;
public:
    Economia(Rota* rota, float valor){
        this->rota = rota;
        this->valor = valor;
    };
    ~Economia(){
        delete rota;
    };
    float getValor(){return valor;};
    Rota* getRota(){return rota;};
    void setValor(float valor){this->valor = valor;};
    void setRota(Rota* rota){this->rota = rota;};
    void setProxElemento(Economia *prox){proxElemento = prox;};
    Economia *getProxElemento(){return proxElemento;};
};

#endif //EconomiaDefined
