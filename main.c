#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_MISSOES 5
#define TOTAL_TERRITORIOS 10

// ------------------------- ESTRUTURAS -------------------------
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

typedef struct {
    char* missao;   // armazenada dinamicamente
    char cor[10];
} Jogador;

// ------------------------- FUNÇÕES -------------------------
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho);
void exibirMissao(char* missao);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Jogador* jogadores, int qtd);

// ------------------------- MAIN -------------------------
int main() {
    srand(time(NULL));

    // Vetor de missões
    char* missoes[TOTAL_MISSOES] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Manter pelo menos 2 territorios com 5 tropas",
        "Conquistar o territorio Central",
        "Sobreviver ate o final de 5 rodadas"
    };

    int qtdJogadores = 2;
    Jogador* jogadores = (Jogador*) calloc(qtdJogadores, sizeof(Jogador));

    // Atribuindo missões
    for(int i=0; i<qtdJogadores; i++) {
        jogadores[i].missao = (char*) malloc(100 * sizeof(char));
        atribuirMissao(jogadores[i].missao, missoes, TOTAL_MISSOES);
        printf("Jogador %d recebeu sua missao!\n", i+1);
        exibirMissao(jogadores[i].missao);
        printf("\n");
    }

    // Criando um mapa simples
    Territorio mapa[TOTAL_TERRITORIOS];
    for(int i=0; i<TOTAL_TERRITORIOS; i++) {
        sprintf(mapa[i].nome, "Territorio%d", i+1);
        strcpy(mapa[i].cor, (i%2==0) ? "azul" : "vermelho");
        mapa[i].tropas = (rand() % 5) + 1;
    }

    // Simulação simples de checagem ao fim do turno
    for(int i=0; i<qtdJogadores; i++) {
        if(verificarMissao(jogadores[i].missao, mapa, TOTAL_TERRITORIOS)) {
            printf("\n*** Jogador %d venceu cumprindo sua missao! ***\n", i+1);
            break;
        }
    }

    liberarMemoria(jogadores, qtdJogadores);
    return 0;
}