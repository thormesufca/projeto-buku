typedef struct matriz Tabuleiro;
typedef struct pilha Pilha;

/* Funções exportadas */
/* Função cria
** Aloca e retorna uma matriz de dimensão m por n com pilhas em cada posição
*/
Tabuleiro* cria(int tamanho);

/* Função libera
** Libera a memória de uma matriz previamente criada.
*/
void libera(Tabuleiro *);

/* Função preenche
** Pergunta ao usuário para cada posição da matriz que valor deve ser colocado
*/
void preenche(Tabuleiro *);

/* Função print
** Imprime todos os itens da matriz no console
*/
void print_matriz(Tabuleiro *);

/* Função linhas
** Retorna o número de linhas da matriz
*/
int linhas(Tabuleiro *);

/* Função colunas
** Retorna o número de colunas da matriz
*/
int colunas(Tabuleiro *);

/* Função acessar
** Acessa a pilha na linha e coluna indicada na matriz e a seta no ponteiro indicado
*/
int acessar(Tabuleiro *matriz, int linha, int coluna, Pilha **pd);


int esvazia_espaco(Tabuleiro *matriz, int linha, int coluna);
