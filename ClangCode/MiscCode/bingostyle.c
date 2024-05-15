#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SIZE_X 5
#define SIZE_Y 5
#define NUM_CART_X 3
#define NUM_CART_Y 4
#define NUM_FOLHAS 1

// Definição da estrutura para representar uma cartela de bingo
typedef struct {
    int numeros[SIZE_Y][SIZE_X];
} Cartela;

// Função para verificar se um número já existe em uma matriz
bool existeNumero(int numeros[SIZE_Y][SIZE_X], int num) {
    for (int i = 0; i < SIZE_Y; i++) {
        for (int j = 0; j < SIZE_X; j++) {
            if (numeros[i][j] == num) {
                return true;
            }
        }
    }
    return false;
}

// Função para gerar números únicos dentro de um intervalo
void gerarNumerosUnicos(int numeros[SIZE_Y][SIZE_X], int min, int max) {
    int count = 0;
    while (count < SIZE_X * SIZE_Y) {
        max = (count % SIZE_X) * 15 + 15;
        min = (count % SIZE_X) * 15;
        int num = rand() % 15 + 1 + min;
        if (!existeNumero(numeros, num)) {
            numeros[count / SIZE_X][count % SIZE_X] = num;
            count++;
        }
    }
}

// Função para preencher uma cartela de bingo
void preencherCartela(Cartela *cartela) {
    // Gera números únicos para preencher a cartela (1 a 75)
    gerarNumerosUnicos(cartela->numeros, 1, 75);
}

// Função para imprimir uma cartela de bingo em HTML com estilos CSS
void imprimirCartelaHTML(FILE *arquivo, Cartela cartelas[NUM_CART_Y][NUM_CART_X], int folha) {
    fprintf(arquivo, "<!DOCTYPE html>\n\n<head>\n");
    fprintf(arquivo, "<style>\n");
    fprintf(arquivo, ".page { size: 248px 308px; margin: 10mm 10mm; align: center;}\n"); // Define o tamanho da página como A4 e as margens como 10mm
    fprintf(arquivo, "body { align: center; background-image: url('background.jpeg'); background-size: 300px 550px; width: 248px; height: 308px; background-repeat: no-repeat;}\n");
    fprintf(arquivo, ".container { display: grid; grid-template-columns: repeat(%d, auto); grid-gap: 5px; justify-content: center; }\n", NUM_CART_X);
    fprintf(arquivo, ".cartela { margin-bottom: 5px; }\n");
    fprintf(arquivo, "table { border-collapse: collapse; }\n");
    fprintf(arquivo, ".titulo { text-align: center; font-weight: bold; font-size: 24px; margin-bottom: 10px; }\n");
    fprintf(arquivo, ".letra { font-weight: bold; font-size: 24px; }\n");
    fprintf(arquivo, "</style>\n");  
    fprintf(arquivo, "</head>\n<body class=\"page\">\n");

    // Cartelas de Bingo
    fprintf(arquivo, "<div class=\"container\">\n");

    for (int i = 0; i < NUM_CART_Y; i++) {
        for (int j = 0; j < NUM_CART_X; j++) {
            fprintf(arquivo, "<div class=\"cartela\">\n");
            fprintf(arquivo, "<table>\n"); 
            for (int m = 0; m < SIZE_Y; m++) { // Iterar até SIZE_Y 
                fprintf(arquivo, "<tr>\n");
                for (int n = 0; n < SIZE_X; n++) {
                    if (m == SIZE_Y / 2 && n == SIZE_X / 2) {
                        fprintf(arquivo, "<td><b></b></td>\n"); // Espaço central (coringa)
                    } else {
                        fprintf(arquivo, "<td>%2d</td>\n", cartelas[i][j].numeros[m][n]);
                    }
                }
                fprintf(arquivo, "</tr>\n");
            }
            fprintf(arquivo, "</table>\n");
            fprintf(arquivo, "</div>\n");
        }
    }

    fprintf(arquivo, "</div>\n");

    fprintf(arquivo, "</body>\n");
}

int main() {
    srand(time(NULL)); // Inicializa o gerador de números aleatórios

    for (int f = 0; f < NUM_FOLHAS; f++) {
        char nome_arquivo[50];
        sprintf(nome_arquivo, "folha_%d.html", f + 1); // Nome do arquivo com base no número da folha
        FILE *arquivoHTML = fopen(nome_arquivo, "w"); // Abre o arquivo HTML para escrita
        if (arquivoHTML == NULL) {
            printf("Erro ao abrir o arquivo HTML.\n");
            return 1;
        }

        Cartela cartelas[NUM_CART_Y][NUM_CART_X]; // Matriz para armazenar as cartelas desta folha
        for (int i = 0; i < NUM_CART_Y; i++) {
            for (int j = 0; j < NUM_CART_X; j++) {
                preencherCartela(&cartelas[i][j]); // Preenche as cartelas desta folha
            }
        }

        imprimirCartelaHTML(arquivoHTML, cartelas, f + 1); // Imprime as cartelas nesta folha
        fclose(arquivoHTML); // Fecha o arquivo HTML
    }

    printf("Cartelas de bingo foram impressas em arquivos individuais.\n");

    return 0;
}

