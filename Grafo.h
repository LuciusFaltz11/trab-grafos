#ifndef GrafoDefined
#define GrafoDefined

#include <iostream>
#include "No.h"
#include "Pilha.h"
#include "Lista.h"
using namespace std;

class Grafo
{
private:
    No* raizGrafo;
    void AddNoArestaAux(int no1, int no2);
    void AddNoArestaAux(int no);
    bool direcionado;
    bool ponderado;
public:
    string nome;
    int unico;
    int duplo;
    Grafo(bool direcionado);
    Grafo(bool direcionado, char ponderado);
    ~Grafo();
    bool getPonderado(){return ponderado;}
    void AddNoAresta(int no1, int no2);
    No* procuraId(int id);
    void buscaProfundidade(int id);
    void fechoTransitivoIndireto(int id);
};

#endif // GrafoDefined