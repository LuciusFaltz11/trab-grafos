#ifndef GrafoDefined
#define GrafoDefined

#include <iostream>
#include <stdarg.h>
#include <fstream>
#include "No.h"
#include "Pilha.h"
#include "PilhaArestas.h"
#include "PilhaArestasElemento.h"
#include "Lista.h"
#include "ListaOrdenaAresta.h"
using namespace std;

struct Subarvore
{
    int *nos; // nos da subarvore
    int tam;  // tamanho atual do vetor
    int max;  // capacidade máxima
};
class Grafo
{
private:
    No *raizGrafo;
    No *ultimoNo;
    int totalNos = 0;
    void AddNoArestaAux(int no1, int no2, int peso);
    // void AddNoArestaAux(int no1, int no2);
    void AddNo(int no);
    bool direcionado;
    bool ponderadoAresta;
    bool ponderadoVertice;
    int ponderadoId;
    void criaListaOrdenadaAresta(ListaOrdenaAresta *lista, bool direcionado);
    // int encontrarSubarvore(Lista *vetorNos[], int id);
    // int encontrarSubarvore(int id, int parent[]);
    // void criaSubarvoreNos(Lista *subarvoreNos[]);
    // bool avaliaSubarvores(int no1, int no2, Lista *subarvoreNos[]);
    // bool unirSubarvores(int u, int v, int parent[]);
    // void unirSubarvores(int no1, int no2, Lista *subarvoreNos[]);
    void criarSubarvores(Subarvore subarvore[]);
    void adicionarNo(Subarvore &subarvore, int no);
    void liberarSubarvore(Subarvore &subarvore);
    int encontraSubarvore(int id, Subarvore *vetorSub);
    void unirSubarvores(int idxArvU, int idxArvV, Subarvore *vetorSub);
    void imprimirSubarvores(Subarvore vetorNos[]);

public:
    string nome;
    int unico;
    int duplo;
    Grafo(bool direcionado, int ponderadoId);
    ~Grafo();
    bool getPonderadoAresta() { return ponderadoAresta; }
    bool getPonderadoVertice() { return ponderadoVertice; }
    void AddNoAresta(int no1, int no2);
    void AddNoAresta(int no1, int no2, int peso);
    No *procuraId(int id);
    Lista *buscaProfundidade(int id);
    Lista *getArestasNo(int id);
    bool iterate(bool (*func)(int, int));
    void generateDreampufFile(string filename);
    Grafo *inverteArestasDirecionadas();
    void arvoreProfundidade(int id);
    void arvoreProfundidade(int id, bool generateDreampufFile);
    void arvoreMinimaKruskal();
    void setTotalNos(int qtd) { this->totalNos = qtd; };
    void arvoreMinimaPrim();
};

#endif // GrafoDefined