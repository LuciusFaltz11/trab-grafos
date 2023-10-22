#define LISTAORDENAARESTA_H
#include "ArestaLista.h"


class ListaOrdenaAresta{
    private:
    ArestaLista* primeira;
    ArestaLista* ultima;
    int totalArestas = 0;

    public:
    bool existeAresta(int origemNovo, int destinoNovo);
    ListaOrdenaAresta();
    ~ListaOrdenaAresta();
    void addAresta(int origem, int destino, int peso);
    void ordenaLista();
    void imprimeListaOrdenada();
};
