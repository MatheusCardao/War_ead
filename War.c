// war_estruturado.c
// Desenvolvido para o desafio: WAR estruturado com ataques entre territórios
// Requisitos atendidos: alocação dinâmica, uso de ponteiros, modularização, srand/rand, validações e free()

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR  10

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int  tropas;
} Territorio;

/* -------------------- Utilidades de IO -------------------- */

// Lê uma linha do stdin (consome '\n') de forma segura.
static void lerLinha(char *buf, size_t tam) {
    if (!fgets(buf, (int)tam, stdin)) {
        // EOF ou erro: garante string vazia
        buf[0] = '\0';
        return;
    }
    size_t n = strlen(buf);
    if (n > 0 && buf[n - 1] == '\n') buf[n - 1] = '\0';
}

// Lê um inteiro com prompt e valida.
static int lerInteiro(const char *prompt, int min, int max) {
    char linha[64];
    int valor;
    while (1) {
        printf("%s", prompt);
        lerLinha(linha, sizeof(linha));
        if (sscanf(linha, "%d", &valor) == 1 && valor >= min && valor <= max) {
            return valor;
        }
        printf("Entrada inválida. Informe um número entre %d e %d.\n", min, max);
    }
}

/* -------------------- Operações de Território -------------------- */

// Cadastra N territórios no vetor alocado dinamicamente.
void cadastrarTerritorios(Territorio *mapa, int n) {
    printf("\n=== Cadastro de Territórios (%d) ===\n", n);
    for (int i = 0; i < n; i++) {
        printf("\nTerritório #%d\n", i);
        printf("Nome (até %d chars): ", MAX_NOME - 1);
        lerLinha(mapa[i].nome, sizeof(mapa[i].nome));

        printf("Cor do exército (até %d chars): ", MAX_COR - 1);
        lerLinha(mapa[i].cor, sizeof(mapa[i].cor));

        mapa[i].tropas = lerInteiro("Quantidade de tropas (>= 0): ", 0, 1000000);
    }
}

