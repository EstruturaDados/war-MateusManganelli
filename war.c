#include <stdio.h>
#include <stdlib.h> // Para calloc, free, rand, srand
#include <string.h> // Para strcspn, strcpy
#include <time.h>   // Para srand(time(NULL))

// mateusConceito: A struct permanece a mesma
struct Territorio {
    char nome[50];
    char cor[30];
    int tropas;
};

// -----------------------------------------------------------------
// FUNÇÃO AUXILIAR: Limpar o "buffer" do teclado
// -----------------------------------------------------------------
void limpar_buffer_teclado() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// -----------------------------------------------------------------
// FUNÇÃO NOVA: Exibir o estado atual do mapa
// -----------------------------------------------------------------
// (Modularizando o que estava no main do Nível Novato)
// Conceito: Ponteiros
// Recebemos 'mapa' como um ponteiro para a struct.
// -----------------------------------------------------------------
void exibir_mapa(struct Territorio *mapa, int num_territorios) {
    printf("\n\n===========================================================\n");
    printf("              🗺️  ESTADO ATUAL DO MAPA 🗺️\n");
    printf("===========================================================\n");
    printf("%-5s | %-20s | %-15s | %s\n", "ID", "Território", "Cor do Exército", "Tropas");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < num_territorios; i++) {
        // Usamos (i+1) para exibir um ID amigável (1-5)
        printf("(%d)   | %-20s | %-15s | %d\n",
               i + 1,
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
    printf("-----------------------------------------------------------\n");
}

// -----------------------------------------------------------------
// FUNÇÃO NOVA: Simulação de Batalha
// -----------------------------------------------------------------
//  Conceito: Números Aleatórios (rand)
// -----------------------------------------------------------------
void batalhar(struct Territorio *mapa, int id_atacante, int id_defensor) {
    // Conceito: rand()
    // (rand() % 6) gera um número de 0 a 5.
    // Somamos 1 para simular um dado (1 a 6).
    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;

    struct Territorio *atacante = &mapa[id_atacante];
    struct Territorio *defensor = &mapa[id_defensor];

    printf("\n⚔️  BATALHA: %s (%s) ataca %s (%s)!\n",
           atacante->nome, atacante->cor,
           defensor->nome, defensor->cor);
    
    printf("   Dado de Ataque: %d\n", dado_ataque);
    printf("   Dado de Defesa: %d\n", dado_defesa);

    // Lógica da Batalha (Empate favorece o atacante, conforme solicitado)
    if (dado_ataque >= dado_defesa) {
        printf("   VITÓRIA DO ATAQUE! O defensor (%s) perde 1 tropa.\n", defensor->nome);
        defensor->tropas--;

        // Verificar conquista
        if (defensor->tropas == 0) {
            printf("   🚩 CONQUISTA! %s foi dominado pelo exército %s!\n",
                   defensor->nome, atacante->cor);
            
            // 1. Território conquistado passa a ter a cor do atacante
            strcpy(defensor->cor, atacante->cor);
            
            // 2. O atacante move 1 tropa para ocupar o território
            defensor->tropas = 1;
            
            // 3. O território de origem perde essa 1 tropa
            atacante->tropas--;
        }
    } else {
        printf("   VITÓRIA DA DEFESA! O ataque falhou.\n");
        // (De acordo com a regra, o atacante não perde tropas ao falhar)
    }
}

// -----------------------------------------------------------------
// FUNÇÃO PRINCIPAL
// -----------------------------------------------------------------
int main() {
    const int NUM_TERRITORIOS = 5;

    // Conceito: Ponteiros
    // 'mapa' agora é um ponteiro, que apontará para o
    // bloco de memória que vamos alocar.
    struct Territorio *mapa;

    // Conceito: calloc (Alocação Dinâmica)
    // calloc(N, TAMANHO)
    // Aloca memória para 'N' elementos do tamanho 'TAMANHO'
    // e inicializa todos os bytes com 0.
    mapa = (struct Territorio *)calloc(NUM_TERRITORIOS, sizeof(struct Territorio));

    // Boa prática: sempre verificar se a alocação funcionou
    if (mapa == NULL) {
        printf("Erro crítico: Falha ao alocar memória!\n");
        return 1; // Encerra o programa com código de erro
    }
    
    // Conceito: srand()
    // Inicializa a semente do gerador de números aleatórios.
    // Usamos time(NULL) para garantir que a semente seja
    // diferente a cada execução do programa.
    srand(time(NULL));


    // --- 1. CADASTRO INICIAL (Similar ao Nível Novato) ---
    printf("--- 🗺️ Cadastro Inicial de Territórios (WAR) ---\n");

    for (int i = 0; i < NUM_TERRITORIOS; i++) {
        printf("\n--- Território %d ---\n", i + 1);

        printf("Digite o nome: ");
        fgets(mapa[i].nome, 50, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Digite a cor do exército: ");
        fgets(mapa[i].cor, 30, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Digite o número de tropas: ");
        scanf("%d", &mapa[i].tropas);
        limpar_buffer_teclado();
    }


    // --- 2. FASE DE ATAQUE (Laço Interativo) ---
    char opcao_ataque;
    int id_atacante, id_defensor;

    while (1) { // Loop infinito de batalha
        exibir_mapa(mapa, NUM_TERRITORIOS);

        printf("\nDeseja realizar um ataque? (s/n): ");
        
        // " %c" -> O espaço antes do %c é crucial.
        // Ele consome qualquer 'ENTER' ('\n') deixado
        // pelo scanf anterior, evitando a necessidade
        // de chamar limpar_buffer_teclado() aqui.
        scanf(" %c", &opcao_ataque);

        if (opcao_ataque == 'n' || opcao_ataque == 'N') {
            printf("Encerrando fase de ataques...\n");
            break; // Sai do loop 'while(1)'
        }
        
        // --- Escolha do Atacante ---
        printf("Digite o ID (1-5) do território ATACANTE: ");
        scanf("%d", &id_atacante);

        // --- Escolha do Defensor ---
        printf("Digite o ID (1-5) do território DEFENSOR: ");
        scanf("%d", &id_defensor);
        limpar_buffer_teclado(); // Limpa o buffer após o último scanf

        // --- Validações ---
        
        // Ajuste de índice (Usuário digita 1-5, Vetor é 0-4)
        id_atacante--;
        id_defensor--;

        if (id_atacante < 0 || id_atacante >= NUM_TERRITORIOS ||
            id_defensor < 0 || id_defensor >= NUM_TERRITORIOS) {
            printf("\nERRO: IDs inválidos! Tente novamente.\n");
            continue; // Pula para a próxima iteração do loop
        }

        if (id_atacante == id_defensor) {
            printf("\nERRO: Um território não pode atacar a si mesmo!\n");
            continue;
        }

        // Validação de tropas: precisa ter ao menos 2 tropas para atacar
        // (1 para atacar, 1 que fica no território)
        if (mapa[id_atacante].tropas < 2) {
            printf("\nERRO: O território atacante (%s) precisa de \n"
                   "      pelo menos 2 tropas para iniciar um ataque!\n", 
                   mapa[id_atacante].nome);
            continue;
        }

        // Validação de dono (Não pode atacar um território que já é seu)
        if (strcmp(mapa[id_atacante].cor, mapa[id_defensor].cor) == 0) {
            printf("\nERRO: Você não pode atacar um território que já é seu!\n");
            continue;
        }

        // Se passou em todas as validações, inicia a batalha
        batalhar(mapa, id_atacante, id_defensor);
        
        printf("\nPressione ENTER para continuar...\n");
        getchar(); // Pausa a tela
    }


    // --- 3. LIMPEZA DA MEMÓRIA ---
    printf("\nJogo encerrado. Liberando memória...\n");
    
    // Conceito: free
    // Libera o bloco de memória que foi alocado com calloc.
    // Essencial para evitar "vazamento de memória" (memory leak).
    free(mapa);
    mapa = NULL; // Boa prática: anular o ponteiro após o free.

    return 0;
}