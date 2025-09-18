#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "war.h"

// Sorteia e atribui uma missão
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Exibe a missão uma vez
void exibirMissao(char* missao) {
    printf("Sua missao: %s\n", missao);
}

// Verificação (lógica simplificada para teste)
int verificarMissao(char* missao, Territorio* mapa, int tamanho) {
    if(strstr(missao, "vermelha") != NULL) {
        // Jogador vence se nao houver tropas vermelhas
        for(int i=0; i<tamanho; i++) {
            if(strcmp(mapa[i].cor, "vermelho") == 0 && mapa[i].tropas > 0) {
                return 0; // ainda existe vermelho
            }
        }
        return 1; // venceu
    }
    // Missões genéricas simuladas
    return 0;
}

// Simulação de ataque (simplificada)
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    if(dadoA > dadoD) {
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        printf("%s conquistou %s!\n", atacante->nome, defensor->nome);
    } else {
        atacante->tropas--;
        printf("%s perdeu uma tropa!\n", atacante->nome);
    }
}

// Libera a memória alocada
void liberarMemoria(Jogador* jogadores, int qtd) {
    for(int i=0; i<qtd; i++) {
        free(jogadores[i].missao);
    }
    free(jogadores);
}
