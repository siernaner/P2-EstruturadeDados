#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Estrutura que representa um diretório (pasta)
typedef struct Diretorio {
    char nome[50];                  // Nome da pasta
    struct Diretorio *subdiretorio; // Primeira subpasta (filho)
    struct Diretorio *irmao;        // Próxima pasta no mesmo nível (irmão)
    struct Diretorio *pai;          // Pasta superior (pai)
} Diretorio;

/*
 * Cria um novo diretório com o nome informado.
 * Retorna o ponteiro para o novo nó alocado.
 */
Diretorio* criarDiretorio(char nome[]) {
    Diretorio *novo = (Diretorio*) malloc(sizeof(Diretorio));
    if (novo == NULL) {
        printf("Erro: memória insuficiente.\n");
        exit(1);
    }
    strcpy(novo->nome, nome);
    novo->subdiretorio = NULL;
    novo->irmao = NULL;
    novo->pai = NULL;
    return novo;
}

/*
 * Adiciona uma subpasta ao diretório pai.
 * Insere no final da lista de irmãos.
 * Retorna o ponteiro para a nova subpasta criada.
 */
Diretorio* adicionarSubdiretorio(Diretorio *pai, char nome[]) {
    if (pai == NULL) return NULL;

    Diretorio *novo = criarDiretorio(nome);
    novo->pai = pai; // Define o pai para permitir navegação "acima"

    if (pai->subdiretorio == NULL) {
        pai->subdiretorio = novo;
    } else {
        Diretorio *atual = pai->subdiretorio;
        while (atual->irmao != NULL) {
            atual = atual->irmao;
        }
        atual->irmao = novo;
    }
    return novo;
}

/*
 * Exibe toda a árvore de diretórios a partir de 'dir', usando
 * indentação de acordo com o nível.
 */
void listarDiretorios(Diretorio *dir, int nivel) {
    if (dir == NULL) return;

    // Indentação
    for (int i = 0; i < nivel; i++) {
        printf("  ");
    }
    printf("- %s\n", dir->nome);

    // Percorre subdiretórios (filhos)
    Diretorio *filho = dir->subdiretorio;
    while (filho != NULL) {
        listarDiretorios(filho, nivel + 1);
        filho = filho->irmao;
    }
}

/*
 * Lista apenas os nomes das subpastas imediatas do diretório atual.
 * Utilizada pelo comando 'lpa'.
 */
void listarSubdiretorios(Diretorio *dir) {
    if (dir == NULL) {
        printf("Diretório inválido.\n");
        return;
    }
    if (dir->subdiretorio == NULL) {
        printf("(vazio)\n");
        return;
    }
    Diretorio *filho = dir->subdiretorio;
    while (filho != NULL) {
        printf("%s\n", filho->nome);
        filho = filho->irmao;
    }
}

/*
 * Renomeia uma subpasta do diretório atual.
 * Procura pelo nomeAntigo na lista de filhos e, se encontrado,
 * altera para nomeNovo.
 */
void renomearSubdiretorio(Diretorio *atual, char nomeAntigo[], char nomeNovo[]) {
    if (atual == NULL) return;
    Diretorio *filho = atual->subdiretorio;
    while (filho != NULL) {
        if (strcmp(filho->nome, nomeAntigo) == 0) {
            strcpy(filho->nome, nomeNovo);
            printf("Pasta renomeada com sucesso.\n");
            return;
        }
        filho = filho->irmao;
    }
    printf("Erro: pasta '%s' não encontrada no diretório atual.\n", nomeAntigo);
}

/*
 * Função para navegação "acima" (pai) ou "abaixo" (subpasta).
 * Se 'acima' for 1, retorna o pai (ou NULL se estiver na raiz).
 * Caso contrário, procura a subpasta com o nome dado e retorna seu ponteiro.
 * Retorna NULL se a operação não for possível (erro ou raiz).
 */
