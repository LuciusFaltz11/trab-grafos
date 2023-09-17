#include <iostream>
using namespace std;

class Grafo
{
private:
    /* data */
public:
string nome;
    int unico;
    int duplo;
    Grafo();
    ~Grafo();
    void AddNoAresta(int no);
    void AddNoAresta(int no1, int no2);
};
