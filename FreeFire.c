#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

// --- Definições e Constantes ---
#define MAX_ITENS 10
#define TAM_NOME 30
#define TAM_TIPO 20

// Enum para facilitar a escolha da ordenação
typedef enum {
    ORDENAR_NOME = 1,
    ORDENAR_TIPO = 2,
    ORDENAR_PRIORIDADE = 3
} CriterioOrdenacao;

// Estrutura do Item (Loot)
typedef struct {
    char nome[TAM_NOME];
    char tipo[TAM_TIPO]; // Ex: Arma, Municao, Cura
    int quantidade;
    int prioridade;      // 1 (Baixa) a 5 (Alta)
} Item;

// Variáveis Globais (Mochila)
Item mochila[MAX_ITENS];
int totalItens = 0;
bool estaOrdenadoPorNome = false; // Flag para validar Busca Binária

// --- Protótipos das Funções ---
void adicionarItem();
void listarItens();
void removerItem();
void ordenarItens(CriterioOrdenacao criterio);
void buscarItemBinaria();
void limparBuffer();
void lerString(char* buffer, int tamanho);

// --- Função Principal ---
int main() {
    int opcao;

    do {
        printf("\n=== 🎒 MOCHILA FREE FIRE (NIVEL MESTRE) ===\n");
        printf("1. Adicionar Item (Loot)\n");
        printf("2. Remover Item\n");
        printf("3. Listar Inventario\n");
        printf("4. Ordenar Mochila (Insertion Sort)\n");
        printf("5. Buscar Item (Busca Binaria)\n");
        printf("0. Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch(opcao) {
            case 1: adicionarItem(); break;
            case 2: removerItem(); break;
            case 3: listarItens(); break;
            case 4: 
                printf("\nCritério: [1] Nome | [2] Tipo | [3] Prioridade: ");
                int crit;
                scanf("%d", &crit);
                ordenarItens((CriterioOrdenacao)crit);
                break;
            case 5: buscarItemBinaria(); break;
            case 0: printf("Saindo da ilha...\n"); break;
            default: printf("Opcao invalida!\n");
        }
    } while (opcao != 0);

    return 0;
}

// --- Implementação das Funções ---

void adicionarItem() {
    if (totalItens >= MAX_ITENS) {
        printf("❌ Mochila cheia! Descarte algo antes.\n");
        return;
    }

    Item novo;
    printf("\n--- Novo Loot ---\n");
    printf("Nome: ");
    lerString(novo.nome, TAM_NOME);
    
    printf("Tipo (Arma/Municao/Cura): ");
    lerString(novo.tipo, TAM_TIPO);
    
    printf("Quantidade: ");
    scanf("%d", &novo.quantidade);
    
    do {
        printf("Prioridade (1-5): ");
        scanf("%d", &novo.prioridade);
    } while (novo.prioridade < 1 || novo.prioridade > 5);

    mochila[totalItens] = novo;
    totalItens++;
    estaOrdenadoPorNome = false; // Ao adicionar, a lista pode perder a ordem
    printf("✅ Item adicionado com sucesso!\n");
}

void listarItens() {
    if (totalItens == 0) {
        printf("\n📭 Mochila vazia.\n");
        return;
    }

    printf("\n%-4s | %-20s | %-15s | %-5s | %-5s\n", "ID", "Nome", "Tipo", "Qtd", "Prior");
    printf("----------------------------------------------------------------\n");
    for (int i = 0; i < totalItens; i++) {
        printf("#%-3d | %-20s | %-15s | %-5d | %-5d\n", 
               i, mochila[i].nome, mochila[i].tipo, mochila[i].quantidade, mochila[i].prioridade);
    }
}

