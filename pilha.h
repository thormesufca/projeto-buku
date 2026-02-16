typedef struct pilha Pilha;
typedef struct elemento Elemento;

Pilha *criar();
int inserir(Pilha *pd);
int inserir_com_coords(Pilha *pd, int linha, int coluna);
int obter_coords_topo(Pilha *pd, int *linha, int *coluna);
int remover(Pilha *pd);
int tamanho(Pilha *pd);
int vazia(Pilha *pd);
void destruir(Pilha *pd);
