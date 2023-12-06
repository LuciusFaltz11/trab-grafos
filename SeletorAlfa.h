
#ifndef SeletorAlfa_defined
#define SeletorAlfa_defined



#include <cstdlib>
#include <iostream>
using namespace std;
class SeletorAlfa
{
private:
    int nAlfas;
    double *alfas;
    double *probabilidades;
    float melhorResultado;

public:
    SeletorAlfa()
    {
        this->nAlfas = 5;
        this->alfas = new double[5];
        alfas[0] = 0.05;
        alfas[1] = 0.1;
        alfas[2] = 0.15;
        alfas[3] = 0.3;
        alfas[4] = 0.5;
        
        this->probabilidades = new double[nAlfas];
        for(int i = 0; i < nAlfas; i++){
            probabilidades[i] = 1.0/nAlfas;
        }

    }
    ~SeletorAlfa(){
        delete[] alfas;
        delete[] probabilidades;
    };
    int selecionarAlfaIndex()
    {
        double r = (double)rand() / (double)RAND_MAX;
        double soma = 0;
        for (int i = 0; i < nAlfas; i++)
        {
            soma += probabilidades[i];
            if (r <= soma)
            {
                return i;
            }
        }
        return nAlfas - 1;
    };
    void atualizarProbabilidade(int alfaIndex, float resultado)
    {
        if(resultado > melhorResultado){
            melhorResultado = resultado;
        }
        const float fatorSuavizacao = 0.9;
        double soma = 0;
        for (int i = 0; i < nAlfas; i++)
        {
            soma += probabilidades[i];
        }
        probabilidades[alfaIndex] = (fatorSuavizacao * probabilidades[alfaIndex]) + ((1 - fatorSuavizacao) * (resultado / melhorResultado));
        // normalizar as probabilidades
        soma = 0;
        for (int i = 0; i < nAlfas; i++)
        {
            soma += probabilidades[i];
        }
        for (int i = 0; i < nAlfas; i++)
        {
            probabilidades[i] = probabilidades[i] / soma;
        }
    };
    
    double getAlfa(int i)
    {
        return alfas[i];
    };
    double getProbabilidade(int i){
        return probabilidades[i];
    };
    int getNAlfas(){
        return nAlfas;
    };
    void setProbabilidade(int i, double probabilidade){
        probabilidades[i] = probabilidade;
    };
    void setProbabilidades(double *probabilidades){
        this->probabilidades = probabilidades;
    };
    void setAlfas(double *alfas){
        this->alfas = alfas;
    };
    void setNAlfas(int nAlfas){
        this->nAlfas = nAlfas;
    };
    void imprimir(){
        for (int i = 0; i < nAlfas; i++)
        {
            cout << "Alfa: " << alfas[i] << " Probabilidade: " << probabilidades[i] << endl;    
        }
    };
};

#endif // SeletorAlfa_defined
