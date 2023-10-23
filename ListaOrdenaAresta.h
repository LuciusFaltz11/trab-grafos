#define LISTAORDENAARESTA_H
#include "ArestaLista.h"

class ListaOrdenaAresta
{
private:
    ArestaLista *primeira;
    ArestaLista *ultima;
    int totalArestas = 0;
    void merge(ArestaLista *&cabeca, ArestaLista *esquerda, ArestaLista *direita);
    void mergeSort(ArestaLista *&cabeca);
    void split(ArestaLista *origem, ArestaLista *&frente, ArestaLista *&tras);

public:
    bool existeAresta(int origemNovo, int destinoNovo);
    ListaOrdenaAresta();
    ~ListaOrdenaAresta();
    void addAresta(int origem, int destino, int peso);
    void ordenaLista();
    void imprimeListaOrdenada();
    int getTotalArestas();
};
