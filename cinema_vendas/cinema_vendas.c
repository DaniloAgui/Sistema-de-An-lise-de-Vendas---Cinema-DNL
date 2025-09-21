
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdlib.h>

// =============================================================
//                       CONSTANTES
// =============================================================
#define DNL_FILMES      100     // Limite máximo de filmes
#define DNL_NOME        100     // Tamanho máximo do nome do filme
#define BARRA_DNL       60      // Largura máxima das barras no gráfico
#define NOME_DISPLAY    25      // Largura para exibição dos nomes
#define DIAS_PERIODO    7      // Período de análise em dias

// =============================================================
//                    ESTRUTURA DE DADOS
// =============================================================
typedef struct {
    char nome[DNL_NOME];
    int ingressosPorDia[DIAS_PERIODO];  // Vendas diárias durante 7 dias
    int totalIngressos;                 // Total acumulado no período
} Filme;

typedef struct {
    long long total;
    double media;
    int maximo;
    int minimo;
} Estatisticas;

// =============================================================
//                  PROTÓTIPOS DAS FUNÇÕES
// =============================================================
void exibirCabecalho(void);
void exibirSeparador(char caractere, int tamanho);
void limparBuffer(void);
int lerQuantidadeFilmes(void);
void cadastrarFilme(Filme *filme, int numero);
void coletarVendasDiarias(Filme *filme);
void calcularTotaisFilmes(Filme filmes[], int quantidade);
void calcularEstatisticas(Filme filmes[], int quantidade, Estatisticas *stats);
void exibirResumo(Filme filmes[], int quantidade, Estatisticas stats);
void exibirFilmesPorCategoria(Filme filmes[], int quantidade, int valor, const char *categoria);
void exibirGrafico(Filme filmes[], int quantidade, int maximo);
void exibirVendasDiarias(Filme filmes[], int quantidade);
void exibirRodape(void);
bool validarEntradaNumerica(int *valor);
void formatarNome(char *nome);

// =============================================================
//                     FUNÇÃO PRINCIPAL
// =============================================================
int main(void) {
    Filme filmes[DNL_FILMES];
    Estatisticas stats;
    int quantidadeFilmes;
    
    // Interface inicial
    exibirCabecalho();
    
    // Entrada de dados
    quantidadeFilmes = lerQuantidadeFilmes();
    
    printf("\n");
    exibirSeparador('=', 60);
    printf("            CADASTRO DOS FILMES\n");
    exibirSeparador('=', 60);
    
    for (int i = 0; i < quantidadeFilmes; i++) {
        cadastrarFilme(&filmes[i], i + 1);
    }
    
    // Coleta de vendas diárias por 7 dias
    printf("\n");
    exibirSeparador('=', 70);
    printf("           PERÍODO DE COLETA (7 DIAS)\n");
    exibirSeparador('=', 70);
    printf("\nSimulando período de vendas de 7 dias...\n");
    
    for (int i = 0; i < quantidadeFilmes; i++) {
        coletarVendasDiarias(&filmes[i]);
    }
    
    // Processamento dos totais e estatísticas
    calcularTotaisFilmes(filmes, quantidadeFilmes);
    calcularEstatisticas(filmes, quantidadeFilmes, &stats);
    
    // Saída de resultados
    exibirVendasDiarias(filmes, quantidadeFilmes);
    exibirResumo(filmes, quantidadeFilmes, stats);
    exibirGrafico(filmes, quantidadeFilmes, stats.maximo);
    exibirRodape();
    
    return 0;
}

// =============================================================
//                IMPLEMENTAÇÃO DAS FUNÇÕES
// =============================================================

/**
 * Exibe o cabeçalho inicial do programa
 */
void exibirCabecalho(void) {
    system("clear || cls"); // Limpa a tela (funciona no Linux e Windows)
    
    exibirSeparador('=', 70);
    printf("                  SISTEMA DE ANÁLISE DE VENDAS\n");
    printf("                         CINEMA DNL v2.0\n");
    exibirSeparador('=', 70);
    printf("\n📊 ANÁLISE DE VENDAS - PERÍODO DE %d DIAS\n\n", DIAS_PERIODO);
    printf("Este sistema permite:\n");
    printf("• Cadastrar filmes para análise de vendas\n");
    printf("• Coletar vendas diárias por %d dias consecutivos\n", DIAS_PERIODO);
    printf("• Calcular estatísticas detalhadas do período\n");
    printf("• Identificar filmes de maior e menor sucesso\n");
    printf("• Visualizar dados através de gráfico ASCII\n");
    printf("• Acompanhar evolução das vendas dia a dia\n");
    printf("\n");
    exibirSeparador('-', 70);
}

/**
 * Exibe uma linha separadora
 */
void exibirSeparador(char caractere, int tamanho) {
    for (int i = 0; i < tamanho; i++) {
        putchar(caractere);
    }
    putchar('\n');
}

