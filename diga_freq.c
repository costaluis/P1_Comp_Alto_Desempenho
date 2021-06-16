#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define TAM_LINE 1002
#define NUM_CHARS 96
#define T 8

typedef struct charfreq
{
    int code, freq;
} charfreq;

typedef struct list
{
    charfreq *vector;
    char *line;
    struct list *next;
} list;

void free_memory(list * aux){
    if(aux->next){
        free_memory(aux->next);
    }
    free(aux->line);
    free(aux->vector);
    free(aux);
}

int compare(const void *p1, const void *p2)
{
    charfreq *cf1 = (charfreq *)p1;
    charfreq *cf2 = (charfreq *)p2;

    if (cf1->freq != cf2->freq)
        return cf1->freq - cf2->freq;

    return cf2->code - cf1->code;
}

void count_freq(list *elem_list)
{
    for(int i=0; i<TAM_LINE; i++){
        if(elem_list->line[i] == '\n' || elem_list->line[i] == '\0'){
            break;
        }
        elem_list->vector[elem_list->line[i] - 32].freq++;
    }

    qsort(elem_list->vector, NUM_CHARS, sizeof(charfreq), compare);
}

int main()
{
    list *lines_list = malloc(sizeof(list));
    list *aux = lines_list;
    list *prev;
    char *line = malloc(TAM_LINE * sizeof(char));
    charfreq *vec_freq;
    double time;

    #pragma omp parallel num_threads(T)
    {
        #pragma omp single
        {
            time = omp_get_wtime();
            while (fgets(line, TAM_LINE, stdin))
            {

                aux->line = line;

                vec_freq = malloc(NUM_CHARS * sizeof(charfreq));
                if(vec_freq == NULL){
                    printf("Morri 1\n");
                }

                for (int i = 0; i < NUM_CHARS; i++)
                {
                    vec_freq[i].code = i + 32;
                    vec_freq[i].freq = 0;
                }

                aux->vector = vec_freq;

                #pragma omp task
                {
                    count_freq(aux);
                }

                aux->next = malloc(sizeof(list));
                if(aux->next == NULL){
                    printf("Morri 2\n");
                }

                prev = aux;
                aux = aux->next;

                line = malloc(TAM_LINE * sizeof(char));
                if(line == NULL){
                    printf("Morri 3\n");
                }
            }

            free(line);
            free(prev->next);
            prev->next = NULL;
        }
    }
    time = omp_get_wtime() - time;

    /*
    aux = lines_list;
    while (aux != NULL)
    {
        for (int i = 0; i < NUM_CHARS; i++)
        {
            
            if (aux->vector[i].freq)
            {
                printf("%d %d\n", aux->vector[i].code, aux->vector[i].freq);
            }
        }

        aux = aux->next;

        if(aux){
            printf("\n");
        }
    }
    */
    printf("\n%.10lf\n",time);

    free_memory(lines_list);

    /*
    aux = lines_list;
    while(aux != NULL){
        printf("%s",aux->line);
        aux = aux->next;
    }*/
}