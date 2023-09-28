#ifndef ListaElementoDefined
#define ListaElementoDefined
#include <iostream>
class ListaElemento
{
private:
    int value;
    ListaElemento *proxElemento;

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
};

#endif // ListaElementoDefined
