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

void free_memory(list *aux)
{
    if (aux->next)
    {
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

void count_freq(list *elem)
{
    for (int i = 0; i < TAM_LINE; i++)
    {
        if (elem->line[i] == '\n' || elem->line[i] == '\0')
        {
            break;
        }
        elem->vector[elem->line[i] - 32].freq++;
    }

    qsort(elem->vector, NUM_CHARS, sizeof(charfreq), compare);
}

int main()
{
    list *prev;
    charfreq *vec_freq;
    list *lines_list = (list *)malloc(sizeof(list));
    if (lines_list == NULL)
    {
        printf("Falha na alocacao de memoria\n");
        return 1;
    }
    list *aux = lines_list;

    char *line = (char *)malloc(TAM_LINE * sizeof(char));
    if (line == NULL)
    {
        printf("Falha na alocacao de memoria\n");
        return 1;
    }

    
    //double time;

    #pragma omp parallel num_threads(T)
    {
        #pragma omp single
        {
            //time = omp_get_wtime();
            while (fgets(line, TAM_LINE, stdin))
            {

                aux->line = line;

                vec_freq = (charfreq *)malloc(NUM_CHARS * sizeof(charfreq));
                if (vec_freq == NULL)
                {
                    printf("Falha na alocacao de memoria\n");
                    return 1;
                }

                for (int i = 0; i < NUM_CHARS; i++)
                {
                    vec_freq[i].code = i + 32;
                    vec_freq[i].freq = 0;
                }

                aux->vector = vec_freq;

                #pragma omp task firstprivate(aux)
                {
                    count_freq(aux);
                }

                aux->next = (list *)malloc(sizeof(list));
                if (aux->next == NULL)
                {
                    printf("Falha na alocacao de memoria\n");
                    return 1;
                }

                prev = aux;
                aux = aux->next;

                line = (char *)malloc(TAM_LINE * sizeof(char));
                if (line == NULL)
                {
                    printf("Falha na alocacao de memoria\n");
                    return 1;
                }
            }

            free(line);
            free(prev->next);
            prev->next = NULL;
        }
    }

    //time = omp_get_wtime() - time;

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

        if (aux)
        {
            printf("\n");
        }
    }

    //Print para verificar tempo de execução
    //printf("\n%.10lf\n", time);

    free_memory(lines_list);
}