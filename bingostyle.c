/* bingostyle.c
 *
 * Copyright 2024 CesarHPMP
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

//Programa que faz cartelas de bingo em folhas A4, contendo 12 cartelas por folha

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SIZE_X 5
#define SIZE_Y 6
#define NUM_CART_X 3
#define NUM_CART_Y 4
#define NUM_FOLHAS 1000

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
    fprintf(arquivo, "<!DOCTYPE html>\n<html>\n<head>\n");
    fprintf(arquivo, "<title>Cartelas de Bingo</title>\n");
    fprintf(arquivo, "<style>\n");
    fprintf(arquivo, "@page { size: 210mm 297mm; margin: 10mm }\n"); // Define o tamanho da página como A4 e as margens como 10mm
    fprintf(arquivo, "table { border-collapse: collapse; }\n");
    fprintf(arquivo, "td { border: 2px solid #dddddd; width: 30px; height: 30px; text-align: center; padding: 0; }\n");
    fprintf(arquivo, "td.central { background-color: #ffcccc; }\n");
    fprintf(arquivo, "td.vertical { border-left: 4px solid #000; }\n");
    fprintf(arquivo, "tr.horizontal { border-top: 4px solid #000; }\n");
    fprintf(arquivo, ".titulo { text-align: center; font-weight: bold; font-size: 24px; margin-bottom: 10px; }\n");
    fprintf(arquivo, ".container { display: flex; flex-direction: column; align-items: center; }\n");
    fprintf(arquivo, ".linha { display: flex; }\n");
    fprintf(arquivo, ".cartela { margin-right: 20px; margin-bottom: 20px; }\n"); // Adicionando margem entre as cartelas
    fprintf(arquivo, ".letra { font-weight: bold; font-size: 24px; }\n");
    fprintf(arquivo, "</style>\n");
    fprintf(arquivo, "</head>\n<body>\n");

    // Título "BINGO"
    fprintf(arquivo, "<div class=\"titulo\">BINGO</div>\n");

    // Cartelas de Bingo
    fprintf(arquivo, "<div class=\"container\">\n");

    for (int i = 0; i < NUM_CART_Y; i++) {
        fprintf(arquivo, "<div class=\"linha\">\n");
        for (int j = 0; j < NUM_CART_X; j++) {
            fprintf(arquivo, "<div class=\"cartela\">\n");
            fprintf(arquivo, "<table>\n");
            // Soletra "BINGO" acima de cada cartela
            fprintf(arquivo, "<tr>\n");
            fprintf(arquivo, "<td class=\"letra\">B</td>\n");
            fprintf(arquivo, "<td class=\"letra\">I</td>\n");
            fprintf(arquivo, "<td class=\"letra\">N</td>\n");
            fprintf(arquivo, "<td class=\"letra\">G</td>\n");
            fprintf(arquivo, "<td class=\"letra\">O</td>\n");
            fprintf(arquivo, "</tr>\n");
            for (int m = 0; m < SIZE_Y - 1; m++) { // Iterar até SIZE_Y - 1 para excluir a linha de soletração "BINGO"
                fprintf(arquivo, "<tr>\n");
                for (int n = 0; n < SIZE_X; n++) {
                    if (m == SIZE_Y / 3 && n == SIZE_X / 2) {
                        fprintf(arquivo, "<td class=\"central\"><b>XX</b></td>\n"); // Espaço central (coringa)
                    } else {
                        fprintf(arquivo, "<td%s>%2d</td>\n", n == 0 ? " class=\"vertical\"" : "", cartelas[i][j].numeros[m][n]);
                    }
                }
                fprintf(arquivo, "</tr>\n");
            }
            fprintf(arquivo, "</table>\n");
            fprintf(arquivo, "</div>\n");
        }
        fprintf(arquivo, "</div>\n");
    }

    fprintf(arquivo, "</div>\n");

    fprintf(arquivo, "</body>\n</html>\n");
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
