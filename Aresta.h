#ifndef ArestaDefined
#define ArestaDefined
/* code */

class Aresta
{
private:
    float peso;
    Aresta* proxAresta;
public:
    Aresta(int destino); // construtor para primeira aresta, sem peso
    ~Aresta();
};

#endif // ArestaDefined