#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "matriz.h"
#include "pilha.h"
#include "colors.h"
#include "jogo.h"
#include "teclas.h"

const int directions[4][2] = {
    [0] = {-1, 0}, // CIMA (W)
    [1] = {0, 1},  // DIREITA (D)
    [2] = {1, 0},  // BAIXO (S)
    [3] = {0, -1}  // ESQUERDA (A)
};

struct jogador
{
    char nome[20];
    char nome_posicao[7];
    int posicao; // 0 - Branco; 1 - Preto
    Pilha *mao;
};

int casa_visitada(Pilha *historico, int linha, int coluna);


void print_tabuleiro(Tabuleiro *matriz, Pilha *historico)
{
    if (matriz == NULL)
    {
        return;
    }
    if (linhas(matriz) < 1 || colunas(matriz) < 1)
    {
        return;
    }
    char fg[12];
    set_foreground_color(2, 0, 1, fg);
    char bg[12];
    set_background_color(3, bg);
    char reset[12];
    set_background_color(8, reset);
    printf("\n\t   ");
    for (int i = 0; i < colunas(matriz); i++)
    {
        printf("%s%s %02d %s", fg, bg, i, reset);
    }
    printf("\n");
    for (int i = 0; i < linhas(matriz); i++)
    {
        set_foreground_color(2, 1, 0, fg);
        set_background_color(6, bg);
        printf("%s%s\t%02d|", fg, bg, i);
        for (int j = 0; j < colunas(matriz); j++)
        {
            Pilha *pd;
            if (!acessar(matriz, i, j, &pd))
            {
                printf("Não foi possível acessar a pilha nessa posição");
                continue;
            };
            if ((i + j) % 2 == 0)
            { // Se soma é par (diagonais principais) (branco)
                set_foreground_color(1, 0, 0, fg);
                set_background_color(2, bg);
            }
            else
            { //(preto)
                set_foreground_color(2, 0, 0, fg);
                set_background_color(1, bg);
            }
            if (tamanho(pd) == 0)
            {
                printf("%s%s    %s", fg, bg, reset);
            }
            else
            {
                if (casa_visitada(historico, i, j))
                {
                    set_foreground_color(4, 0, 0, fg); // Fonte vermelha para indicar jogada
                }
                printf("%s%s %02d %s", fg, bg, tamanho(pd), reset);
            }
        }
        printf("\n%s", reset);
    }
}

void imprime_repeated_char(char letra, int vezes)
{
    for (int i = 0; i < vezes; i++)
    {
        printf("%c", letra);
    }
}

void imprime_erro(char *message)
{
    char bg[12];
    set_background_color(4, bg);
    char fg[12];
    set_foreground_color(5, 0, 0, fg);
    printf("%s%s", bg, fg);
    imprime_repeated_char(' ', strlen(message) + 2);
    printf("\n");
    printf(" %s \n", message);
    imprime_repeated_char(' ', strlen(message) + 2);
    set_background_color(9, bg);
    printf("\n%s", bg);
}

Jogador *criar_jogador(char *nome, int posicao)
{
    Jogador *j = (Jogador *)malloc(sizeof(Jogador));
    if (j == NULL)
        return NULL;
    strncpy(j->nome, nome, 19);
    j->nome[19] = '\0';
    j->posicao = posicao;
    if (posicao == 0)
    {
        strcpy(j->nome_posicao, "Branco");
    }
    else
    {
        strcpy(j->nome_posicao, "Preto");
    }
    j->mao = criar();
    return j;
}

void liberar_jogador(Jogador *j)
{
    if (j != NULL)
    {
        destruir(j->mao);
        free(j);
    }
}

int cor_casa(int linha, int coluna)
{
    return (linha + coluna) % 2; // 0 - branca; 1 - preta
}

int posicao_valida(Tabuleiro *t, int linha, int coluna)
{
    return linha >= 0 && linha < linhas(t) && coluna >= 0 && coluna < colunas(t);
}

int casa_visitada(Pilha *historico, int linha, int coluna)
{
    Pilha *temp = criar();
    int encontrou = 0;
    int l, c;

    // Retira os elementos da pilha para verificar as coordenadas
    while (!vazia(historico))
    {
        obter_coords_topo(historico, &l, &c);
        if (l == linha && c == coluna)
        {
            encontrou = 1;
        }
        inserir_com_coords(temp, l, c);
        remover(historico);
    }

    // Devolve os elementos para a pilha original. Remoção e reinserção em pilha mantém a ordem original.
    while (!vazia(temp))
    {
        obter_coords_topo(temp, &l, &c);
        inserir_com_coords(historico, l, c);
        remover(temp);
    }
    destruir(temp);

    return encontrou;
}

