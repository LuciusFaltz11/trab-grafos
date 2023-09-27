#ifndef GrafoDefined
#define GrafoDefined

#include <iostream>
#include "No.h"
using namespace std;

class Grafo
{
private:
    No* raizGrafo;
    void AddNoArestaAux(int no1, int no2);
    bool direcionado;
    bool ponderado;
public:
    string nome;
    int unico;
    int duplo;
    Grafo(bool direcionado);
    ~Grafo();
    bool getPonderado(){return ponderado;}
    void AddNoAresta(int no);
    void AddNoAresta(int no1, int no2);
    No* procuraId(int id);
    void buscaProfundidade(int id);
};

#endif // GrafoDefined