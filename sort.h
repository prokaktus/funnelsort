#ifndef INCLUDES_FUNNELSORT_SORT_H
#define INCLUDES_FUNNELSORT_SORT_H
#include <stddef.h>

enum {FUNNEL_EXHAUSTED = 1, FUNNEL_NOT_EMPTY = 2};

typedef int (*cmp_t)(const void *, const void *);

int compare_longlong(const void *a, const void *b);
int compare_short(const void *a, const void *b);

void
sort(void *ptr, size_t nmemb, size_t size, cmp_t cmp);

typedef struct funnel {
    struct funnel *lr[2];
    void *in;
    void *out;
    size_t size;
    size_t nmemb;
    size_t index;
    cmp_t cmp;
} funnel;

funnel *
funnel_create(void *in, void *out, size_t nmemb, size_t size, cmp_t cmp);

void
funnel_fill(funnel *f);

void funnel_sort(void *base, size_t nmemb, size_t size, cmp_t cmp);

#endif /* INCLUDES_FUNNELSORT_SORT_H */
