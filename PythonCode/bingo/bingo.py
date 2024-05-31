import random
import os
import pdfkit
import unittest

SIZE_X = 5
SIZE_Y = 5
NUM_CART_X = 3
NUM_CART_Y = 4
NUM_FOLHAS = 1

class Cartela:
    def __init__(self):
        self.numeros = [[0 for _ in range(SIZE_X)] for _ in range(SIZE_Y)]

def existe_numero_na_coluna(numeros, col, num):
    return any(numeros[i][col] == num for i in range(SIZE_Y))

def gerar_numeros_unicos_para_coluna(numeros, col, min_val, max_val):
    count = 0
    while count < SIZE_Y:
        num = random.randint(min_val, max_val)
        if not existe_numero_na_coluna(numeros, col, num):
            numeros[count][col] = num
            count += 1

def preencher_cartela(cartela):
    ranges = [
        (1, 15),   # Coluna 1: 1-15
        (16, 30),  # Coluna 2: 16-30
        (31, 45),  # Coluna 3: 31-45
        (46, 60),  # Coluna 4: 46-60
        (61, 75)   # Coluna 5: 61-75
    ]
    for col in range(SIZE_X):
        gerar_numeros_unicos_para_coluna(cartela.numeros, col, ranges[col][0], ranges[col][1])

def imprimir_cartela_html(arquivo, cartelas, folha):
    arquivo.write("<!DOCTYPE html>\n<html>\n<head>\n")
    arquivo.write("<style>\n")
    arquivo.write("@media print {\n.cartela {\npage-break-before: never;\n}\n}")
    arquivo.write("body { margin: 0; padding: 0; background-image: url('background.jpg'); background-size: 210mm 297mm; background-repeat: no-repeat;}\n")
    arquivo.write(".container { display: grid; grid-template-columns: repeat(%d, 1fr); grid-template-rows: repeat(%d, 1fr); height: 279mm; width: 204mm; grid-gap: 0px; padding: 0px; margin-top: 65px; margin-left: 10px;}\n" % (NUM_CART_X, NUM_CART_Y))
    arquivo.write(".cartela { height: auto; background: rgba(255, 255, 255, 0.0); border-radius: 0px; padding: 0px; display: flex; justify-content: center; align-items: center;}\n")
    arquivo.write(".folha {text-align: right; font-size: 15px; display: flex; justify-content: flex-end; margin: 10px;}\n")
    arquivo.write(".conteudo { width: 100%%; height: 100%%; display: table;}\n")
    arquivo.write(".number { font-size: 20px; font-weight: bold; text-align: center; width: 35px; height: 35px; display: flex; align-items: center; justify-content: center; }\n")
    arquivo.write("table {  width: 100%%; height: 100%%; border-collapse: collapse; tab-size: inherit; }\n")
    arquivo.write("td { border: none; width: 40px; height: 37px; text-align: center; vertical-align: middle; font-size: 20px; font-weight: bold; }\n")
    arquivo.write("</style>\n")
    arquivo.write("</head>\n<body>\n")
    arquivo.write("<div class=\"container\">\n")

    for i in range(NUM_CART_X):
        for j in range(NUM_CART_Y):
            arquivo.write("<div class=\"cartela\">\n")
            arquivo.write("<table>\n")
            for m in range(SIZE_X):
                arquivo.write("<tr>\n")
                for n in range(SIZE_Y):
                    if m == 2 and n == 2:
                        arquivo.write("<td></td>\n")  # Center cell left blank
                    else:
                        arquivo.write("<td>%d</td>\n" % cartelas[i][j].numeros[n][m])
                arquivo.write("</tr>\n")
            arquivo.write("</table>\n")
            arquivo.write("</div>\n")
    
    arquivo.write("</div>\n")
    arquivo.write("<div class=\"folha\">Cartela N°%d</div>\n" % folha)
    arquivo.write("</body>\n</html>\n")

def main():
    for f in range(NUM_FOLHAS):
        nome_arquivo = "folha_%d.html" % (f + 1)
        try:
            with open(nome_arquivo, "w") as arquivo_html:
                cartelas = [[Cartela() for _ in range(NUM_CART_X)] for _ in range(NUM_CART_Y)]
                for i in range(NUM_CART_Y):
                    for j in range(NUM_CART_X):
                        preencher_cartela(cartelas[i][j])
                imprimir_cartela_html(arquivo_html, cartelas, f + 1)
            print("Cartela de bingo foi impressa no arquivo", nome_arquivo)
        except IOError as e:
            print(f"Erro ao escrever o arquivo {nome_arquivo}: {e}")

    path_to_wkhtmltopdf = r'/usr/bin/wkhtmltopdf'
    config = pdfkit.configuration(wkhtmltopdf=path_to_wkhtmltopdf)
    options = {
        'page-size': 'A4',
        'margin-top': '0mm',
        'margin-bottom': '0mm',
        'margin-left': '0mm',
        'margin-right': '0mm',
        'enable-local-file-access': None,
        'header-right': '[page]',
        'footer-center': 'Bingo Cards - Page [page] of [toPage]',
        'footer-font-size': '10',
        'zoom': '3'
    }

    for f in range(NUM_FOLHAS):
        input_file = "folha_%d.html" % (f + 1)
        output_file = "folha_%d.pdf" % (f + 1)
        
        if os.path.exists(input_file):
            try:
                pdfkit.from_file(input_file, output_file, configuration=config, options=options)
                print("Convertido", input_file, "para", output_file)
            except OSError as e:
                print(f"Erro ao converter {input_file} para PDF: {e}")
        else:
            print(input_file, "não existe")

if __name__ == "__main__":
    main()

# Unit tests
class TestBingo(unittest.TestCase):
    def test_existe_numero_na_coluna(self):
        numeros = [
            [1, 0, 0, 0, 0],
            [2, 0, 0, 0, 0],
            [3, 0, 0, 0, 0],
            [4, 0, 0, 0, 0],
            [5, 0, 0, 0, 0]
        ]
        self.assertTrue(existe_numero_na_coluna(numeros, 0, 3))
        self.assertFalse(existe_numero_na_coluna(numeros, 0, 6))

    def test_gerar_numeros_unicos_para_coluna(self):
        numeros = [[0 for _ in range(SIZE_X)] for _ in range(SIZE_Y)]
        gerar_numeros_unicos_para_coluna(numeros, 0, 1, 15)
        coluna_numeros = [numeros[i][0] for i in range(SIZE_Y)]
        self.assertEqual(len(coluna_numeros), len(set(coluna_numeros)))  # Check if all numbers are unique
        self.assertTrue(all(1 <= num <= 15 for num in coluna_numeros))  # Check if numbers are within the range

    def test_preencher_cartela(self):
        cartela = Cartela()
        preencher_cartela(cartela)
        for col in range(SIZE_X):
            coluna_numeros = [cartela.numeros[row][col] for row in range(SIZE_Y)]
            min_val, max_val = 1 + 15 * col, 15 * (col + 1)
            self.assertEqual(len(coluna_numeros), len(set(coluna_numeros)))  # Unique numbers
            self.assertTrue(all(min_val <= num <= max_val for num in coluna_numeros))  # Within range

if __name__ == "__main__":
    unittest.main()

