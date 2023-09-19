#ifndef GrafoDefined
#define GrafoDefined

#include <iostream>
#include "No.h"
using namespace std;

class Grafo
{
private:
    No* raizGrafo;
public:
    string nome;
    int unico;
    int duplo;
    Grafo();
    ~Grafo();
    void AddNoAresta(int no);
    void AddNoAresta(int no1, int no2);
    No* procuraId(int id);
};

#endif // GrafoDefined