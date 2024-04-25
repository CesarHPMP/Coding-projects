#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    char *E;
    char *N;
    char *P;
    char *S;
} gramatica;

typedef struct no {
    struct no *esq;
    struct no *dir;
    char *token;
} Node;

int gram_set(gramatica *);
void gram_feed(Node *, gramatica );
int testvar(gramatica , char , size_t );

int main(void) 
{
    Node *arvore;
    gramatica gram;

    arvore = (Node *)malloc(sizeof(Node));

    gram_set(&gram);
    gram_feed(arvore, gram);
    //print_tree(*arvore);

    return 0;
}

int gram_set(gramatica *gram) 
{
    int capacity = 10; // Initial capacity, adjust as needed
    int length = 0;
    char c, d;

    printf("Digite o alfabeto (E): ");
    gram->E = (char *)malloc(capacity * sizeof(char));

    if (gram->E == NULL) 
    {
        return -1; // Memory allocation failed
    }

    while ((c = getchar()) != '\n') 
    {
        // Expand memory if needed
        if (length >= capacity - 1) 
        {
            capacity *= 2; // Double the capacity
            gram->E = (char *)realloc(gram->E, capacity * sizeof(char));
            if (gram->E == NULL) 
            {
                free(gram->E);
                return -1; // Memory allocation failed
            }
        }
        gram->E[length++] = c; // Store the character
    }
    gram->E[length] = '\0'; // Null-terminate the string

    length = 0; // Reset length for reading gram->N

    printf("Digite as variáveis (N): ");
    gram->N = (char *)malloc(capacity * sizeof(char));

    if (gram->N == NULL) 
    {
        free(gram->E); // Free previously allocated memory for gram->E
        return -1; // Memory allocation failed
    }

    // Consume newline character after reading gram->E
    getchar();

    while ((c = getchar()) != '\n' && c != EOF) 
    {
        // Expand memory if needed
        if (length >= capacity - 1) 
        {
            capacity *= 2; // Double the capacity
            gram->N = (char *)realloc(gram->N, capacity * sizeof(char));
            if (gram->N == NULL) 
            {
                free(gram->E); // Free previously allocated memory for gram->E
                free(gram->N); // Free previously allocated memory for gram->N
                return -1; // Memory allocation failed
            }
        }
        printf("\nadded %c\n ", c);
        if (c != ' ') // Ignore spaces
        {
            gram->N[length++] = c; // Store the character
            printf("\nadded %c\n ", gram->N[length]);   
        }
    }
    gram->N[length] = '\0'; // Null-terminate the string

    getchar(); // Consume newline character

    printf("Digite o símbolo inicial (S): ");
    gram->S = (char *)malloc(2 * sizeof(char)); // Allocate memory for S
    gram->S[0] = getchar();
    gram->S[1] = '\0'; // Null-terminate the string

    printf("Digite as regras de produção (P)\n");

    // Allocate memory for P
    capacity = 100; // Initial capacity, adjust as needed
    length = 0;
    gram->P = (char *)malloc(capacity * sizeof(char));

    if (gram->P == NULL) 
    {
        return -1; // Memory allocation failed
    }

    getchar();
    printf("\nChoose variable to develop (press ENTER to move on): ");
    d = getchar();
    getchar();
    
    c = 'a';
    do
    {
        while (c != '\n') 
        {
            if(testvar(*gram, d, 1))
                exit(testvar(*gram, d, 1));
            
            printf("%c -> ", d);
            c = getchar();

            if(testvar(*gram, c, 3))
                exit(testvar(*gram, d, 3));
            
            // Expand memory if needed
            if (length >= capacity - 1) 
            {
                capacity *= 2; // Double the capacity
                gram->P = (char *)realloc(gram->P, capacity * sizeof(char));
                if (gram->P == NULL) 
                {
                free(gram->P);
                    return -1; // Memory allocation failed
                }
            }
            gram->P[length++] = c; // Store the character
        }
        gram->P[length-1] = '\0'; // Null-terminate the string
        getchar();
        scanf("Digite outra variavel para reiniciar [Digite ENTER para sair]: %c", &d);
        getchar();
    } while(d != '\n');
    return 0;
}

void gram_feed(Node *arvore, gramatica gram) 
{
    char *production_rule = gram.P;

    while (*production_rule != '\0') 
    {
        while (*production_rule == ' ' || *production_rule == '\t') 
        {
            production_rule++;
        }

        Node *new_node = (Node *)malloc(sizeof(Node));
        new_node->esq = NULL;
        new_node->dir = NULL;
        new_node->token = NULL;

        new_node->token = production_rule;
        while (*production_rule != '-' && *(production_rule + 1) != '>') 
        {
            production_rule++;
        }
        production_rule += 2;

        while (*production_rule != ';' && *production_rule != '\0') 
        {
            while (*production_rule == ' ' || *production_rule == '\t') 
            {
                production_rule++;
            }

            Node *symbol_node = (Node *)malloc(sizeof(Node));
            symbol_node->esq = NULL;
            symbol_node->dir = NULL;
            symbol_node->token = production_rule;

            while (*production_rule != ' ' && *production_rule != '\t' && *production_rule != ';' && *production_rule != '\0') 
            {
                production_rule++;
            }

            if (new_node->esq == NULL) 
            {
                new_node->esq = symbol_node;
            } 
            else 
            {
                Node *temp = new_node->esq;
                while (temp->dir != NULL) 
                {
                    temp = temp->dir;
                }
                temp->dir = symbol_node;
            }
        }

        if (arvore == NULL) 
        {
            arvore = new_node;
        } 
        else 
        {
            Node *temp = arvore;
            while (temp->esq != NULL) 
            {
                temp = temp->esq;
            }
            temp->esq = new_node;
        }

        while (*production_rule != ';' && *production_rule != '\0') 
        {
            production_rule++;
        }

        if (*production_rule != '\0') 
        {
            production_rule++;
        }
    }
}

int testvar(gramatica gram, char c, size_t opt)
{
    int j, i, d;
    if(opt == 1 || opt == 3)
    {
        if (gram.S[0] == c) 
        {
            return 0; // Match found
        }
        for(j = 0; gram.N[j] != '\0'; j++)
        {
            if(gram.N[j] == c)
            {
                return 0; // Match found
            }
        }
    }
    
    if(opt == 2 || opt == 3)
    {
        for(i = 0; gram.E[i] != '\0'; i++)
        {
            if(gram.E[i] == c)
            {
                return 0; // Match found
            }
        } 
    }
    for(d = 0; d <= i && d <= j; d++)
        {
            printf("%c nao existe em E = %c ou N = %c\n", c, gram.E[d], gram.N[d]);
        } 
    
    return 100; // No match found
}

