#ifndef ArestaDefined
#define ArestaDefined
/* code */
#include <iostream>
using namespace std;
class Aresta
{
private:
    float peso;
    Aresta *proxAresta;
    int destino;

public:
    Aresta(int destino); // construtor para primeira aresta, sem peso
    ~Aresta();
    Aresta *getProxAresta()
    {
        return proxAresta;
    }
    Aresta *setProxAresta(int idNo); //* cria uma aresta no fim da lista de aresta e retorna o ponteiro para a mesma
    int getDestino() { return destino; };
};

#endif // ArestaDefined