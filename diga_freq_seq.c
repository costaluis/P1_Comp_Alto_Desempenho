#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

struct CharFreq {
    int code, freq;
};

int compare(const void *p1, const void *p2)
{
    struct CharFreq *cf1 = (struct CharFreq *) p1;
    struct CharFreq *cf2 = (struct CharFreq *) p2;

    if (cf1->freq != cf2->freq)
        return cf1->freq - cf2->freq;

    return cf2->code - cf1->code;
}

int main()
{
    double time;
    char line[1002];
    struct CharFreq charFreq[96];

    time = omp_get_wtime();
    while (fgets(line, 1002, stdin)) {
        char *ptr;
        int i;

        for (i = 0; i < 96; ++i) {
            charFreq[i].code = i + 32;
            charFreq[i].freq = 0;
        }

        for (ptr = line; *ptr != '\n'; ++ptr)
            ++charFreq[*ptr - 32].freq;

        qsort(charFreq, 96, sizeof(struct CharFreq), compare);

        /*
        for (i = 0; i < 96; ++i) {
            if (charFreq[i].freq > 0)
                printf("%d %d\n", charFreq[i].code, charFreq[i].freq);
        }*/

    }
    time = omp_get_wtime()-time;

    printf("\n%.10lf\n",time);
    return 0;
}