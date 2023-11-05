#ifndef ListaElementoDefined
#define ListaElementoDefined
#include <iostream>
class ListaElemento
{
private:
    int value;
    ListaElemento *proxElemento = NULL;
    ListaElemento *anteElemento = NULL;

public:
    ListaElemento(int value)
    {
        this->proxElemento = NULL;
        this->value = value;
    };
    ~ListaElemento(){};
    int getValue() { return value; };
    void setProxElemento(ListaElemento *proxElemento)
    {
        this->proxElemento = proxElemento;
    }
    ListaElemento *getProxElemento()
    {
        return proxElemento;
    }
    void setAnteElemento(ListaElemento *anteElemento)
    {
        this->anteElemento = anteElemento;
    }
    ListaElemento *getAnteElemento()
    {
        return anteElemento;
    }
};

#endif // ListaElementoDefined
