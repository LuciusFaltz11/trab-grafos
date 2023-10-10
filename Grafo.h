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
    void AddNoArestaAux(int no1, int no2, int peso);
    void AddNoArestaAux(int no1, int no2);
    void AddNoArestaAux(int no);
    bool direcionado;
    bool ponderadoAresta;
    bool ponderadoVertice;
public:
    string nome;
    int unico;
    int duplo;
    Grafo(bool direcionado, int ponderadoId);
    ~Grafo();
    bool getPonderadoAresta(){return ponderadoAresta;}
    bool getPonderadoVertice(){return ponderadoVertice;}
    void AddNoAresta(int no1, int no2);
    void AddNoAresta(int no1, int no2, int peso);
    No* procuraId(int id);
    void buscaProfundidade(int id);
};

#endif // GrafoDefined