void limpar_pilha(Pilha *p)
{
    while (!vazia(p))
    {
        remover(p);
    }
}



int coletar_linha(Tabuleiro *t, int linha, Pilha *jogada)
{
    if (t == NULL || jogada == NULL)
        return 0;
    if (linha < 0 || linha >= linhas(t))
        return 0;

    int total = 0;
    for (int col = 0; col < colunas(t); col++)
    {
        Pilha *casa;
        if (acessar(t, linha, col, &casa))
        {
            while (!vazia(casa))
            {
                inserir(jogada);
                remover(casa);
                total++;
            }
        }
    }
    return total;
}

int coletar_coluna(Tabuleiro *t, int coluna, Pilha *jogada)
{
    if (t == NULL || jogada == NULL)
        return 0;
    if (coluna < 0 || coluna >= colunas(t))
        return 0;

    int total = 0;
    for (int lin = 0; lin < linhas(t); lin++)
    {
        Pilha *casa;
        if (acessar(t, lin, coluna, &casa))
        {
            while (!vazia(casa))
            {
                inserir(jogada);
                remover(casa);
                total++;
            }
        }
    }
    return total;
}

int coletar(Tabuleiro *t, int posicao, Pilha *jogada, Jogador *jogador)
{
    if (jogador->posicao == 0)
    { // branco, joga em linhas
        return coletar_linha(t, posicao, jogada);
    }
    else
    { // preto, joga em colunas
        return coletar_coluna(t, posicao, jogada);
    }
}

int semear_ficha(Tabuleiro *t, Pilha *jogada, int linha, int coluna)
{
    if (vazia(jogada))
        return 0;

    Pilha *casa;
    if (!acessar(t, linha, coluna, &casa))
        return 0;

    inserir(casa);
    remover(jogada);
    return 1;
}

int validar_movimento(Tabuleiro *t, int linha, int coluna, Pilha *historico)
{
    if (!posicao_valida(t, linha, coluna))
        return 0;
    if (casa_visitada(historico, linha, coluna))
        return 0;
    return 1;
}

/**
 * @brief Desfaz o último movimento registrado no histórico e devolve para a pilha de jogadas
 *
 * @param t Tabuleiro
 * @param jogada Pilha da jogada atual
 * @param historico Pilha com histórico das jogadas
 * @return int 1 => conseguiu desfazer; 0 => Não foi possível desfazer (pilha da jogada, casa ou histórico vazios)
 */
int desfazer_jogada(Tabuleiro *t, Pilha *jogada, Pilha *historico)
{
    if (vazia(historico))
        return 0;

    int linha, coluna;
    obter_coords_topo(historico, &linha, &coluna);

    Pilha *casa;
    if (!acessar(t, linha, coluna, &casa))
        return 0;

    if (vazia(casa))
        return 0;

    inserir(jogada);
    remover(casa);
    remover(historico);

    return 1;
}

int colheita(Tabuleiro *t, Jogador *jogador, int desistencia)
{
    int total_capturado = 0;
    int cor_jogador = jogador->posicao; // 0 = branco; 1 = preto

    for (int i = 0; i < linhas(t); i++)
    {
        for (int j = 0; j < colunas(t); j++)
        {
            if ((cor_casa(i, j) == jogador->posicao) || desistencia) // Colhe se a casa for do jogador ou então se houve desistência do outro jogador (coletar linha vazia)
            {
                Pilha *casa;
                if (acessar(t, i, j, &casa))
                {
                    int qtd = tamanho(casa);
                    if (qtd >= 3 || desistencia) // Se tiver mais que três fichas ou for em razão de desistência, pode colher a casa
                    {
                        // Move todas as fichas para a mao
                        while (!vazia(casa))
                        {
                            inserir(jogador->mao);
                            remover(casa);
                            total_capturado++;
                        }
                    }
                }
            }
        }
    }
    return total_capturado;
}

