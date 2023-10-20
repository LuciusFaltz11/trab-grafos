#ifndef PilhaArestasElementoDefined
#define PilhaArestasElementoDefined
class PilhaArestasElemento
{
private:
    int origem;
    int destino;
    PilhaArestasElemento* elementoAnterior;
public:
    PilhaArestasElemento(int origem, int destino, PilhaArestasElemento* elementoAnterior){
        this->origem = origem;
        this->destino = destino;
        this->elementoAnterior = elementoAnterior;
    };
    ~PilhaArestasElemento(){};
    int getOrigem(){
        return origem;
    };
    int getDestino(){
        return destino;
    }
    PilhaArestasElemento* getElementoAnterior(){
        return elementoAnterior;
    }
    void setElementoAnterior(PilhaArestasElemento* anterior)
    {
        this->elementoAnterior = anterior;
    }
};
/* code */
#endif //PilhaArestasElementoDefined
