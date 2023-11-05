#define ARESTALISTA_H

class ArestaLista
{
private:
    int peso;
    int destino;
    int origem;
    ArestaLista *proxima = nullptr;

public:
    ArestaLista()
    {
        this->peso = 0;
        this->destino = -1;
        this->origem = -1;
        proxima = nullptr;
    };
    ArestaLista(int origemDef, int destinoDef, int pesoDef)
    {
        this->peso = pesoDef;
        this->origem = origemDef;
        this->destino = destinoDef;
        proxima = nullptr;
    };
    ArestaLista *getProxima()
    {
        return proxima;
    };
    void setProxima(ArestaLista *arestaNova)
    {
        proxima = arestaNova;
    };
    int getPeso()
    {
        return this->peso;
    };
    int getDestino()
    {
        return this->destino;
    };
    int getOrigem()
    {
        return this->origem;
    };
};