int jogo_terminado(Tabuleiro *t)
{
    for (int i = 0; i < linhas(t); i++)
    {
        for (int j = 0; j < colunas(t); j++)
        {
            Pilha *casa;
            if (acessar(t, i, j, &casa))
            {
                if (tamanho(casa) > 1)
                    return 0;
            }
        }
    }
    return 1;
}

int determinar_vencedor(Jogador *branco, Jogador *preto)
{
    int pecas_branco = tamanho(branco->mao);
    int pecas_preto = tamanho(preto->mao);

    printf("\n========== FIM DE JOGO ==========\n");
    printf("%s (%s) capturou %d fichas\n", branco->nome, branco->nome_posicao, pecas_branco);
    printf("%s (%s) capturou %d fichas\n", preto->nome, preto->nome_posicao, pecas_preto);

    if (pecas_branco > pecas_preto)
    {
        printf("\nVencedor: %s!\n", branco->nome);
        return 0;
    }
    else if (pecas_preto > pecas_branco)
    {
        printf("\nVencedor: %s!\n", preto->nome);
        return 1;
    }
    else
    {
        printf("\nEmpate!\n");
        return -1;
    }
}

int desistencia(Tabuleiro *t, Jogador *recebedor)
{
    return colheita(t, recebedor, 1);
}

// ==================== FUNCOES DE INTERFACE ====================

void mostrar_menu_direcoes()
{
    printf("Escolha uma direcao (WASD ou teclas de direcao): ");
    printf("\n\t\t[W] Cima\n");
    printf("[A] Esquerda\t\t\t[D] Direita\n");
    printf("\t\t[S] Baixo\n");
    printf("[Z] Desfazer\n");
}

char ler_direcao()
{
    char c;
    scanf(" %c", &c);
    return toupper(c);
}

int ler_posicao_coleta(Tabuleiro *t, Jogador *j)
{
    int pos;
    char bg[12];
    char reset[12];
    set_background_color(8, reset);
    if (j->posicao == 0)
    {
        set_background_color(6, bg);
        printf("Digite a %sLINHA%s para coletar (0-%d): ", bg, reset, linhas(t) - 1);
    }
    else
    {
        set_background_color(3, bg);

        printf("Digite a %sCOLUNA%s para coletar (0-%d): ", bg, reset, colunas(t) - 1);
    }
    int result = scanf("%d", &pos);
    while (getchar() != '\n')
        ;
    if (result != 1)
    {
        return -1;
    };
    return pos;
}

int ler_posicao_inicial(Tabuleiro *t, int *linha, int *coluna)
{
    printf("Digite a posicao inicial para semear:\n");
    char bg[12];
    char reset[12];
    set_background_color(8, reset);
    set_background_color(6, bg);
    printf("%sLinha%s (0-%d): ", bg, reset, linhas(t) - 1);
    scanf("%d", linha);
    while (getchar() != '\n')
        ;
    set_background_color(3, bg);
    printf("%sColuna%s (0-%d): ", bg, reset, colunas(t) - 1);
    scanf("%d", coluna);
    while (getchar() != '\n')
        ;
    return 1;
}

/**
 * Loop Principal
 * @return 0 => Jogo terminou
 * @return 1 => Próxima jogada
 * @return 2 => Jogador Desistiu (entregar fichas para adversário)
 */

