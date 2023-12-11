Utilização do código

Para facilitar a execução de testes muito extensivos, criamos um script “BateriaDeTestes.sh” para nos auxiliar. Esse script funciona junto com um arquivo “args.txt”, que define os argumentos que serão passados a cada execução. Uma linha de execução do args tem a seguinte aparência: 

<Arquivo de entrada> <pasta de saida> <grafo direcionado> <peso nas arestas> <peso nos vértices> <nome do teste> <alfa selecionado>
./files/A-n34-k5.txt ./out 0 1 1 ReativoTeste10 0.50

Para rodar a bateria de testes basta utilizar o comando "bash BateriaDeTestes.sh"

Os resultados da execução do programa serão salvos em uma pasta dentro de “out/” com o nome do teste iniciado.
O tipo que algoritmo a ser rodado é definido pelo nome do teste:
Para guloso randomizado reativo, incluir “Reativo” no nome do teste. O alfa não será utilizado”
Para o guloso randomizado, incluir “Randomizado” no nome do teste e passar o alfa desejado como o último parâmetro.

Os arquivos gerados são: 

-> InformacoesGerais.txt: Contendo informações sobre os argumentos iniciais passados

-> MelhorGraphviz.txt: Um grafo em formato graphviz que pode ser visualizado. (por exemplo no site https://dreampuf.github.io/GraphvizOnline/)

-> MelhorLogsRotas.txt: informações sobre a melhor rota encontrada 

-> Resultado.txt: Informações relevantes como o tempo de execução, custo total e o conjunto de probabilidades de alfa.

Os parâmetros de número de iterações pode ser alterado no início do arquivo main:
//============================================================//
const int mesclarRotasIteracoes = 1;
const int iteracoesReativo = 100;
const int iteracoesRandomizado = 100;
const int iteracoesParaMelhoraFinalDeRotas = 1000;
const int repeticoesMediaAdaptativo = 5;
//============================================================//

-> mesclarRotasIteracoes: número de iterações do algoritmo que faz o merge das rotas. Caso seja igual a 1, será executado o algoritmo guloso simples, caso seja maior, será executado o algoritmo randomizado.

-> iteracoesParaMelhoraFinalDeRotas: número de execuções do algoritmo randomizado guloso de merge nas rotas finais.



O procedimento acima descreve o uso do grafo para a aplicação do caixeiro viajante. Para executar a primeira parte do projeto, basta rodar o código sem passar os argumentos. Todas as informações do grafo serão requisitadas via terminal.
