#include "Grafo.h"

Grafo::Grafo()
{
    unico = 0;
    duplo = 0;
}

Grafo::~Grafo()
{
}

void Grafo::AddNoAresta(int no)
{
    unico += 1;
    // cout << nome <<": Foi passado apenas o no: " << no << endl;
}
void Grafo::AddNoAresta(int no1, int no2)
{
    if(no1 == no2){
        cout << "tem coisa igual: " << no1 << " e " << no2 << endl;
    }
    duplo += 1;
    // cout << nome <<": Foram passados dois nos: " << no1 << " e " << no2 << endl;
}