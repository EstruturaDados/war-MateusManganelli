#include <stdio.h>
#include <string.h> // Necessário para a função strcspn

// 💡 Conceito: struct
// Uma struct é um tipo de dado que agrupa diferentes
// variáveis sob um único nome.
struct Territorio {
    char nome[50];
    char cor[30];
    int tropas;
};

// -----------------------------------------------------------------
// FUNÇÃO AUXILIAR: Limpar o "buffer" do teclado
// -----------------------------------------------------------------
// Por que isso é necessário?
// Quando você digita um número com scanf (ex: "5" e aperta ENTER),
// o scanf() lê o "5", mas deixa o "ENTER" (o caractere '\n')
// na fila de entrada.
// A próxima função fgets() veria esse "ENTER" e pensaria que
// você digitou uma string vazia.
// Esta função consome todos os caracteres restantes até o ENTER.
// -----------------------------------------------------------------
void limpar_buffer_teclado() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// -----------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// -----------------------------------------------------------------
int main() {
    // 💡 Conceito: Vetor Estático
    // Criamos um vetor (array) de 5 posições.
    // Cada posição armazena UMA struct Territorio completa.
    const int NUM_TERRITORIOS = 5;
    struct Territorio mapa[NUM_TERRITORIOS];

    printf("--- 🗺️ Cadastro Inicial de Territórios (WAR) ---\n");
    printf("Por favor, insira os dados para os %d territórios.\n", NUM_TERRITORIOS);

    // --- 1. LEITURA DE DADOS (CADASTRO) ---
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Território %d ---\n", i + 1);

        // 💡 Conceito: Leitura com fgets
        // Usamos fgets para ler strings que podem conter espaços.
        
        // Leitura do NOME
        printf("Digite o nome: ");
        fgets(mapa[i].nome, 50, stdin);
        // fgets() armazena o "ENTER" ('\n') no final da string.
        // A linha abaixo remove esse '\n' para a impressão ficar correta.
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        // Leitura da COR
        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, 30, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        // 💡 Conceito: Leitura com scanf
        // Usamos scanf para ler um valor numérico (int).
        printf("Digite o número de tropas: ");
        scanf("%d", &mapa[i].tropas);

        // ❗️ PONTO CRÍTICO ❗️
        // Limpamos o buffer DEPOIS do scanf, para que o "ENTER"
        // que o usuário apertou não atrapalhe o próximo fgets()
        // na próxima volta do loop.
        limpar_buffer_teclado();
    }

    // --- 2. EXIBIÇÃO DO MAPA (SAÍDA) ---
    printf("\n\n=======================================================\n");
    printf("            🗺️  ESTADO ATUAL DO MAPA 🗺️\n");
    printf("=======================================================\n");

    // Imprime um cabeçalho formatado
    // "%-20s" significa: alinhe o texto (string) à ESQUERDA (-) em 20 espaços.
    printf("%-20s | %-15s | %s\n", "Território", "Cor do Exército", "Tropas");
    printf("-------------------------------------------------------\n");

    // Loop para imprimir cada território cadastrado
    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("%-20s | %-15s | %d\n",
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
    printf("-------------------------------------------------------\n");

    return 0;
}