/**
 * Limpa o buffer de entrada
 */
void limparBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/**
 * Lê e valida a quantidade de filmes
 */
int lerQuantidadeFilmes(void) {
    int quantidade;
    
    do {
        printf("\n Informe a quantidade de filmes (1 a %d): ", DNL_FILMES);

        if (!validarEntradaNumerica(&quantidade)) {
            printf(" Entrada inválida! Digite apenas números.\n");
            continue;
        }

        if (quantidade < 1 || quantidade > DNL_FILMES) {
            printf(" Quantidade deve estar entre 1 e %d filmes.\n", DNL_FILMES);
        } else {
            printf(" Quantidade confirmada: %d filme(s)\n", quantidade);
            break;
        }
    } while (true);
    
    return quantidade;
}

/**
 * Valida entrada numérica
 */
bool validarEntradaNumerica(int *valor) {
    if (scanf("%d", valor) != 1) {
        limparBuffer();
        return false;
    }
    limparBuffer();
    return true;
}

/**
 * Cadastra um filme com validação
 */
void cadastrarFilme(Filme *filme, int numero) {
    printf("\n📽️  FILME %d\n", numero);
    printf("────────────────────────────────────────\n");
    
    // Leitura do nome
    printf("Nome do filme: ");
    if (!fgets(filme->nome, DNL_NOME, stdin)) {
        strcpy(filme->nome, "Filme Sem Título");
    }
    
    // Remove quebra de linha e formata
    filme->nome[strcspn(filme->nome, "\r\n")] = '\0';
    formatarNome(filme->nome);
    
    // Inicializa as vendas diárias
    for (int i = 0; i < DIAS_PERIODO; i++) {
        filme->ingressosPorDia[i] = 0;
    }
    filme->totalIngressos = 0;
    
    printf("✅ Filme \"%s\" cadastrado! Vendas serão coletadas por %d dias.\n", 
           filme->nome, DIAS_PERIODO);
}

/**
 * Formata o nome do filme (primeira letra maiúscula)
 */
void formatarNome(char *nome) {
    if (strlen(nome) == 0) {
        strcpy(nome, "Filme Sem Nome");
        return;
    }
    
    // Primeira letra maiúscula
    if (islower(nome[0])) {
        nome[0] = toupper(nome[0]);
    }
}

/**
 * Coleta vendas diárias para um filme durante 15 dias
 */
void coletarVendasDiarias(Filme *filme) {
    printf("\n🎬 Coletando vendas para: %s\n", filme->nome);
    printf("──────────────────────────────────────────\n");
    
    for (int dia = 0; dia < DIAS_PERIODO; dia++) {
        int vendas;
        do {
            printf("Dia %2d - Ingressos vendidos: ", dia + 1);
            
            if (!validarEntradaNumerica(&vendas)) {
                printf("❌ Digite apenas números inteiros!\n");
                continue;
            }
            
            if (vendas < 0) {
                printf("❌ O número de ingressos não pode ser negativo!\n");
            } else {
                filme->ingressosPorDia[dia] = vendas;
                break;
            }
        } while (true);
    }
    
    printf("✅ Coleta concluída para \"%s\"\n", filme->nome);
}

/**
 * Calcula o total de ingressos para cada filme
 */
void calcularTotaisFilmes(Filme filmes[], int quantidade) {
    for (int i = 0; i < quantidade; i++) {
        filmes[i].totalIngressos = 0;
        for (int dia = 0; dia < DIAS_PERIODO; dia++) {
            filmes[i].totalIngressos += filmes[i].ingressosPorDia[dia];
        }
    }
}

/**
 * Calcula todas as estatísticas
 */
void calcularEstatisticas(Filme filmes[], int quantidade, Estatisticas *stats) {
    stats->total = 0;
    stats->maximo = filmes[0].totalIngressos;
    stats->minimo = filmes[0].totalIngressos;
    
    for (int i = 0; i < quantidade; i++) {
        stats->total += filmes[i].totalIngressos;
        
        if (filmes[i].totalIngressos > stats->maximo) {
            stats->maximo = filmes[i].totalIngressos;
        }
        
        if (filmes[i].totalIngressos < stats->minimo) {
            stats->minimo = filmes[i].totalIngressos;
        }
    }
    
    stats->media = (quantidade > 0) ? (double)stats->total / quantidade : 0.0;
}

/**
 * Exibe o resumo das estatísticas
 */
