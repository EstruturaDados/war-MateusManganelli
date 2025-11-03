#include <stdio.h>
#include <stdlib.h>     // Para calloc, free, rand, srand
#include <string.h>     // Para strcpy, strcmp, sprintf
#include <time.h>       // Para srand(time(NULL))
#include <stdbool.h>    // Para usar o tipo 'bool' (true/false)

// -----------------------------------------------------------------
// 💡 CONCEITO: Constantes Globais (Boas Práticas)
// Usar #define (ou const) para valores mágicos torna o código
// mais fácil de manter.
// -----------------------------------------------------------------
#define NUM_TERRITORIOS 5
#define COR_JOGADOR "Azul" // O jogador sempre será o exército Azul

// -----------------------------------------------------------------
// 💡 CONCEITO: Estruturas de Dados
// -----------------------------------------------------------------

// A struct de Território, como nos níveis anteriores
struct Territorio {
    char nome[50];
    char cor[30];
    int tropas;
};

// 💡 CONCEITO: Enum
// Um 'enum' torna o código mais legível. Em vez de usar 'tipo 1'
// ou 'tipo 2' para a missão, usamos nomes descritivos.
typedef enum {
    DESTRUIR_COR,
    CONQUISTAR_N_TERRITORIOS
} TipoMissao;

// Nova struct para armazenar os detalhes da missão
struct Missao {
    TipoMissao tipo;
    char descricao[150];
    
    // Alvos da missão
    char cor_alvo[30];
    int num_territorios_alvo;
};

// 💡 CONCEITO: Modularização (Estado do Jogo)
// Em vez de passar 3 ou 4 argumentos para cada função,
// agrupamos todos os dados principais do jogo (mapa e missão)
// em uma única struct 'EstadoJogo'.
struct EstadoJogo {
    struct Territorio *mapa; // O mapa agora é um ponteiro
    struct Missao missao_jogador;
};


// -----------------------------------------------------------------
// PROTÓTIPOS DE FUNÇÕES (Declarações)
// -----------------------------------------------------------------
// (Informar ao C quais funções existem antes de usá-las)

// --- Funções Utilitárias ---
void limpar_buffer_teclado();
void pausar_tela();
int contar_territorios_por_cor(const struct Territorio *mapa, int num_territorios, const char *cor);

// --- Funções de Inicialização e Limpeza ---
void inicializar_mapa_automatico(struct Territorio *mapa);
void atribuir_missao_aleatoria(struct Missao *missao);
void inicializar_jogo(struct EstadoJogo *jogo);
void limpar_jogo(struct EstadoJogo *jogo);

// --- Funções de Exibição ---
void exibir_mapa(const struct Territorio *mapa, int num_territorios);
void exibir_menu();

// --- Funções de Lógica do Jogo ---
void batalhar(struct Territorio *mapa, int id_atacante, int id_defensor);
void processar_fase_ataque(struct EstadoJogo *jogo);
bool verificar_missao(struct EstadoJogo *jogo, bool exibir_status);


// -----------------------------------------------------------------
// FUNÇÃO PRINCIPAL (main)
// -----------------------------------------------------------------
// 💡 CONCEITO: Modularização
// A 'main' agora é apenas um "controlador". Ela chama
// outras funções para fazer o trabalho pesado e gerencia
// o menu principal.
// -----------------------------------------------------------------
int main() {
    // Inicializa a semente para números aleatórios
    srand(time(NULL));

    struct EstadoJogo jogo;
    inicializar_jogo(&jogo);

    bool jogo_ativo = true;
    int opcao;

    while (jogo_ativo) {
        exibir_mapa(jogo.mapa, NUM_TERRITORIOS);
        exibir_menu();
        
        if (scanf("%d", &opcao) != 1) {
            opcao = -1; // Valor inválido para forçar o 'default'
        }
        limpar_buffer_teclado();

        switch (opcao) {
            case 1:
                processar_fase_ataque(&jogo);
                
                // 💡 CONCEITO: Verificação de Vitória
                // Após cada ataque, verificamos silenciosamente (sem exibir status)
                // se a missão foi cumprida.
                if (verificar_missao(&jogo, false)) { 
                    printf("\n\n****************************************\n");
                    printf("    🎉 V-I-T-Ó-R-I-A! 🎉\n");
                    printf("   Você completou sua missão:\n");
                    printf("   %s\n", jogo.missao_jogador.descricao);
                    printf("****************************************\n\n");
                    jogo_ativo = false; // Termina o loop do jogo
                }
                break;

            case 2:
                // Apenas exibe o status da missão
                verificar_missao(&jogo, true);
                pausar_tela();
                break;

            case 0:
                printf("Obrigado por jogar! Saindo...\n");
                jogo_ativo = false;
                break;

            default:
                printf("Opção inválida. Tente novamente.\n");
                pausar_tela();
                break;
        }
    }

    // Libera a memória alocada antes de sair
    limpar_jogo(&jogo);
    return 0;
}


