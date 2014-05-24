#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "sort.h"
#define CACHELINE_SIZE 64

int compare_longlong(const void *a, const void *b) {
    return *(const unsigned long long *) a < *(const unsigned long long *) b ? -1 : 1;
}

int compare_short(const void *a, const void *b) {
    return *(const unsigned short *) a < *(const unsigned short *) b ? -1 : 1;
}

funnel *
funnel_create(void *in, void *out, size_t nmemb, size_t size, cmp_t cmp) {
    funnel *f = (funnel *)malloc(sizeof(funnel));

    f->out = out;
    f->nmemb = nmemb;
    f->size = size;
    f->cmp = cmp;
    f->in = in;
    f->index = 0;

    if ((f->nmemb) * size > CACHELINE_SIZE) {
        // fprintf(stdout, "Branches\n");
        size_t nmemb_left = nmemb / 2;
        size_t nmemb_right = nmemb - nmemb_left;
        void *out_left = malloc(nmemb_left * f->size);
        void *out_right = malloc(nmemb_right * f->size);        
        funnel_create(in, out_left, nmemb_left, f->size, cmp);
        funnel_create((char *) in + nmemb_left * size, out_right, 
                      nmemb_right, f->size, cmp);
    } else {
        // fprintf(stdout, "Feel the bottom\n");
        f->lr[0] = f->lr[1] = NULL;
    }
    return f;
}

void *
funnel_first(funnel *f) {
    if (f->index < f->nmemb) {
        return (void *) ((char *) f->in + f->size * f->index);
    }
    return NULL;  
}

int
funnel_pop(funnel *f) {
    f->index++;
    if (f->index >= f->nmemb) {
        // funnel_fill(f);
        return FUNNEL_EXHAUSTED;
    }
    return FUNNEL_NOT_EMPTY;
}
void
funnel_fill(funnel *f) {
    //size_t i1 = 0, i2 = 0;

    if (f->lr[0] == NULL || f->lr[1] == NULL) {
        qsort(f->in, f->nmemb, f->size, f->cmp);
        memcpy(f->out, f->in, f->nmemb * f->size);
        //free(f->in);
        return;
    }
    funnel_fill(f->lr[0]);
    funnel_fill(f->lr[1]);
    // perform merge
    size_t i1 = f->lr[0]->index, i2 = f->lr[1]->index;
    size_t n1 = f->lr[0]->nmemb, n2 = f->lr[1]->nmemb;
    void *e1, *e2;
    size_t index = 0;

    while (i1 < n1 && i2 < n2) {
        e1 = funnel_first(f->lr[0]);
        e2 = funnel_first(f->lr[1]);

        if (f->cmp(e1, e2)) {
            memcpy((char *)f->out + index * f->size, e1, f->size);
            if (funnel_pop(f->lr[0]) == FUNNEL_EXHAUSTED) {
                //free(f->lr[0]->out);
            }
            i1++;        
        } else {
            memcpy((char *)f->out + index * f->size, e2, f->size);
            if (funnel_pop(f->lr[1]) == FUNNEL_EXHAUSTED) {
                //free(f->lr[1]->out);
            }
            i2++;
        }
        index++;
    }
    while (i1 < n1) {
        e1 = funnel_first(f->lr[0]);
        memcpy((char *)f->out + index * f->size, e1, f->size);
        if (funnel_pop(f->lr[0]) == FUNNEL_EXHAUSTED) {
            //free
        }
        i1++;
        index++;
    }
    while (i2 < n2) {
        e1 = funnel_first(f->lr[1]);
        memcpy((char *)f->out + index * f->size, e2, f->size);
        if (funnel_pop(f->lr[1]) == FUNNEL_EXHAUSTED) {
            //free
        }
        i2++;
        index++;
    }

    return; 
}


void funnel_sort(void *base, size_t nmemb, size_t size, cmp_t cmp) {
    void *out = malloc(size * nmemb);
    funnel *funn = funnel_create(base, out, nmemb, size, cmp);
    //
    if (!funn) {
        fprintf(stdout, "Error while creation funnel\n");
        return;
    }
    funnel_fill(funn);
    memcpy(out, base, size * nmemb);
}
