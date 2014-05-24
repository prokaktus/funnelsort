#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

int main(int argc, char *argv[])
{
        if (argc < 4) {
            fprintf(stderr, "There are %d parameters. Need more than 3.\n", argc - 1);
            fprintf(stderr, "1: number elements.\n2: size each element.\n3: file with data that need to be sorted.\n"
                            "4 (optional): log file with sorted data\n");
            return 0;
        }
        FILE *fp;
        FILE *fwp = NULL;
        fp = fopen(argv[3], "rb");
        if (!fp) {
            fprintf(stderr, "Cannot open file");
            return 0;
        }
        long long count = atoi(argv[1]);
        size_t size = atoi(argv[2]);
        if (argc > 4) {
            fwp = fopen(argv[4], "wb");
            if (!fwp) {
                fprintf(stderr, "Cannot open file for logging\n");
            }
        }
        unsigned short *p = (unsigned short *) calloc(count, size);
        unsigned short *p_qsort = (unsigned short *) calloc(count, size);

        fread(p, size, count, fp);

        /*int i = 0;
        
        for (i = 0; i < count; ++i) {
            printf("%hu ", p[i]);
            p_qsort[i] = p[i];
        }
        printf("\n============\n");*/
        funnel_sort(p, count, size, compare_short);
        qsort(p_qsort, count, size, compare_short);
        int correct = 1;
        /*for (i = 0; i < count; ++i) {
            // %hd only for testing.
            printf("%hu ", p[i]);
            if (p_qsort[i] != p[i]) {
                correct = 0;
            }
        }*/
        if (fwp) {
            fwrite(p, size, count, fwp);
            fclose(fwp);
        }
        
        fprintf(stdout, "\nF: %p,c: %lld, s: %zu\n", fp, count, size); 
        if (correct) {
            fprintf(stdout, "Exactly like qsort\n");
        } else {
            fprintf(stdout, "Wrong sort\n");
        }
        free(p);
        free(p_qsort);
        fclose(fp);
	return 0;
}
