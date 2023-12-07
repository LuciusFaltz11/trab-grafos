#ifndef ListaJuncaoDefined
#define ListaJuncaoDefined
#include "Rota.h"
#include <unordered_set>
struct Juncao
{
    Rota *rota1;
    Rota *rota2;
    Rota *mesclada;
    float distancia;
    Juncao(Rota *rota1, Rota *rota2, float distancia, Rota *mesclada = NULL)
    {
        this->rota1 = rota1;
        this->rota2 = rota2;
        this->distancia = distancia;
        this->mesclada = mesclada;
    };
    ~Juncao(){};
    Juncao *proxJuncao = NULL;
    Juncao *getProxJuncao()
    {
        return proxJuncao;
    };
};

class ListaJuncao
{
private:
    Juncao *primeiroElemento = NULL;
    Juncao *ultimoElemento = NULL;
    int nElementos = 0;

public:
    ListaJuncao(){};
    ~ListaJuncao()
    {
        Juncao *elementoNav = primeiroElemento;
        Juncao *elementoAnterior = primeiroElemento;
        while (elementoNav != NULL)
        {
            elementoAnterior = elementoNav;
            elementoNav = elementoNav->proxJuncao;
            delete elementoAnterior;
        }
    };
    void AddElemento(Rota *rota1, Rota *rota2, float distancia, Rota *mesclada)
    {
        Juncao *elementoCopia = new Juncao(rota1, rota2, distancia, mesclada);
        nElementos++;
        if (primeiroElemento == NULL)
        {
            primeiroElemento = elementoCopia;
            ultimoElemento = primeiroElemento;
            return;
        }
        ultimoElemento->proxJuncao = elementoCopia;
        ultimoElemento = elementoCopia;
    };
    // {
    //     Juncao *elementoCopia = new Juncao(elemento->rota1, elemento->rota2, elemento->distancia);
    //     nElementos++;
    //     if (primeiroElemento == NULL)
    //     {
    //         primeiroElemento = elementoCopia;
    //         ultimoElemento = primeiroElemento;
    //         return;
    //     }
    //     ultimoElemento->proxJuncao = elementoCopia;
    //     ultimoElemento = elementoCopia;
    // };
    Juncao *getPrimeiroElemento()
    {
        return primeiroElemento;
    };
    Juncao *getUltimoElemento()
    {
        return ultimoElemento;
    };
    void RemoveElementoBaseadoNaRota(Juncao *elemento)
    {
        Juncao *elementoNav = primeiroElemento;
        Juncao *elementoAnterior = primeiroElemento;
        while (elementoNav != NULL)
        {
            if (
                elementoNav->rota1 == elemento->rota1 && elementoNav->rota2 == elemento->rota2 ||
                elementoNav->rota1 == elemento->rota2 && elementoNav->rota2 == elemento->rota1)
            {
                if (elementoNav == primeiroElemento)
                {
                    primeiroElemento = elementoNav->proxJuncao;
                    delete elementoNav;
                    nElementos--;
                    return;
                }
                if (elementoNav == ultimoElemento)
                {
                    ultimoElemento = elementoAnterior;
                    delete elementoNav;
                    nElementos--;
                    return;
                }
                elementoAnterior->proxJuncao = elementoNav->proxJuncao;
                delete elementoNav;
                nElementos--;
                return;
            }
            elementoAnterior = elementoNav;
            elementoNav = elementoNav->proxJuncao;
        }
    };
    void RemoveElemento(Juncao *elemento)
    {
        nElementos--;
        Juncao *elementoNav = primeiroElemento;
        Juncao *elementoAnterior = primeiroElemento;
        while (elementoNav != NULL)
        {
            if (elementoNav == elemento)
            {
                if (elementoNav == primeiroElemento)
                {
                    primeiroElemento = elementoNav->proxJuncao;
                    delete elementoNav;
                    return;
                }
                if (elementoNav == ultimoElemento)
                {
                    ultimoElemento = elementoAnterior;
                    delete elementoNav;
                    return;
                }
                elementoAnterior->proxJuncao = elementoNav->proxJuncao;
                delete elementoNav;
                return;
            }
            elementoAnterior = elementoNav;
            elementoNav = elementoNav->proxJuncao;
        }
    };
    Juncao *getJuncaoMaisProxima(ListaJuncao *listaJuncaoExceto = NULL)
    {

        Juncao *elementoNav = primeiroElemento;
        Juncao *juncaoMaisProxima = primeiroElemento;
        while (elementoNav != NULL)
        {
            // cout << "ta preso nesse while" << endl;
            if (elementoNav->distancia < juncaoMaisProxima->distancia && listaJuncaoExceto != NULL && !listaJuncaoExceto->contains(elementoNav))
            {
                juncaoMaisProxima = elementoNav;
            }
            elementoNav = elementoNav->proxJuncao;
        }
        // cout << "saiu do while " << endl;
        if (listaJuncaoExceto != NULL && listaJuncaoExceto->contains(juncaoMaisProxima))
        {
            return NULL;
        }
        return juncaoMaisProxima;
    };
    // Juncao *getJuncaoMaisProxima(ListaJuncao *listaJuncaoExceto = NULL)
    // {
    //     std::unordered_set<Juncao *> visited;
    //     Juncao *elementoNav = primeiroElemento;
    //     Juncao *juncaoMaisProxima = primeiroElemento;
    //     while (elementoNav != NULL)
    //     {
    //         // cout << "ta preso nesse while" << endl;
    //         if (visited.count(elementoNav))
    //         {
    //             break; // We've seen this element before, so we have a cycle
    //         }
    //         visited.insert(elementoNav);

