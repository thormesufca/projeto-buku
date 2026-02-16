#include <stdio.h>
#include "matriz.h"
#include "pilha.h"
#include "jogo.h"

int main()
{
    

    Pilha *mao = criar();       
    Pilha *historico = criar();



    printf("========================================\n");
    printf("         BEM-VINDO AO JOGO BUKU        \n");
    printf("========================================\n");
    printf("Branco: joga em LINHAS, captura em casas brancas\n");
    printf("Preto: joga em COLUNAS, captura em casas pretas\n");
    printf("Captura: casas da sua cor com 3 ou 4 fichas\n");
    printf("Fim: todas as casas tem 0 ou 1 ficha\n");
    printf("Fim: Se um jogador selecionar uma linha vazia no seu turno, todas as fichas do tabuleiro vao para o adversario e o jogo termina\n");
    printf("========================================\n\n");

    int tamanho;
    printf("Digite um numero para o tamanho do tabuleiro\n");
    scanf(" %d", &tamanho);
    Tabuleiro *tabuleiro = cria(tamanho);

    // Criar jogadores
    char nome[20];
    printf("Digite o nome do primeiro jogador (Brancas). Ate 20 carateres:\n");
    scanf_s("%s", nome, sizeof(nome));
    while(getchar() != '\n');
    Jogador *branco = criar_jogador(nome, 0);
    
    printf("Digite o nome do segundo jogador (Pretas). Ate 20 carateres:\n");
    scanf_s("%s", nome, sizeof(nome));
    while(getchar() != '\n');
    Jogador *preto = criar_jogador(nome, 1);

    if (branco == NULL || preto == NULL)
    {
        printf("Erro ao criar jogadores!\n");
        return 1;
    }

    int turno = 1;
    int primeiro_turno_branco = 1;
    int resultado_jogada;

    // Loop principal
    while (1)
    {
        Jogador *jogador_atual = (turno % 2 == 1) ? branco : preto;

        print_tabuleiro(tabuleiro, historico);
        resultado_jogada = turno_jogador(tabuleiro, jogador_atual, mao, historico, primeiro_turno_branco);
        if(resultado_jogada == 2){
            Jogador *jogador_atual = (turno % 2 == 1) ? branco : preto;
            desistencia(tabuleiro, jogador_atual);
            break;
        } else if(resultado_jogada == 0){
            break;
        }
        if (turno == 1)
        {
            primeiro_turno_branco = 0;
        }
        turno++;
        limpar_pilha(mao);
    }

    // Fim de jogo
    print_tabuleiro(tabuleiro, historico);
    determinar_vencedor(branco, preto);

    // Liberar memoria
    liberar_jogador(branco);
    liberar_jogador(preto);
    destruir(mao);
    destruir(historico);
    libera(tabuleiro);
    return 0;
}