void removerItem() {
    char alvo[TAM_NOME];
    printf("Nome do item a remover: ");
    lerString(alvo, TAM_NOME);

    int indice = -1;
    // Busca Sequencial simples para encontrar o índice
    for (int i = 0; i < totalItens; i++) {
        if (strcasecmp(mochila[i].nome, alvo) == 0) { // strcasecmp ignora maiusculas/minusculas (POSIX)
            indice = i;
            break;
        }
    }

    if (indice == -1) {
        printf("❌ Item nao encontrado.\n");
        return;
    }

    // Remove deslocando os itens à direita para a esquerda
    for (int i = indice; i < totalItens - 1; i++) {
        mochila[i] = mochila[i + 1];
    }
    totalItens--;
    printf("🗑️ Item removido.\n");
}

// ALGORITMO: Insertion Sort (Ordenação por Inserção)
void ordenarItens(CriterioOrdenacao criterio) {
    if (totalItens < 2) {
        printf("Poucos itens para ordenar.\n");
        return;
    }

    int i, j;
    Item chave;
    int comparacoes = 0;

    for (i = 1; i < totalItens; i++) {
        chave = mochila[i];
        j = i - 1;

        // Lógica de comparação baseada no critério escolhido
        bool precisaMover = false;
        
        while (j >= 0) {
            comparacoes++; // Contabiliza performance
            
            if (criterio == ORDENAR_NOME) {
                // strcmp > 0 significa que mochila[j] vem DEPOIS da chave alfabeticamente
                if (strcasecmp(mochila[j].nome, chave.nome) > 0) precisaMover = true;
                else break;
            } 
            else if (criterio == ORDENAR_TIPO) {
                if (strcasecmp(mochila[j].tipo, chave.tipo) > 0) precisaMover = true;
                else break;
            } 
            else if (criterio == ORDENAR_PRIORIDADE) {
                // Ordenação Decrescente de prioridade (5 vem antes de 1)
                if (mochila[j].prioridade < chave.prioridade) precisaMover = true;
                else break;
            }

            if (precisaMover) {
                mochila[j + 1] = mochila[j];
                j = j - 1;
            } else {
                break;
            }
        }
        mochila[j + 1] = chave;
    }

    printf("🔄 Mochila ordenada! (Comparacoes realizadas: %d)\n", comparacoes);
    
    // Atualiza flag apenas se ordenou por nome
    if (criterio == ORDENAR_NOME) estaOrdenadoPorNome = true;
    else estaOrdenadoPorNome = false;
    
    listarItens();
}

// ALGORITMO: Binary Search (Busca Binária)
void buscarItemBinaria() {
    if (!estaOrdenadoPorNome) {
        printf("⚠️ ERRO: A mochila precisa estar ORDENADA POR NOME para usar a Busca Binaria.\n");
        printf("   Va na opcao 4 e ordene por Nome primeiro.\n");
        return;
    }

    char alvo[TAM_NOME];
    printf("Digite o nome do item para buscar: ");
    lerString(alvo, TAM_NOME);

    int inicio = 0;
    int fim = totalItens - 1;
    int meio;
    int passos = 0;
    bool encontrado = false;

    while (inicio <= fim) {
        passos++;
        meio = (inicio + fim) / 2;

        int comparacao = strcasecmp(alvo, mochila[meio].nome);

        if (comparacao == 0) {
            encontrado = true;
            printf("\n🔍 ITEM ENCONTRADO (em %d passos)!\n", passos);
            printf("Nome: %s | Tipo: %s | Qtd: %d | Prior: %d\n", 
                   mochila[meio].nome, mochila[meio].tipo, mochila[meio].quantidade, mochila[meio].prioridade);
            break;
        }
        else if (comparacao > 0) {
            // O alvo é "maior" (vem depois no alfabeto), descarta a metade esquerda
            inicio = meio + 1;
        }
        else {
            // O alvo é "menor", descarta a metade direita
            fim = meio - 1;
        }
    }

    if (!encontrado) {
        printf("❌ Item nao encontrado na mochila.\n");
    }
}

// --- Funções Auxiliares ---
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void lerString(char* buffer, int tamanho) {
    limparBuffer(); // Garante buffer limpo antes de ler
    fgets(buffer, tamanho, stdin);
    // Remove o \n do final da string, se existir
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
}
