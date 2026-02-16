#include "matriz.h"
#include "pilha.h"

typedef struct jogador Jogador;


Jogador *criar_jogador(char *nome, int posicao);
void liberar_jogador(Jogador *j);


int coletar(Tabuleiro *t, int posicao, Pilha *jogada, Jogador *j);



int semear_ficha(Tabuleiro *t, Pilha *mao, int linha, int coluna);
int validar_movimento(Tabuleiro *t, int linha, int coluna, Pilha *historico);


int desfazer_jogada(Tabuleiro *t, Pilha *mao, Pilha *historico);


int colheita(Tabuleiro *t, Jogador **jogador, int desistencia);
int cor_casa(int linha, int coluna);


int turno_jogador(Tabuleiro *t, Jogador *j, Pilha *mao, Pilha *historico, int primeiro_turno_branco);
int jogo_terminado(Tabuleiro *t);
int determinar_vencedor(Jogador *branco, Jogador *preto);
int desistencia(Tabuleiro *t, Jogador *recebedor);


void print_tabuleiro(Tabuleiro *matriz, Pilha *historico);
void imprime_erro(char *message);