// -----------------------------------------------------------------
// IMPLEMENTAÇÃO DAS FUNÇÕES
// -----------------------------------------------------------------

// --- Funções Utilitárias ---

void limpar_buffer_teclado() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void pausar_tela() {
    printf("\nPressione ENTER para continuar...\n");
    getchar();
}

// 💡 CONCEITO: const correctness
// Usamos 'const' para dizer que esta função PROMETE
// não alterar o 'mapa' ou a 'cor'. Ela é "read-only".
int contar_territorios_por_cor(const struct Territorio *mapa, int num_territorios, const char *cor) {
    int contador = 0;
    for (int i = 0; i < num_territorios; i++) {
        if (strcmp(mapa[i].cor, cor) == 0) {
            contador++;
        }
    }
    return contador;
}


// --- Funções de Inicialização e Limpeza ---

// Apenas preenche o mapa com dados iniciais
void inicializar_mapa_automatico(struct Territorio *mapa) {
    // Território 0 (Jogador)
    strcpy(mapa[0].nome, "Brasil");
    strcpy(mapa[0].cor, COR_JOGADOR);
    mapa[0].tropas = 10;

    // Território 1 (Inimigo)
    strcpy(mapa[1].nome, "Argentina");
    strcpy(mapa[1].cor, "Verde");
    mapa[1].tropas = 5;

    // Território 2 (Inimigo)
    strcpy(mapa[2].nome, "Peru");
    strcpy(mapa[2].cor, "Verde");
    mapa[2].tropas = 3;

    // Território 3 (Neutro)
    strcpy(mapa[3].nome, "Suecia");
    strcpy(mapa[3].cor, "Amarelo");
    mapa[3].tropas = 8;

    // Território 4 (Neutro)
    strcpy(mapa[4].nome, "Egito");
    strcpy(mapa[4].cor, "Preto");
    mapa[4].tropas = 6;
}

// Sorteia e configura uma das duas missões
void atribuir_missao_aleatoria(struct Missao *missao) {
    int tipo_missao = rand() % 2; // Sorteia 0 ou 1

    if (tipo_missao == 0) {
        // Missão 0: Destruir o exército Verde
        missao->tipo = DESTRUIR_COR;
        strcpy(missao->cor_alvo, "Verde");
        missao->num_territorios_alvo = 0; // Não usado aqui
        // sprintf formata a string e a salva na variável
        sprintf(missao->descricao, "Destruir todos os territórios do exército %s.", missao->cor_alvo);
    } else {
        // Missão 1: Conquistar 3 territórios
        missao->tipo = CONQUISTAR_N_TERRITORIOS;
        missao->num_territorios_alvo = 3;
        strcpy(missao->cor_alvo, ""); // Não usado aqui
        sprintf(missao->descricao, "Conquistar um total de %d territórios.", missao->num_territorios_alvo);
    }
}

