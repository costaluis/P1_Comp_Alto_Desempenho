#include <stdio.h>
#include <stdlib.h>
#define TAM_LINE 1002
#define NUM_CHARS 96

typedef struct charfreq
{
    int code;
    int freq;
} charfreq;

typedef struct list
{
    charfreq *vector;
    char *line;
    struct list *next;
} list;

int main()
{
    list *lines_list = malloc(sizeof(list));
    list *aux = lines_list;
    list *prev;
    char *line = malloc(TAM_LINE * sizeof(char));
    charfreq *vec_freq;
    while (fgets(line, TAM_LINE, stdin))
    {

        aux->line = line;

        vec_freq = malloc(NUM_CHARS * sizeof(charfreq));

        for (int i = 0; i < NUM_CHARS; i++)
        {
            vec_freq[i].code = i + 32;
            vec_freq[i].freq = 0;
        }

        aux->vector = vec_freq;

        aux->next = malloc(sizeof(list));
        prev = aux;
        aux = aux->next;
    }

    free(prev->next);
    prev->next = NULL;

    aux = lines_list;
    while(aux != NULL){
        printf("%s",aux->line);
        aux = aux->next;
    }

}