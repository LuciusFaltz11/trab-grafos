#ifndef RotaDefined
#define RotaDefined

#include "No.h"
#include <cmath>
using namespace std;

class Rota
{
private:
    No *primeiroElemento = NULL;
    No *ultimoElemento = NULL;
    Rota *proxElemento = NULL;
    int capacidade = 100;
    int capacidadeAtual = 0;
    int nElementos = 0;
    double distanciaTotal = 0;

public:
    Rota(int capacidade)
    {
        if (DEBUG)
        {
            cout << "== < Chamou o construtor de rota > ==" << endl;
        }
        this->capacidade = capacidade;
        primeiroElemento = NULL;
        ultimoElemento = NULL;
        proxElemento = NULL;
    };
    ~Rota(){
        No *elementoNav = primeiroElemento;
        No *elementoAnterior = primeiroElemento;
        while (elementoNav != NULL)
        {
            elementoAnterior = elementoNav;
            elementoNav = elementoNav->getProxNo();
            delete elementoAnterior;
        }
    };
    void AddElemento(int id, int peso, int coordenadaX, int coordenadaY)
    {
        if (DEBUG)
        {
            cout << " == < chamou o add elemento > == " << endl;
        }
        // cout << "id: " << id << endl;
        // cout << "peso: " << peso << endl;
        // cout << "coordenadaX: " << coordenadaX << endl;
        // cout << "coordenadaY: " << coordenadaY << endl;

        nElementos++;
        No *novoNo = new No(id, peso, coordenadaX, coordenadaY);
        if (primeiroElemento == NULL)
        {
            primeiroElemento = novoNo;
            ultimoElemento = novoNo;
            return;
        }

        capacidadeAtual += peso;
        distanciaTotal += sqrt(pow(novoNo->getCoordenadaX() - ultimoElemento->getCoordenadaX(), 2) + pow(novoNo->getCoordenadaY() - ultimoElemento->getCoordenadaY(), 2));
        ultimoElemento->setProxNo(novoNo);
        ultimoElemento = novoNo;
    };
    float getDistanciaTotal() { return distanciaTotal; };
    No *getPrimeiroElemento() { return primeiroElemento; };
    No *getUltimoElemento() { return ultimoElemento; };
    No *getElemento(int posicao)
    {
        No *elementoNav = primeiroElemento;
        for (int i = 0; i < posicao; i++)
        {
            elementoNav = elementoNav->getProxNo();
        }
        return elementoNav;
    };
    int getNElementos() { return nElementos; };
    // void setNElementos(int nElementos){this->nElementos = nElementos;};
    int getCapacidade() { return capacidade; };
    int getCapacidadeAtual() { return capacidadeAtual; };
    void setCapacidade(int novaCapacidade) { this->capacidade = novaCapacidade; };
    void setProxElemento(Rota *prox) { proxElemento = prox; };
    Rota *getProxElemento() { return proxElemento; };
    bool podeAdicionarNo(int id, int demandaNo)
    {
        No *elementoNav = primeiroElemento;
        while (elementoNav != NULL)
        {
            if (elementoNav->getId() == id)
            {
                return false;
            }
            elementoNav = elementoNav->getProxNo();
        }

        // Verifique se adicionar o nó excederia a capacidade do veículo
        if (this->capacidadeAtual + demandaNo > this->capacidade)
        {
            return false;
        }

        return true;
    };
    void imprime()
    {
        No *elementoNav = primeiroElemento;
        while (elementoNav != NULL)
        {
            cout << elementoNav->getId() << " ";
            elementoNav = elementoNav->getProxNo();
        }
        cout << endl;
    };
    bool possuiNo(int id)
    {
        No *elementoNav = primeiroElemento;
        while (elementoNav != NULL)
        {
            if (elementoNav->getId() == id)
            {
                return true;
            }
            elementoNav = elementoNav->getProxNo();
        }
        return false;
    };
    Rota *somaRota(Rota *rota)
    {
        Rota *novaRota = new Rota(this->capacidade);
        novaRota->AddElemento(1, 0, 0, 0);
        No *elementoNav = primeiroElemento;
        while (elementoNav != NULL)
        {
            if (elementoNav->getId() != 1)
            {
                novaRota->AddElemento(elementoNav->getId(), elementoNav->getPeso(), elementoNav->getCoordenadaX(), elementoNav->getCoordenadaY());
            }
            elementoNav = elementoNav->getProxNo();
        }
        elementoNav = rota->getPrimeiroElemento();
        while (elementoNav != NULL)
        {
            if (elementoNav->getId() != 1)
            {
                novaRota->AddElemento(elementoNav->getId(), elementoNav->getPeso(), elementoNav->getCoordenadaX(), elementoNav->getCoordenadaY());
            }
            elementoNav = elementoNav->getProxNo();
        }
        novaRota->AddElemento(1, 0, 0, 0);
        if (DEBUG)
        {

            cout << "=========================" << endl;
            cout << "Rota 1: ";
            this->imprime();
            cout << "Rota 2: ";
            rota->imprime();
            cout << "Nova rota: ";
            novaRota->imprime();
            cout << "=========================" << endl;
        }
        return novaRota;
    };
};

#endif // RotaDefined
