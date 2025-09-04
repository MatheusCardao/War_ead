// war_novato_minimo.c — Nível Novato
// Objetivo: cadastrar 5 territórios usando struct e exibir seus dados.
// Requisitos atendidos:
//  - struct Territorio com campos: char nome[30], char cor[10], int tropas
//  - vetor de 5 elementos (Territorio territórios[5])
//  - entrada de dados via terminal usando scanf (com cuidado para strings)
//  - exibição formatada após o cadastro
//  - comentários explicativos e nomes claros para manutenibilidade
//
// Compilação: gcc -std=c11 -Wall -Wextra -O2 war_novato_minimo.c -o war_novato
// Execução:   ./war_novato

#include <stdio.h>   // printf, scanf
#include <string.h>  // (opcional aqui) utilidades para strings

#define QTDE_TERRITORIOS 5

// Estrutura composta que agrupa dados relacionados de um território.
// nome[30]  : até 29 caracteres + terminador '\0'
// cor[10]   : até 9 caracteres + terminador '\0' (ex.: "Vermelho", "Azul")
// tropas    : quantidade de exércitos/peças no território
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main(void) {
    Territorio territorios[QTDE_TERRITORIOS]; // vetor de structs (5 posições)

    printf("\n=== Cadastro de Territórios (Nível Novato) ===\n");
    printf("Você irá cadastrar %d territórios. Para o nome, pode usar espaços.\n", QTDE_TERRITORIOS);
    printf("A cor aceita até 9 letras (ex.: Vermelho, Azul). Tropas devem ser inteiras.\n\n");

    // Laço de entrada: preenche cada posição do vetor com dados informados
    for (int i = 0; i < QTDE_TERRITORIOS; ++i) {
        printf("--- Território %d/%d ---\n", i + 1, QTDE_TERRITORIOS);

        // Lendo o nome com scanf usando um "scanset" para aceitar espaços até '\n'
        // Formato: " %29[^\n]" -> lê até 29 chars ou até nova linha; o espaço inicial consome quebras/espacos pendentes
        printf("Nome: ");
        scanf(" %29[^\n]", territorios[i].nome);

        // Lendo a cor (uma palavra). Se preferir aceitar espaços, poderia usar novamente o scanset.
        printf("Cor do exército (ex.: Vermelho/Azul): ");
        scanf(" %9s", territorios[i].cor);

        // Lendo tropas (inteiro). Sem validação extra para manter o nível novato simples
        printf("Quantidade de tropas: ");
        scanf(" %d", &territorios[i].tropas);

        printf("\n");
    }

    // Exibição: percorre o vetor e imprime os dados formatados
    printf("\n=== Territórios cadastrados ===\n");
    printf("%-3s | %-30s | %-10s | %-6s\n", "#", "Nome", "Cor", "Tropas");
    printf("-----------------------------------------------\n");
    for (int i = 0; i < QTDE_TERRITORIOS; ++i) {
        printf("%-3d | %-30s | %-10s | %-6d\n", i, territorios[i].nome, territorios[i].cor, territorios[i].tropas);
    }

    printf("\nCadastro concluído.\n");
    return 0;
}
