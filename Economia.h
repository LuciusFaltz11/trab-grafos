#ifndef EconomiaDefined
#define EconomiaDefined
#include "ListaElemento.h"
#include "No.h"
#include "Rota.h"
class Economia
{
private:
    float valor = 0;
    Rota *rota = NULL;
    Economia *proxElemento = NULL;

public:
    Economia(Rota *rota, float valor)
    {
        this->rota = rota;
        this->valor = valor;
    };
    ~Economia()
    {
        delete rota;
    };
    float getValor() { return valor; };
    Rota *getRota() { return rota; };
    Rota *cloneRota()
    {
        Rota *novaRota = new Rota(rota->getCapacidade());
        No *elementoNav = rota->getPrimeiroElemento();
        while (elementoNav != NULL)
        {
            novaRota->AddElemento(elementoNav->getId(), elementoNav->getPeso(), elementoNav->getCoordenadaX(), elementoNav->getCoordenadaY());
            elementoNav = elementoNav->getProxNo();
        }
        return novaRota;
    };
    void setValor(float valor) { this->valor = valor; };
    void setRota(Rota *rota) { this->rota = rota; };
    void setProxElemento(Economia *prox) { proxElemento = prox; };
    Economia *getProxElemento() { return proxElemento; };
};

#endif // EconomiaDefined
