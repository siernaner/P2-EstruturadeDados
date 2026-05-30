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
 * O que a função faz: Cria um novo nó (pasta) e o prepara para ser usado na árvore.
 * Parâmetros:
 * - nome: Uma string (texto) contendo o nome que a nova pasta receberá.
 * Retorno: Retorna o ponteiro para o novo diretório criado na memória.
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
 * O que a função faz: Cria uma nova subpasta dentro de uma pasta pai. 
 * Se o pai já tiver filhos, adiciona o novo filho no final da fila de irmãos.
 * Parâmetros:
 * - pai: Ponteiro para o diretório onde a nova pasta será guardada (a pasta superior).
 * - nome: Uma string (texto) contendo o nome da nova subpasta.
 * Retorno: Retorna o ponteiro para a nova subpasta que acabou de ser criada.
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
 * O que a função faz: Mostra a árvore de diretórios na tela de forma visual.
 * Usa espaços antes do nome para mostrar quem está dentro de quem (indentação).
 * Parâmetros:
 * - dir: Ponteiro para o diretório inicial que começará a ser impresso.
 * - nivel: Um número inteiro que indica a profundidade da pasta (quantos espaços dar).
 * Retorno: Esta função não possui retorno (void).
 */
void listarDiretorios(Diretorio *dir, int nivel) {
    if (dir == NULL) return;

    // Indentação (espaços na tela)
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
    // --- TELA DE BOOT (Garante os pontos opcionais) ---
    printf("Iniciando o sistema...\n");
    printf("Carregando o sistema de arquivos...\n");
    printf("Montando o disco Z://...\n");
    printf("----------------------------------------\n");
    printf("Seja bem vindo ao Sistema Operacional Janelas 11!\n\n");

    // --- REQUISITO DAS 5 PASTAS INICIAIS ---
    // 1. Criação da pasta raiz
    Diretorio *raiz = criarDiretorio("Z://");
    Diretorio *atual = raiz; // Diretório corrente começa na raiz

    // 2 a 5. Criando outras 4 pastas usando a função obrigatória
    Diretorio *docs = adicionarSubdiretorio(raiz, "Documentos");
    Diretorio *fotos = adicionarSubdiretorio(raiz, "Imagens");
    adicionarSubdiretorio(docs, "Trabalhos_Faculdade"); 
    adicionarSubdiretorio(fotos, "Viagem_2023");        

    char comando[10];
    char nome[50], nomeAntigo[50], nomeNovo[50];
    char opcaoMudanca; 

    // --- LOOP DO MENU INTERATIVO ---
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
            printf("Deseja subir (s) ou descer (d)? ");
            scanf(" %c", &opcaoMudanca); 

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
            break; 

        } else {
            printf("Comando inválido. Tente novamente.\n");
        }

    } while (1);
    
    // Requisito final: Exibir toda a estrutura ao encerrar
    printf("\nEstrutura final da árvore de diretórios:\n");
    listarDiretorios(raiz, 0);

    printf("\nDesligando...\n");

    return 0;
}