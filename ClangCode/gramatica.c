#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

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
void gram_feed(Node *, char * );
int testvar(gramatica , char , size_t );
void print_tree(Node *);
void process_word(Node *, gramatica , char *, size_t );
void free_tree(Node *);
char ** find_rule(char *, char);

int main(void) 
{
    Node *palavra = (Node *)malloc(sizeof(Node)); // Allocate memory for the root node
    if (palavra == NULL) {
        printf("Memory allocation failed for root node.\n");
        return -1;
    }
    palavra->esq = NULL;
    palavra->dir = NULL;
    palavra->token = NULL;


    Node *arvore = (Node *)malloc(sizeof(Node)); // Allocate memory for the root node
    if (arvore == NULL) {
        printf("Memory allocation failed for root node.\n");
        return -1;
    }
    arvore->esq = NULL;
    arvore->dir = NULL;
    arvore->token = NULL;

    gramatica gram;
    char buff[100];
    char *word =(char *)malloc(sizeof(buff)); // Initialize an empty character array to store the word

    gram_set(&gram); // Setting up the grammar
    gram_feed(arvore, gram.P); // Building the parse tree
    find_rule(gram.P, 'b');
    printf("\ngram_feed success\n");
    print_tree(arvore); // Printing the parse tree
    printf("\nprint_tree success\n");
    
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


int gram_set(gramatica *gram) 
{
    int capacity = 100; // Initial capacity, adjust as needed
    char buffer[100];// Buffer for reading a line
    int i;

    // Prompt and read alphabet (E)
    printf("Digite o alfabeto (E): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strlen(buffer) - 1] = '\0'; // Removing the newline character
    gram->E = strdup(buffer);

    // Prompt and read variables (N)
    printf("Digite as variáveis (N): ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strlen(buffer) - 1] = '\0'; // Removing the newline character
    gram->N = strdup(buffer);

    // Allocate memory for S
    gram->S = (char *)malloc(2 * sizeof(char)); // Allocate memory for S
    if (gram->S == NULL) {
        printf("Memory allocation failed for S.\n");
        return -1;
    }

    // Prompt and read initial symbol (S)
    printf("Digite o símbolo inicial (S): ");
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%c", &gram->S[0]);
    gram->S[1] = '\0'; // Null-terminate the string

    // Allocate memory for P
    gram->P = (char *)malloc(capacity * sizeof(char)); // Allocate memory for P
    if (gram->P == NULL) {
        printf("Memory allocation failed for P.\n");
        return -1;
    }

    printf("Digite as regras de produção (P)\n");

    do
    {
        printf("Digite 'var:branch'[apenas enter para sair]");
        fgets(buffer, sizeof(buffer), stdin);
        
        if(buffer[0] == '\n')
            break;
        
        i = 0;
        while (buffer[i] != '\n' && buffer[i] != '\0') 
        {
            if (testvar(*gram, buffer[i], 3)) {
                exit(testvar(*gram, buffer[i], 3));
            }
            i++;
        }
        if (buffer[i] == '\0') {
            printf("Missing newline character in input.\n");
            return -1;
        }
        buffer[i] = '\0'; // Null-terminate the string
        strcat(gram->P, buffer);      
        gram->P[strlen(gram->P)] = ';'; // Add semicolon to indicate the end of a production rule
    
    } while(1);
    return 0;
}


void gram_feed(Node *root, char *production_rule) {
    char buff[100];
    int counter = 0;

    // Skip leading whitespace characters
    while (*production_rule == ' ' || *production_rule == '\t') {
        production_rule++;
    }

    // Check if the production rule string is empty
    if (*production_rule == '\0') {
        printf("\nNo production rules found.\n");
        return;
    }

    // Allocate memory for a new node
    Node *new_node = (Node *)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("Memory allocation failed for new_node.\n");
        return;
    }

    // Initialize the new node
    new_node->esq = NULL;
    new_node->dir = NULL;

    // Copy the production rule into buff
    while (*production_rule != ';') {
        buff[counter++] = *production_rule;
        production_rule++;
    }
    buff[counter] = '\0'; // Null-terminate the string

    // Allocate memory for the token and copy the production rule
    new_node->token = malloc(strlen(buff) + 1); // +1 for the null terminator
    if (new_node->token == NULL) {
        printf("Memory allocation failed for token.\n");
        free(new_node); // Free the new node memory
        return;
    }
    strcpy(new_node->token, buff); // Copy the content of buff into token
     
    printf("\nNew node token-> %s", new_node->token);

    // Move to the end of the production rule
    while (*production_rule != ';' && *production_rule != '\0') {
        production_rule++;
    }

    // Check for end of string
    if (*production_rule == '\0') {
        // If there are no more production rules, exit the function
        return;
    }

    // Null-terminate the production rule and move to the next rule
    *production_rule = '\0';
    production_rule++;

    // Append the new node to the root of the parse tree
    if (root->esq == NULL) {
        printf("\ngoing left");
        root->esq = new_node;
        gram_feed(root, production_rule);
    } else if (root->dir == NULL) {
        printf("\ngoing right");
        root->dir = new_node;
        gram_feed(new_node, production_rule);
    } else {
        printf("Error: Both esq and dir nodes are occupied.\n");
        free(new_node->token); // Free the token memory
        free(new_node); // Free the new node memory
        return;
    }
}


int testvar(gramatica gram, char c, size_t opt)
{
    if(c == ':')
        return 0;
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

void print_tree(Node *root) {
    if (root == NULL) {
        printf("Root node is NULL\n");
        return;
    }

    if (root->token == NULL) {
        printf("Token: (null)\n");
    } else {
        printf("Token: %s\n", root->token); // Print the current node's token
    }

    if(root->esq != NULL)
    {    
        printf("Printing left subtree:\n");
        print_tree(root->esq);
    }
    if(root->dir != NULL)
    {
        printf("Printing right subtree:\n");
        print_tree(root->dir);
    }
    return;
}


void process_word(Node *root, gramatica gram, char *word, size_t w) {// process_word(palavra, &arvore, gram, word)
   //rework whole function. 
    int i = 0, j = 0;
    char c[w];
    root = (Node *)malloc(sizeof(Node));
    if(root == NULL)
        return;

    for(i = 0; *word != '\0'; i++)
    {
        c[i] = *word;
        word++;
    }
    
    
}





void free_tree(Node *root) {
    int n = 0;

    if (root == NULL) {
        return;
    }

    // Free memory for the left subtree
    free_tree(root->esq);

    // Free memory for the right subtree
    free_tree(root->dir);

    // Free memory for the token
    if (root->token != NULL) {
        free(root->token);
    }

    // Free memory for the node itself
    free(root);
}

char ** find_rule(char *rules, char var)
{
    bool s = false;
    char **p;

    do
    {
        switch(*rules)
        {
            case ':':
                s = true;
                break;

            case ';':
                s = false;
                break;
        }

        if(*rules == var && s == false)
        {
            *(p) = &var;
            printf("Value is in char %s and in int %i", var, var);
            p++;
        }
    rules++;
    }while(*rules != '\0');
    return p;
}