int turno_jogador(Tabuleiro *t, Jogador *j, Pilha *jogada, Pilha *historico, int primeiro_turno_branco)
{
    // 1 - Adicionar 65a ficha no primeiro turno do branco
    if (primeiro_turno_branco && j->posicao == 0)
    {
        inserir(jogada);
        int tam = linhas(t) * colunas(t) + 1;
        printf("%da ficha adicionada a jogada!\n", tam);
    }

    printf("\n========== TURNO DE %s (%s) ==========\n", j->nome, j->nome_posicao);
    printf("Possui %d fichas\n", tamanho(j->mao));


    // 2 - Coletar linha ou coluna
    int pos_coleta;
    int coleta_valida = 0;

    while (!coleta_valida)
    {
        pos_coleta = ler_posicao_coleta(t, j);
        if (j->posicao == 0) // Verifica coleta jogador banco
        {
            if (pos_coleta < 0 || pos_coleta > linhas(t) - 1)
            {
                char msg[64];
                snprintf(msg, sizeof(msg), "Posicao invalida! Digite um valor entre 0 e %d.", linhas(t) - 1);
                imprime_erro(msg);
                continue;
            }
            if (coletar(t, pos_coleta, jogada, j) > 0)
                coleta_valida = 1;
            else
            {
                imprime_erro("Linha vazia! Jogador desistiu.");
                return 2;
            }
        }
        else // Verifica coleta jogador preto
        {
            if (pos_coleta < 0 || pos_coleta > colunas(t) - 1)
            {
                char msg[64];
                snprintf(msg, sizeof(msg), "Posicao invalida! Digite um valor entre 0 e %d.", colunas(t) - 1);
                imprime_erro(msg);
                continue;
            }
            if (coletar(t, pos_coleta, jogada, j) > 0)
                coleta_valida = 1;
            else
            {
                imprime_erro("Coluna vazia! Jogador desistiu.");
                return 2;
            }
        }
    }
    print_tabuleiro(t, historico);
    printf("Jogada de %s (%s)\nFichas na jogada: %d\n", j->nome, j->nome_posicao, tamanho(jogada));

    // 4 - Posicao inicial para semeadura
    int linha_atual, coluna_atual;
    int pos_inicial_valida = 0;

    while (!pos_inicial_valida)
    {
        ler_posicao_inicial(t, &linha_atual, &coluna_atual);

        if (posicao_valida(t, linha_atual, coluna_atual))
        {
            pos_inicial_valida = 1;
        }
        else
        {
            imprime_erro("Posicao invalida!");
        }
    }

    // 4 - Depositar primeira ficha
    semear_ficha(t, jogada, linha_atual, coluna_atual);       // Inserir no tabuleiro
    inserir_com_coords(historico, linha_atual, coluna_atual); // Inserir no histórico para desfazer se necessário
    print_tabuleiro(t, historico);
    printf("Jogada de %s (%s)\nFichas na jogada: %d\n", j->nome, j->nome_posicao, tamanho(jogada));

    // 5 - Loop de semeadura das demais fichas (andando no tabuleiro de acordo com as direções)
    while (!vazia(jogada))
    {

        mostrar_menu_direcoes();
        int direcao = ler_caracter();

        int nova_linha = linha_atual;
        int nova_coluna = coluna_atual;

        if (direcao == 99)
        {
            if (desfazer_jogada(t, jogada, historico))
            {
                // Atualiza posicao atual para a ultima do historico
                if (!vazia(historico))
                {
                    obter_coords_topo(historico, &linha_atual, &coluna_atual);
                }
                print_tabuleiro(t, historico);
                printf("Jogada de %s (%s)\nMovimento desfeito! Fichas na jogada: %d\n", j->nome, j->nome_posicao, tamanho(jogada));
            }
            else
            {
                imprime_erro("Nao ha movimento para desfazer!");
            }
            continue;
        }

        if (direcao >= 0)
        {
            nova_linha = linha_atual + directions[direcao][0];
            nova_coluna = coluna_atual + directions[direcao][1];

            if (!validar_movimento(t, nova_linha, nova_coluna, historico)) // Se movimento é inválido informa ao usuário e retorna para pegar novo movimento
            {
                if (!posicao_valida(t, nova_linha, nova_coluna))
                {
                    imprime_erro("Movimento fora do tabuleiro!");
                }
                else
                {
                    imprime_erro("Casa ja visitada neste turno!");
                }
                print_tabuleiro(t, historico);
                continue;
            }

            // Movimento válido
            linha_atual = nova_linha;
            coluna_atual = nova_coluna;
            semear_ficha(t, jogada, linha_atual, coluna_atual);       // Coloca ficha no tabuleiro
            inserir_com_coords(historico, linha_atual, coluna_atual); // Coloca ficha no histórico
            print_tabuleiro(t, historico);
            printf("Jogada de %s (%s)\nFichas na jogada: %d\n", j->nome, j->nome_posicao, tamanho(jogada));
        }
    }

    // 6 - Terminou o movimento, realiza captura de fichas
    int capturadas = colheita(t, j, 0);
    if (capturadas > 0)
    {
        printf("\n%s (%s) capturou %d fichas! Agora tem %d fichas\n", j->nome, j->nome_posicao, capturadas, tamanho(j->mao));
    }

    // 7 - Limpar historico
    limpar_pilha(historico);

    // 8 - Verificar se chegou ao fim
    if (jogo_terminado(t))
    {
        return 0;
    }

    return 1;
}