// Função "mestre" de inicialização
// 💡 CONCEITO: Passagem por Referência
// Passamos 'struct EstadoJogo *jogo' (um ponteiro) para
// que a função possa MODIFICAR a struct original que está na main.
void inicializar_jogo(struct EstadoJogo *jogo) {
    // 1. Aloca memória para o mapa
    jogo->mapa = (struct Territorio *)calloc(NUM_TERRITORIOS, sizeof(struct Territorio));
    if (jogo->mapa == NULL) {
        printf("Erro crítico: Falha ao alocar memória!\n");
        exit(1); // Encerra o programa
    }

    // 2. Preenche o mapa
    inicializar_mapa_automatico(jogo->mapa);

    // 3. Atribui a missão
    atribuir_missao_aleatoria(&jogo->missao_jogador);

    // 4. Mensagem de boas-vindas
    printf("--- 🗺️  Bem-vindo ao WAR (Nível Mestre) 🗺️  ---\n");
    printf("Você controla o exército %s.\n", COR_JOGADOR);
    printf("Sua missão: %s\n", jogo->missao_jogador.descricao);
    pausar_tela();
}

// Libera a memória que foi alocada com calloc
void limpar_jogo(struct EstadoJogo *jogo) {
    free(jogo->mapa);
    jogo->mapa = NULL;
}


// --- Funções de Exibição ---

void exibir_mapa(const struct Territorio *mapa, int num_territorios) {
    // system("clear || cls"); // (Opcional) Limpar a tela
    printf("\n\n===========================================================\n");
    printf("              🗺️  ESTADO ATUAL DO MAPA 🗺️\n");
    printf("===========================================================\n");
    printf("%-5s | %-20s | %-15s | %s\n", "ID", "Território", "Cor do Exército", "Tropas");
    printf("-----------------------------------------------------------\n");

    for (int i = 0; i < num_territorios; i++) {
        printf("(%d)   | %-20s | %-15s | %d\n",
               i + 1, // ID amigável (1-5)
               mapa[i].nome,
               mapa[i].cor,
               mapa[i].tropas);
    }
    printf("-----------------------------------------------------------\n");
}

void exibir_menu() {
    printf("\n--- FASE DE ATAQUE ---\n");
    printf("Escolha sua ação:\n");
    printf(" 1 - Atacar\n");
    printf(" 2 - Verificar Missão\n");
    printf(" 0 - Sair do Jogo\n");
    printf("Sua escolha: ");
}


// --- Funções de Lógica do Jogo ---

// Lógica de batalha (igual ao Nível Aventureiro, mas modularizada)
void batalhar(struct Territorio *mapa, int id_atacante, int id_defensor) {
    // Usamos ponteiros para facilitar o acesso
    struct Territorio *atacante = &mapa[id_atacante];
    struct Territorio *defensor = &mapa[id_defensor];

    int dado_ataque = (rand() % 6) + 1;
    int dado_defesa = (rand() % 6) + 1;

    printf("\n⚔️  BATALHA: %s (%s) ataca %s (%s)!\n",
           atacante->nome, atacante->cor,
           defensor->nome, defensor->cor);
    
    printf("   Dado de Ataque: %d\n", dado_ataque);
    printf("   Dado de Defesa: %d\n", dado_defesa);

    // Empates favorecem o atacante
    if (dado_ataque >= dado_defesa) {
        printf("   VITÓRIA DO ATAQUE! O defensor (%s) perde 1 tropa.\n", defensor->nome);
        defensor->tropas--;

        if (defensor->tropas == 0) {
            printf("   🚩 CONQUISTA! %s foi dominado pelo exército %s!\n",
                   defensor->nome, atacante->cor);
            
            strcpy(defensor->cor, atacante->cor); // Mude a cor
            defensor->tropas = 1;                 // Ocupa com 1 tropa
            atacante->tropas--;                   // Move a tropa do território original
        }
    } else {
        printf("   VITÓRIA DA DEFESA! O ataque falhou.\n");
    }
}

