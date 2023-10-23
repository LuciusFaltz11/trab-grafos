#include "ListaOrdenaAresta.h"
#include <iostream>

using namespace std;

ListaOrdenaAresta::ListaOrdenaAresta(){
    primeira = NULL;
    ultima = NULL;
    totalArestas = 0;
}

ListaOrdenaAresta::~ListaOrdenaAresta(){
    ArestaLista* arestaDeleta;
    while(primeira != NULL){
        arestaDeleta = primeira;
        primeira = primeira->getProxima();
        delete arestaDeleta;
        totalArestas--;
    }
}

bool ListaOrdenaAresta::existeAresta(int origemNovo, int destinoNovo) {
    ArestaLista* aux = primeira;
    while (aux != NULL) {
        if (aux->getOrigem() == origemNovo && aux->getDestino() == destinoNovo) {
            return true;
        }
        aux = aux->getProxima();
    }
    return false;
}

void ListaOrdenaAresta::addAresta(int origem, int destino, int peso) {
    ArestaLista* aux = primeira;
    while (aux != NULL) {
        if (aux->getOrigem() == origem && aux->getDestino() == destino) {
            return;
        }
        aux = aux->getProxima();
    }

    // A aresta não existe
    ArestaLista* novaAresta = new ArestaLista(origem, destino, peso);
    if (ultima == NULL) {
        primeira = novaAresta;
        ultima = primeira;
    } else {
        ultima->setProxima(novaAresta);
        ultima = ultima->getProxima();
    }
    totalArestas++;
}

void ListaOrdenaAresta::imprimeListaOrdenada(){
    ArestaLista* aux = primeira;
    while(aux != NULL){
        cout << "origem: " << aux->getOrigem() << " destino: " << aux->getDestino() << " peso: " << aux->getPeso() << endl;
        aux = aux->getProxima();
    }
}