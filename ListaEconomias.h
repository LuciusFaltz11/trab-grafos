#ifndef ListaEconomiasDefined
#define ListaEconomiasDefined
#include "ListaElemento.h"
#include "Economia.h"
class ListaEconomias
{
private:
    Economia *primeiroElemento = NULL;
    Economia *ultimoElemento = NULL;
    ListaEconomias *proxElemento = NULL;
    int nElementos;
    int capacidade = 100;

public:
    ListaEconomias(int capacidade)
    {
        this->capacidade = capacidade;
        nElementos = 0;
    };
    ~ListaEconomias()
    {
        Economia *elementoNav = primeiroElemento;
        Economia *elementoAnterior = primeiroElemento;
        while (elementoNav != NULL)
        {
            elementoAnterior = elementoNav;
            elementoNav = elementoNav->getProxElemento();
            delete elementoAnterior;
        }
    };
    void AddElemento(Economia *elemento)
    {
        nElementos++;
        if (primeiroElemento == NULL)
        {
            primeiroElemento = elemento;
            ultimoElemento = primeiroElemento;
            return;
        }
        ultimoElemento->setProxElemento(elemento);
        ultimoElemento = elemento;
    };
    Economia *getPrimeiroElemento()
    {
        return primeiroElemento;
    };
    Economia *getMaiorEconomia()
    {
        Economia *elementoNav = primeiroElemento;
        Economia *maiorEconomia = primeiroElemento;
        while (elementoNav != NULL)
        {
            if (elementoNav->getValor() > maiorEconomia->getValor())
            {
                maiorEconomia = elementoNav;
            }
            elementoNav = elementoNav->getProxElemento();
        }
        //* retorna uma copia da maior economia
        return new Economia(maiorEconomia->cloneRota(), maiorEconomia->getValor());
    };
    void removePrimeiroElemento()
    {
        nElementos--;
        Economia *aux = primeiroElemento;
        primeiroElemento = primeiroElemento->getProxElemento();
        delete aux;
    };
    Economia *getElemento(int posicao)
    {
        Economia *elementoNav = primeiroElemento;
        for (int i = 0; i < posicao; i++)
        {
            elementoNav = elementoNav->getProxElemento();
        }
        //* retornar uma copia do elementoNav
        return new Economia(elementoNav->cloneRota(), elementoNav->getValor());
    };
    int getNElementos() { return nElementos; };
    // void setNElementos(int nElementos){this->nElementos = nElementos;};
    void setProxElemento(ListaEconomias *prox) { proxElemento = prox; };
    ListaEconomias *getProxElemento() { return proxElemento; };
    void imprime()
    {
        Economia *elementoNav = primeiroElemento;
        while (elementoNav != NULL)
        {
            cout << elementoNav->getValor() << " ";
            elementoNav = elementoNav->getProxElemento();
        }
        cout << endl;
    };
    Economia *merge(Economia *left, Economia *right)
    {
        Economia *result = nullptr;

        if (left == nullptr)
            return right;
        if (right == nullptr)
            return left;

        if (left->getValor() >= right->getValor())
        { // Modificado para ordem decrescente (>=)
            result = left;
            result->setProxElemento(merge(left->getProxElemento(), right));
        }
        else
        {
            result = right;
            result->setProxElemento(merge(left, right->getProxElemento()));
        }

        return result;
    }

    void splitList(Economia *source, Economia **front, Economia **back)
    {
        Economia *fast;
        Economia *slow;

        // Caso base: lista vazia ou com um único elemento
        if (source == nullptr || source->getProxElemento() == nullptr)
        {
            *front = source;
            *back = nullptr;
        }
        else
        {
            slow = source;
            fast = source->getProxElemento();

            // 'fast' avança dois nós, 'slow' avança um nó
            while (fast != nullptr)
            {
                fast = fast->getProxElemento();
                if (fast != nullptr)
                {
                    slow = slow->getProxElemento();
                    fast = fast->getProxElemento();
                }
            }

            // 'slow' está na metade da lista, divide a lista em dois
            *front = source;
            *back = slow->getProxElemento();
            slow->setProxElemento(nullptr);
        }
    }
    void mergeSort(Economia **headRef)
    {
        Economia *head = *headRef;
        Economia *left;
        Economia *right;

        // Caso base: lista vazia ou com um único elemento
        if (head == nullptr || head->getProxElemento() == nullptr)
            return;

        // Divide a lista em duas partes
        splitList(head, &left, &right);

        // Ordena recursivamente as duas metades
        mergeSort(&left);
        mergeSort(&right);

        // Mescla as duas metades ordenadas
        *headRef = merge(left, right);
    }
    void sort()
    {

        mergeSort(&primeiroElemento);


        // Economia *elementoNav = primeiroElemento;
        // Economia *elementoNav2 = primeiroElemento;
        // Economia *aux;
        // while (elementoNav != NULL)
        // {
        //     elementoNav2 = primeiroElemento;
        //     while (elementoNav2 != NULL)
        //     {
        //         if (elementoNav->getValor() < elementoNav2->getValor())
        //         {
        //             aux = elementoNav->getProxElemento();
        //             elementoNav->setProxElemento(elementoNav2->getProxElemento());
        //             elementoNav2->setProxElemento(aux);
        //         }
        //         elementoNav2 = elementoNav2->getProxElemento();
        //     }
        //     elementoNav = elementoNav->getProxElemento();
        // }
    }
};

#endif // ListaEconomiasDefined