// Exibe todos os territórios com índice, dono e tropas.
void exibirMapa(const Territorio *mapa, int n) {
    printf("\n=== Mapa de Territórios ===\n");
    printf("%-4s | %-28s | %-10s | %-6s\n", "ID", "Nome", "Cor", "Tropas");
    printf("-----+------------------------------+------------+--------\n");
    for (int i = 0; i < n; i++) {
        printf("%-4d | %-28s | %-10s | %-6d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Busca índice pelo nome (case-sensitive). Retorna -1 se não achar.
int buscarIndicePorNome(const Territorio *mapa, int n, const char *nome) {
    for (int i = 0; i < n; i++) {
        if (strcmp(mapa[i].nome, nome) == 0) return i;
    }
    return -1;
}

// Lista os territórios de uma determinada cor (auxilia o jogador).
void listarPorCor(const Territorio *mapa, int n, const char *cor) {
    printf("\nTerritórios da cor \"%s\":\n", cor);
    int achou = 0;
    for (int i = 0; i < n; i++) {
        if (strcmp(mapa[i].cor, cor) == 0) {
            printf("  ID %-3d  %-28s (tropas: %d)\n", i, mapa[i].nome, mapa[i].tropas);
            achou = 1;
        }
    }
    if (!achou) {
        printf("  (nenhum)\n");
    }
}

/* -------------------- Lógica de Batalha -------------------- */
/*
    Regras escolhidas (claras e simples):
    - Cada lado rola 1 dado (1..6) usando rand().
    - Empate favorece o DEFENSOR (como no WAR clássico).
    - Se o atacante vencer:
        * O defensor MUDA DE DONO: copia a cor do atacante.
        * Metade das tropas do atacante (arredondando para baixo) move para o território conquistado.
          Ex.: atacante=7 => move 3, atacante passa a ter 4, defensor passa a ter 3.
        * Se a metade for 0 (atacante tinha 1 tropa), impedimos ataque na validação (precisa de >=2).
    - Se o atacante perder:
        * O atacante perde 1 tropa (mínimo 0).
*/
void atacar(Territorio *atacante, Territorio *defensor) {
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\n>>> Batalha: %s (%s, %d) ATACA %s (%s, %d)\n",
           atacante->nome, atacante->cor, atacante->tropas,
           defensor->nome, defensor->cor, defensor->tropas);

    printf("Dados: Atacante=%d  Defensor=%d  ", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        // Vitória do atacante => conquista
        int mover = atacante->tropas / 2; // metade (floor)
        if (mover < 1) mover = 1; // fallback para não deixar conquistado vazio (garantia adicional)
        if (mover >= atacante->tropas) mover = atacante->tropas - 1; // mantém pelo menos 1 no atacante

        printf("-> Atacante VENCEU! Conquista o território.\n");
        // Transfere dono (cor) e tropas
        strncpy(defensor->cor, atacante->cor, MAX_COR - 1);
        defensor->cor[MAX_COR - 1] = '\0';
        defensor->tropas = mover;

        atacante->tropas -= mover;

    } else {
        // Empate ou defensor maior: defensor vence
        printf("-> Defensor RESISTE! Atacante perde 1 tropa.\n");
        if (atacante->tropas > 0) atacante->tropas -= 1;
    }

    printf("Após a batalha:\n");
    printf("  Atacante: %s (%s) - tropas: %d\n", atacante->nome, atacante->cor, atacante->tropas);
    printf("  Defensor: %s (%s) - tropas: %d\n", defensor->nome, defensor->cor, defensor->tropas);
}

/* -------------------- Menu de Ataque (Interface Amigável) -------------------- */

static int escolherIndice(const Territorio *mapa, int n, const char *prompt) {
    // Usuário pode escolher por ID ou por nome
    char linha[64];
    printf("%s (digite ID numérico ou nome exato): ", prompt);
    lerLinha(linha, sizeof(linha));

    // Tenta interpretar como ID
    int id;
    if (sscanf(linha, "%d", &id) == 1) {
        if (id >= 0 && id < n) return id;
        printf("ID fora do intervalo.\n");
        return -1;
    }

    // Caso contrário, trata como nome
    int idx = buscarIndicePorNome(mapa, n, linha);
    if (idx == -1) {
        printf("Não encontrado pelo nome informado.\n");
    }
    return idx;
}

// Fluxo de ataque com validações e mensagens claras.
void fluxoAtaque(Territorio *mapa, int n) {
    if (n < 2) {
        printf("É necessário pelo menos 2 territórios para atacar.\n");
        return;
    }

    exibirMapa(mapa, n);

    // Ajuda: listar por cor do atacante
    char corAtacante[MAX_COR];
    printf("\nInforme sua cor para facilitar a escolha (opcional, ENTER para pular): ");
    lerLinha(corAtacante, sizeof(corAtacante));
    if (strlen(corAtacante) > 0) {
        listarPorCor(mapa, n, corAtacante);
    }

    int idxA = escolherIndice(mapa, n, "\nEscolha o TERRITÓRIO ATACANTE");
    if (idxA < 0) return;

    int idxD = escolherIndice(mapa, n, "Escolha o TERRITÓRIO DEFENSOR");
    if (idxD < 0) return;

    if (idxA == idxD) {
        printf("Um território não pode atacar a si mesmo.\n");
        return;
    }

    Territorio *atacante = &mapa[idxA];
    Territorio *defensor  = &mapa[idxD];

    // Validações conforme requisitos
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Jogada inválida: não é permitido atacar um território da PRÓPRIA cor.\n");
        return;
    }
    if (atacante->tropas < 2) {
        printf("Atacante com tropas insuficientes. Necessário ao menos 2 tropas para atacar.\n");
        return;
    }
    if (defensor->tropas < 1) {
        printf("Defensor sem tropas (ocupação inválida). Ataque não executado.\n");
        return;
    }

    atacar(atacante, defensor);
    exibirMapa(mapa, n);
}

/* -------------------- Liberação de Memória -------------------- */

void liberarMemoria(Territorio *mapa) {
    free(mapa);
}

/* -------------------- Main -------------------- */

int main(void) {
    // Semente para aleatoriedade
    srand((unsigned)time(NULL));

    printf("=== WAR Estruturado — Simulador de Ataques ===\n");

    int n = lerInteiro("Informe o número total de territórios: ", 1, 10000);

    // Alocação dinâmica do vetor de territórios
    Territorio *mapa = (Territorio *)calloc((size_t)n, sizeof(Territorio));
    if (!mapa) {
        fprintf(stderr, "Falha ao alocar memória para os territórios.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);

    int opcao;
    do {
        printf("\n=== Menu ===\n");
        printf("1) Exibir mapa\n");
        printf("2) Realizar ataque\n");
        printf("3) Buscar território por nome\n");
        printf("0) Sair\n");
        opcao = lerInteiro("Escolha: ", 0, 3);

        switch (opcao) {
            case 1:
                exibirMapa(mapa, n);
                break;
            case 2:
                fluxoAtaque(mapa, n);
                break;
            case 3: {
                char busca[MAX_NOME];
                printf("Nome do território: ");
                lerLinha(busca, sizeof(busca));
                int idx = buscarIndicePorNome(mapa, n, busca);
                if (idx >= 0) {
                    printf("Encontrado (ID %d): %s | Cor: %s | Tropas: %d\n",
                           idx, mapa[idx].nome, mapa[idx].cor, mapa[idx].tropas);
                } else {
                    printf("Não encontrado.\n");
                }
                break;
            }
            case 0:
                break;
        }
    } while (opcao != 0);

    liberarMemoria(mapa);
    printf("Memória liberada. Até mais!\n");
    return 0;
}
