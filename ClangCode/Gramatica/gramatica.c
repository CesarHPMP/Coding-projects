#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "grams.h"
#include "word.h"
#ifndef GRAM_H__
#define GRAM_H__
#include "gram.h"
#endif

int main(void)
{
    Node *palavra;
    palavra = (Node *)malloc(sizeof(Node)); // Allocate memory for the root node
    if (palavra == NULL) {
        printf("Memory allocation failed for root node.\n");
        return -1;
    }
    palavra->esq = NULL;
    palavra->dir = NULL;
    palavra->token = NULL;


    Node *arvore;
    arvore = (Node *)malloc(sizeof(Node *));
    if (arvore == NULL) {
        printf("Memory allocation failed for root node.\n");
        return -1;
    }
    arvore->esq = NULL;
    arvore->dir = NULL;
    arvore->token = NULL;

    gramatica gram;
    char c = 'b';
    char **p;
    char buff[100];
    char *word = (char *)malloc(sizeof(buff)); // Initialize an empty character array to store the word

    gram_set(&gram); // Setting up the grammar
    //gram_feed(arvore, gram.P); // Building the parse tree
    //gram.P[strlen(gram.P)] = (char *)malloc(sizeof(char *));
    //gram.P[strlen(gram.P)] = '\n'
    

    p = (char **)malloc(strlen(gram.P) * sizeof(char *)); // Allocate memory for an array of char pointers

    for (int i = 0; i < strlen(gram.P); i++) 
    {
        p[i] = (char *)malloc(sizeof(char *)); // Allocate memory for each string
    }

    p = find_rule(gram.P, c);
    while(*(*p) != '\0')
    {
        printf("\n MATCH %c\n", *(*p));
        *p++;
    }
    //print_tree(arvore); // Printing the parse tree
    //printf("\nprint_tree success\n");

    printf("Digite a palavra: ");
    fgets(buff, sizeof(buff), stdin);
    buff[strlen(buff) - 1] = '\0'; // Removing the newline character
    word = strdup(buff);


    process_word(palavra, gram, word, strlen(word)); // Generating the word
    printf("Generated word: %s\n", word); // Printing the generated word
    // Free allocated memory for the parse tree
    free_tree(arvore);

    return 0;
}

