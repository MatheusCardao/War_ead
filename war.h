#ifndef WAR_H
#define WAR_H

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

typedef struct {
    char* missao;
    char cor[10];
} Jogador;

void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void exibirMissao(char* missao);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Jogador* jogadores, int qtd);

#endif
