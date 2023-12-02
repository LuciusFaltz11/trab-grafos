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
    void AddNo(int no);
    bool direcionado;
    bool ponderadoAresta;
    bool ponderadoVertice;
    int ponderadoId;
    void criaListaOrdenadaAresta(ListaOrdenaAresta *lista, bool direcionado, Grafo *subGrafo);
    void criarSubarvores(Subarvore subarvore[]);
    void adicionarNo(Subarvore &subarvore, int no);
    void liberarSubarvore(Subarvore &subarvore);
    int encontraSubarvore(int id, Subarvore *vetorSub);
    void unirSubarvores(int idxArvU, int idxArvV, Subarvore *vetorSub);
    void imprimirSubarvores(Subarvore vetorNos[]);
    bool nosPertencemSubarvore(int raizU, int raizV, Subarvore *vetorSub, int tamVetorSub);
    bool temAresta(int no1, int no2);
    void gerarSubgrafoInduzido(Lista *vertices, Grafo *&subgrafo);

public:
    string nome;
    int unico;
    int duplo;
    Grafo(bool direcionado, bool ponderadoAresta, bool ponderadoVertice);
    ~Grafo();
    No *getRaizGrafo() { return raizGrafo; }
    bool getPonderadoAresta() { return ponderadoAresta; }
    bool getPonderadoVertice() { return ponderadoVertice; }
    void AddNoCoord(int no, int coordenadaX, int coordenadaY);
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
    void arvoreMinimaKruskal(Lista *vertices);
    void setTotalNos(int qtd) { this->totalNos = qtd; };
    void arvoreMinimaPrim();
    void AddPesoAoNo(int no, int peso);

};

#endif // GrafoDefined