void exibirResumo(Filme filmes[], int quantidade, Estatisticas stats) {
    printf("\n\n");
    exibirSeparador('=', 70);
    printf("                         RELATÓRIO FINAL\n");
    exibirSeparador('=', 70);
    
    // Estatísticas gerais
    printf("\n ESTATÍSTICAS GERAIS:\n");
    printf("──────────────────────────────────────────────────────────────────\n");
    printf("Período de análise: %d dias consecutivos\n", DIAS_PERIODO);
    printf("Total de filmes analisados: %d\n", quantidade);
    printf("Total de ingressos vendidos: %lld\n", stats.total);
    printf("Média de ingressos por filme (período): %.2f\n", stats.media);
    printf("Média diária geral: %.2f ingressos/dia\n", (double)stats.total / (DIAS_PERIODO * quantidade));
    printf("Maior número de ingressos (período): %d\n", stats.maximo);
    printf("Menor número de ingressos (período): %d\n", stats.minimo);
    
    // Filmes de maior sucesso
    printf("\n FILMES DE MAIOR SUCESSO (%d ingressos):\n", stats.maximo);
    exibirFilmesPorCategoria(filmes, quantidade, stats.maximo, "sucesso");
    
    // Filmes de menor audiência
    printf("\n FILMES DE MENOR AUDIÊNCIA (%d ingressos):\n", stats.minimo);
    exibirFilmesPorCategoria(filmes, quantidade, stats.minimo, "menor");
}

/**
 * Exibe filmes por categoria (maior/menor sucesso)
 */
void exibirFilmesPorCategoria(Filme filmes[], int quantidade, int valor, const char *categoria) {
    int contador = 0;
    
    for (int i = 0; i < quantidade; i++) {
        if (filmes[i].totalIngressos == valor) {
            contador++;
            printf("  %d. %s\n", contador, filmes[i].nome);
        }
    }
    
    if (contador == 0) {
        printf("  (Nenhum filme encontrado)\n");
    }
}

/**
 * Exibe o gráfico ASCII
 */
void exibirGrafico(Filme filmes[], int quantidade, int maximo) {
    printf("\n\n");
    exibirSeparador('=', 70);
    printf("                      GRÁFICO DE VENDAS\n");
    exibirSeparador('=', 70);
    
    if (maximo == 0) {
        printf("\n Não há dados para exibir (todos os filmes com 0 ingressos)\n");
        return;
    }
    
    printf("\n");
    for (int i = 0; i < quantidade; i++) {
        // Calcula o número de barras proporcionalmente
        int barras = (int)((double)filmes[i].totalIngressos * BARRA_DNL / maximo + 0.5);
        
        // Exibe o nome formatado
        printf("%-*.*s │ ", NOME_DISPLAY, NOME_DISPLAY, filmes[i].nome);
        
        // Exibe as barras
        for (int j = 0; j < barras; j++) {
            printf("█");
        }
        
        // Exibe o valor numérico
        printf(" %d\n", filmes[i].totalIngressos);
    }
    
    printf("\n");
    exibirSeparador('-', 70);
    double valorPorBarra = (double)maximo / BARRA_DNL;
    printf(" Legenda: cada FILME representa aproximadamente %.2f ingressos\n", valorPorBarra);
    printf(" Escala máxima: %d ingressos\n", maximo);
}

/**
 * Exibe as vendas diárias de todos os filmes
 */
void exibirVendasDiarias(Filme filmes[], int quantidade) {
    printf("\n\n");
    exibirSeparador('=', 70);
    printf("                    VENDAS DIÁRIAS (%d DIAS)\n", DIAS_PERIODO);
    exibirSeparador('=', 70);
    
    for (int i = 0; i < quantidade; i++) {
        printf("\n📽️  %s\n", filmes[i].nome);
        exibirSeparador('-', 50);
        
        // Exibe vendas por semana para melhor visualização
        for (int semana = 0; semana < 3; semana++) {
            int diaInicio = semana * 5;
            int diaFim = (semana == 2) ? DIAS_PERIODO : (semana + 1) * 5;
            
            printf("Semana %d: ", semana + 1);
            for (int dia = diaInicio; dia < diaFim; dia++) {
                if (dia < DIAS_PERIODO) {
                    printf("D%d:%3d  ", dia + 1, filmes[i].ingressosPorDia[dia]);
                }
            }
            printf("\n");
        }
        
        printf("📊 Total acumulado: %d ingressos\n", filmes[i].totalIngressos);
        
        // Calcula média diária
        double mediaDiaria = (double)filmes[i].totalIngressos / DIAS_PERIODO;
        printf("📈 Média diária: %.2f ingressos\n", mediaDiaria);
    }
}

/**
 * Exibe o rodapé final
 */
void exibirRodape(void) {
    printf("\n\n");
    exibirSeparador('=', 70);
    printf("                   ANÁLISE CONCLUÍDA COM SUCESSO!\n");
    printf("                     Sistema de informação v2.0 - 2025\n");
    exibirSeparador('=', 70);
    printf("\n Obrigado por usar nosso sistema de análise!\n");
    printf(" Dica: Use estes dados para tomar decisões estratégicas.\n\n");
}
