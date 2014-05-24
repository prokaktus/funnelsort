#include <stdio.h>
#include <stdlib.h>
#include "sort.h"

int main(int argc, char *argv[])
{
        if (argc != 4) {
            fprintf(stderr, "There %d parameters. Need exactly 3.\n", argc - 1);
            return 0;
        }
        FILE *fp;
        fp = fopen(argv[3], "rb");
        if (!fp) {
            fprintf(stderr, "Cannot open file");
            return 0;
        }
        long long count = atoi(argv[1]);
        size_t size = atoi(argv[2]);

        int i = 0;
        for (i = 0; i < count; ++i) {
            void *p = calloc(1, size);
            fread(p, size, 1, fp);
            int *hex = (int *)p;
            fprintf(stdout, "el: %x", *hex);
        }
        
        printf("\nsizeof %lu\n", sizeof(short));
        short a[] = {5, 8, 1, 4, -1, 12};
        qsort(a, 6, sizeof(short), compare_short);
        for (i = 0; i < 6; ++i) {
            printf("%hd ", a[i]);
        }        

        fprintf(stdout, "F: %p,c: %lld, s: %zu\n", fp, count, size); 
	return 0;
}
