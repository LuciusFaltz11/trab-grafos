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

    No* noAddAresta = procuraId(no1);

    if(noAddAresta == nullptr){ // no nao existe, logo insere ao fim da lista de nos
        No* listNos = raizGrafo;
        while(listNos != NULL){
            listNos->getProxNo();
        }

        No* novoNo = new No(no1);
        novoNo->setPrimeiraAresta(no2);
    } else{
        //verificar se já existe a aresta ----> criar função


        noAddAresta->getUltimaAresta()->setProxAresta(no2); // criar a função de adicionar aresta
    }


}

No* Grafo::procuraId(int id){ // se encontrar o no, retorna um ponteiro para procurar a lista de aresta em outras funcoes
    No* nosGrafo = raizGrafo;

    while(nosGrafo != NULL){
        if(nosGrafo->getId() == id){
            return nosGrafo;
        }
        
        nosGrafo->getProxNo();
    }
    return NULL;
}