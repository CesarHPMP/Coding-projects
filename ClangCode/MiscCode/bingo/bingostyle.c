#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SIZE_X 5
#define SIZE_Y 5
#define NUM_CART_X 3
#define NUM_CART_Y 4
#define NUM_FOLHAS 1

typedef struct {
    int numeros[SIZE_Y][SIZE_X];
} Cartela;

bool existeNumeroNaColuna(int numeros[SIZE_Y][SIZE_X], int col, int num) {
    for (int i = 0; i < SIZE_Y; i++) {
        if (numeros[i][col] == num) {
            return true;
        }
    }
    return false;
}

void gerarNumerosUnicosParaColuna(int numeros[SIZE_Y][SIZE_X], int col, int min, int max) {
    int count = 0;
    while (count < SIZE_Y) {
        int num = rand() % (max - min + 1) + min;
        if (!existeNumeroNaColuna(numeros, col, num)) {
            numeros[count][col] = num;
            count++;
        }
    }
}

void preencherCartela(Cartela *cartela) {
    int ranges[SIZE_X][2] = {
        {1, 15},
        {16, 30},
        {31, 45},
        {46, 60},
        {61, 75}
    };

    for (int col = 0; col < SIZE_X; col++) {
        gerarNumerosUnicosParaColuna(cartela->numeros, col, ranges[col][0], ranges[col][1]);
    }
}

void imprimirCartelaHTML(FILE *arquivo, Cartela cartelas[NUM_CART_Y][NUM_CART_X], int folha) {
    int offsetX = 0;
    int offsetY = 0;
    int cellWidth = 35;
    int cellHeight = 35;

    fprintf(arquivo, "<!DOCTYPE html>\n<html>\n<head>\n");
    fprintf(arquivo, "<style>\n");
    fprintf(arquivo, "body {transform: scale(1); transform-origin: 0 0; }\n"); // Scaling CSS
    fprintf(arquivo, ".container { display: grid; grid-template-columns: repeat(%d, 1fr); grid-template-rows: repeat(%d, 1fr); background-image: url('file:///home/cesar/Coding-projects/ClangCode/MiscCode/bingo/background.jpg'); background-size: contain; background-repeat: no-repeat; height: 297mm; width: 210mm; grid-gap: 50px; grid-row-gap: 30px; justify-content: center;}\n", NUM_CART_X, NUM_CART_Y);
    fprintf(arquivo, ".cartela { display: grid; grid-template-columns: repeat(%d, 1fr); grid-template-rows: repeat(%d, 1fr); position: relative; width: 100%%; height: 100%%; }\n", NUM_CART_X, NUM_CART_Y);
    fprintf(arquivo, ".folha{text-align: right; font-size: 15px; display: flex; }\n");
    fprintf(arquivo, ".number { position: absolute; font-size: 20px; font-weight: bold; text-align: center; width: %dpx; height: %dpx; display: flex; align-items: center; justify-content: center; }\n", cellWidth, cellHeight);
    
    fprintf(arquivo, "</style>\n");
    fprintf(arquivo, "</head>\n<body>\n");
    fprintf(arquivo, "<div class=\"container\">\n");

    for (int i = 0; i < NUM_CART_Y; i++) {
        for (int j = 0; j < NUM_CART_X; j++) {
            fprintf(arquivo, "<div class=\"cartela\">\n");
            for (int m = 0; m < SIZE_Y; m++) {
                for (int n = 0; n < SIZE_X; n++) {
                    if (m == 0 && n == 0) {
                        int x = offsetX + n * cellWidth;
                        int y = offsetY + m * cellHeight;
                        fprintf(arquivo, "<div class=\"number\" style=\"left:%dpx; top:%dpx;\">%d</div>\n", x, y, cartelas[i][j].numeros[m][n]);
                    } else if (!(m == 2 && n == 2)) {
                        int x = offsetX + n * cellWidth;
                        int y = offsetY + m * cellHeight;
                        fprintf(arquivo, "<div class=\"number\" style=\"left:%dpx; top:%dpx;\">%d</div>\n", x, y, cartelas[i][j].numeros[m][n]);
                    }
                }
            }
            fprintf(arquivo, "</div>\n");
        }
    }

    fprintf(arquivo, "</div>\n");
    fprintf(arquivo, "</body>\n</html>\n");
    fprintf(arquivo, "<h class=\"folha\">Cartela N°%5d\n\n</h>\n", folha);
}

int main() {
    srand(time(NULL));

    for (int f = 0; f < NUM_FOLHAS; f++) {
        char nome_arquivo[50];
        sprintf(nome_arquivo, "folha_%d.html", f + 1);
        FILE *arquivoHTML = fopen(nome_arquivo, "w");
        if (arquivoHTML == NULL) {
            printf("Erro ao abrir o arquivo HTML.\n");
            return 1;
        }

        Cartela cartelas[NUM_CART_Y][NUM_CART_X];
        for (int i = 0; i < NUM_CART_Y; i++) {
            for (int j = 0; j < NUM_CART_X; j++) {
                preencherCartela(&cartelas[i][j]);
            }
        }

        imprimirCartelaHTML(arquivoHTML, cartelas, f + 1);
        fclose(arquivoHTML);
    }

    printf("Cartelas de bingo foram impressas em arquivos individuais.\n");

    return 0;
}