    //         if (elementoNav->distancia < juncaoMaisProxima->distancia && listaJuncaoExceto != NULL && !listaJuncaoExceto->contains(elementoNav))
    //         {
    //             juncaoMaisProxima = elementoNav;
    //         }
    //         elementoNav = elementoNav->proxJuncao;
    //     }
    //     if (listaJuncaoExceto != NULL && listaJuncaoExceto->contains(juncaoMaisProxima))
    //     {
    //         return NULL;
    //     }
    //     return juncaoMaisProxima;
    // }

    Juncao* getJuncaoDasRotas(Rota* rota1, Rota* rota2){
        Juncao *elementoNav = primeiroElemento;
        while (elementoNav != NULL)
        {
            if (
                elementoNav->rota1 == rota1 && elementoNav->rota2 == rota2 ||
                elementoNav->rota1 == rota2 && elementoNav->rota2 == rota1)
            {
                return elementoNav;
            }
            elementoNav = elementoNav->proxJuncao;
        }
        return NULL;
    }

    bool contains(Rota* rota1, Rota* rota2)
    {
        if (nElementos == 0)
        {
            return false;
        }

        // std::unordered_set<Juncao *> visited;
        Juncao *elementoNav = primeiroElemento;
        while (elementoNav != NULL)
        {
            // if (visited.count(elementoNav))
            // {
            //     break; // We've seen this element before, so we have a cycle
            // }
            // visited.insert(elementoNav);

            if (
                elementoNav->rota1 == rota1 && elementoNav->rota2 == rota2 ||
                elementoNav->rota1 == rota2 && elementoNav->rota2 == rota1)
            {
                return true;
            }
            elementoNav = elementoNav->proxJuncao;
        }
        return false;
    }
    bool contains(Juncao *juncao)
    {
        if (nElementos == 0)
        {
            return false;
        }

        std::unordered_set<Juncao *> visited;
        Juncao *elementoNav = primeiroElemento;
        while (elementoNav != NULL)
        {
            if (visited.count(elementoNav))
            {
                break; // We've seen this element before, so we have a cycle
            }
            visited.insert(elementoNav);

            if (
                elementoNav->rota1 == juncao->rota1 && elementoNav->rota2 == juncao->rota2 ||
                elementoNav->rota1 == juncao->rota2 && elementoNav->rota2 == juncao->rota1)
            {
                return true;
            }
            elementoNav = elementoNav->proxJuncao;
        }
        return false;
    }
    // bool contains(Juncao *juncao)
    // {
    //     if(nElementos == 0){
    //         return false;
    //     }

    //     Juncao *elementoNav = primeiroElemento;
    //     while (elementoNav != NULL)
    //     {
    //         if (elementoNav == juncao)
    //         {
    //             return true;
    //         }
    //         // cout << "elementoNav " << (elementoNav == NULL) << endl;
    //         elementoNav = elementoNav->proxJuncao;
    //     }
    //     return false;
    // };
    void ordenaLista()
    {
        Juncao *elementoNav = primeiroElemento;
        Juncao *elementoNav2 = primeiroElemento;
        Juncao *juncaoMaisProxima = primeiroElemento;
        Juncao *juncaoMaisProxima2 = primeiroElemento;
        while (elementoNav != NULL)
        {
            elementoNav2 = elementoNav;
            juncaoMaisProxima = elementoNav;
            juncaoMaisProxima2 = elementoNav;
            while (elementoNav2 != NULL)
            {
                if (elementoNav2->distancia < juncaoMaisProxima->distancia)
                {
                    juncaoMaisProxima = elementoNav2;
                }
                elementoNav2 = elementoNav2->proxJuncao;
            }
            juncaoMaisProxima2->distancia = elementoNav->distancia;
            elementoNav->distancia = juncaoMaisProxima->distancia;
            juncaoMaisProxima->distancia = juncaoMaisProxima2->distancia;
            elementoNav = elementoNav->proxJuncao;
        }
    };

    Juncao *getJuncao(int index)
    {
        Juncao *elementoNav = primeiroElemento;
        for (int i = 0; i < index; i++)
        {
            elementoNav = elementoNav->proxJuncao;
        }
        return elementoNav;
    };
    int getNElementos()
    {
        return nElementos;
    };
    ListaJuncao *getTopNJuncoesPorProximidade(int n, ListaJuncao *listaJuncaoExceto = NULL)
    {
        ListaJuncao *result = new ListaJuncao();

        //* adicionar todos os elementos da lista original na lista exceto no result
        if (listaJuncaoExceto != NULL)
        {
            Juncao *elementoNav = listaJuncaoExceto->getPrimeiroElemento();
            while (elementoNav != NULL)
            {
                result->AddElemento(elementoNav->rota1, elementoNav->rota2, elementoNav->distancia, elementoNav->mesclada);
                elementoNav = elementoNav->proxJuncao;
            }
        }

        for (int i = 0; i < n; i++)
        {
            Juncao *juncaoMaisProxima = getJuncaoMaisProxima(result);
            if (juncaoMaisProxima == NULL)
            {
                break;
            }

            result->AddElemento(juncaoMaisProxima->rota1, juncaoMaisProxima->rota2, juncaoMaisProxima->distancia, juncaoMaisProxima->mesclada);
        }
        return result;
    }
};

#endif // ListaJuncaoDefined