// Gerencia a lógica de escolha e validação do ataque
void processar_fase_ataque(struct EstadoJogo *jogo) {
    int id_atacante, id_defensor;
    
    printf("Você é o exército %s.\n", COR_JOGADOR);
    printf("Digite o ID (1-5) do território ATACANTE (deve ser seu): ");
    scanf("%d", &id_atacante);
    printf("Digite o ID (1-5) do território DEFENSOR (deve ser inimigo): ");
    scanf("%d", &id_defensor);
    limpar_buffer_teclado();

    // --- Validações ---
    
    // Ajuste de índice (Usuário digita 1-5, Vetor é 0-4)
    id_atacante--;
    id_defensor--;

    if (id_atacante < 0 || id_atacante >= NUM_TERRITORIOS ||
        id_defensor < 0 || id_defensor >= NUM_TERRITORIOS) {
        printf("\nERRO: IDs inválidos! Tente novamente.\n");
        pausar_tela();
        return; // Retorna ao menu
    }

    if (id_atacante == id_defensor) {
        printf("\nERRO: Um território não pode atacar a si mesmo!\n");
        pausar_tela();
        return;
    }

    // 💡 CONCEITO: Validação de Lógica
    // O jogador SÓ pode atacar COM um território SEU.
    if (strcmp(jogo->mapa[id_atacante].cor, COR_JOGADOR) != 0) {
        printf("\nERRO: Você só pode atacar com territórios do exército %s!\n", COR_JOGADOR);
        pausar_tela();
        return;
    }

    // O jogador NÃO pode atacar um território que JÁ É SEU.
    if (strcmp(jogo->mapa[id_defensor].cor, COR_JOGADOR) == 0) {
        printf("\nERRO: Você não pode atacar um território que já é seu!\n");
        pausar_tela();
        return;
    }

    // Regra do WAR: precisa ter > 1 tropa para atacar
    if (jogo->mapa[id_atacante].tropas < 2) {
        printf("\nERRO: O território atacante (%s) precisa de \n"
               "      pelo menos 2 tropas para iniciar um ataque!\n", 
               jogo->mapa[id_atacante].nome);
        pausar_tela();
        return;
    }

    // Se passou em tudo, execute a batalha
    batalhar(jogo->mapa, id_atacante, id_defensor);
    pausar_tela();
}


// 💡 CONCEITO: Função de Verificação (Core da Missão)
// Esta função faz duas coisas:
// 1. (Se exibir_status == true) Mostra o progresso da missão.
// 2. (Sempre) Retorna 'true' se a missão foi concluída, 'false' caso contrário.
bool verificar_missao(struct EstadoJogo *jogo, bool exibir_status) {
    struct Missao *missao = &jogo->missao_jogador;
    const struct Territorio *mapa = jogo->mapa; // 'const' pois não vamos alterar o mapa aqui
    bool missao_completa = false;

    if (exibir_status) {
        printf("\n--- 🎯 VERIFICAR MISSÃO 🎯 ---\n");
        printf("Sua missão: %s\n", missao->descricao);
    }

    switch (missao->tipo) {
        case DESTRUIR_COR:
            // Contamos quantos territórios o inimigo ainda tem
            int territorios_restantes = contar_territorios_por_cor(mapa, NUM_TERRITORIOS, missao->cor_alvo);
            
            if (exibir_status) {
                printf("   Status: O exército %s ainda possui %d territórios.\n",
                       missao->cor_alvo, territorios_restantes);
            }
            
            if (territorios_restantes == 0) {
                missao_completa = true;
            }
            break;

        case CONQUISTAR_N_TERRITORIOS:
            // Contamos quantos territórios o JOGADOR tem
            int territorios_conquistados = contar_territorios_por_cor(mapa, NUM_TERRITORIOS, COR_JOGADOR);
            
            if (exibir_status) {
                printf("   Status: Você possui %d de %d territórios necessários.\n",
                       territorios_conquistados, missao->num_territorios_alvo);
            }
            
            if (territorios_conquistados >= missao->num_territorios_alvo) {
                missao_completa = true;
            }
            break;
    }

    return missao_completa;
}