Diretorio* mudarDiretorio(Diretorio *atual, int acima, char nomeSub[]) {
    if (acima) {
        if (atual->pai == NULL) {
            printf("Já está na raiz do disco Z://.\n");
            return NULL;
        }
        printf("Subindo para %s...\n", atual->pai->nome);
        return atual->pai;
    } else {
        if (nomeSub == NULL || strlen(nomeSub) == 0) {
            printf("Nome da subpasta não pode ser vazio.\n");
            return NULL;
        }
        Diretorio *filho = atual->subdiretorio;
        while (filho != NULL) {
            if (strcmp(filho->nome, nomeSub) == 0) {
                printf("Descendo para %s...\n", filho->nome);
                return filho;
            }
            filho = filho->irmao;
        }
        printf("Erro: subpasta '%s' não encontrada.\n", nomeSub);
        return NULL;
    }
}

int main() {
    // Mensagem de boas-vindas
    printf("Seja bem vindo ao Sistema Operacional Janelas 11\n\n");

    // Criação da pasta raiz
    Diretorio *raiz = criarDiretorio("Z://");
    Diretorio *atual = raiz; // Diretório corrente começa na raiz

    char comando[10];
    char nome[50], nomeAntigo[50], nomeNovo[50];
    char opcaoMudanca; // 's' para subir, 'd' para descer

    // Loop do menu interativo
    do {
        printf("\n--- Menu ---\n");
        printf("cpa - Criar pasta\n");
        printf("rpa - Renomear pasta\n");
        printf("lpa - Listar pastas\n");
        printf("mpa - Mudar de pasta\n");
        printf("exit - Sair\n");
        printf("Diretório atual: %s\n", atual->nome);
        printf("> ");
        scanf("%s", comando);

        if (strcmp(comando, "cpa") == 0) {
            printf("Nome da nova pasta: ");
            scanf("%s", nome);
            adicionarSubdiretorio(atual, nome);
            printf("Pasta '%s' criada em '%s'.\n", nome, atual->nome);

        } else if (strcmp(comando, "rpa") == 0) {
            printf("Nome atual da pasta: ");
            scanf("%s", nomeAntigo);
            printf("Novo nome: ");
            scanf("%s", nomeNovo);
            renomearSubdiretorio(atual, nomeAntigo, nomeNovo);

        } else if (strcmp(comando, "lpa") == 0) {
            printf("Subpastas de '%s':\n", atual->nome);
            listarSubdiretorios(atual);

        } else if (strcmp(comando, "mpa") == 0) {
            // Pergunta se deseja subir (acima) ou descer (abaixo)
            printf("Deseja subir (s) ou descer (d)? ");
            scanf(" %c", &opcaoMudanca); // espaço antes de %c para consumir newline

            if (opcaoMudanca == 's' || opcaoMudanca == 'S') {
                Diretorio *novoAtual = mudarDiretorio(atual, 1, NULL);
                if (novoAtual != NULL) {
                    atual = novoAtual;
                }
            } else if (opcaoMudanca == 'd' || opcaoMudanca == 'D') {
                printf("Nome da subpasta: ");
                scanf("%s", nome);
                Diretorio *novoAtual = mudarDiretorio(atual, 0, nome);
                if (novoAtual != NULL) {
                    atual = novoAtual;
                }
            } else {
                printf("Opção inválida. Use 's' para subir ou 'd' para descer.\n");
            }

        } else if (strcmp(comando, "exit") == 0) {
            break; // sai do loop

        } else {
            printf("Comando inválido. Tente novamente.\n");
        }

    } while (1);
    
    // Exibe a estrutura completa da árvore
    printf("\nEstrutura final da árvore de diretórios:\n");
    listarDiretorios(raiz, 0);

    printf("\nDesligando...\n");

    // Liberação de memória (opcional)
    // Como não é obrigatório, optamos por não implementar a liberação completa
    // para manter o código mais simples.

    return 0;
} 