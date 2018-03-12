#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>

#include IMPL

#ifdef AVL_OPT
#define OUT_FILE "avl_opt.txt"
#elif HASH_AVL_OPT
#define OUT_FILE "hash_avl_opt.txt"
#else
#define OUT_FILE "orig.txt"
#endif

#define DICT_FILE "./dictionary/words.txt"

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main(int argc, char *argv[])
{
    FILE *fp;
    int i = 0;
    char line[MAX_LAST_NAME_SIZE];
    struct timespec start, end;
    double cpu_time1, cpu_time2;

    /* check file opening */
    fp = fopen(DICT_FILE, "r");
    if (fp == NULL) {
        printf("cannot open the file\n");
        return -1;
    }
#ifdef AVL_OPT
    tree *t = create_avl();
    printf("size of entry : %lu bytes\n", sizeof(node));
#elif HASH_AVL_OPT
    tree *table[HASH_TAB_SIZE];
    init_tab(table);
    printf("size of entry : %lu bytes\n", sizeof(node));
#else
    /* build the entry */
    entry *pHead, *e;
    pHead = (entry *) malloc(sizeof(entry));
    printf("size of entry : %lu bytes\n", sizeof(entry));
    e = pHead;
    e->pNext = NULL;
#endif

#if defined(__GNUC__)
#ifdef AVL_OPT
    __builtin___clear_cache((char *) t, (char *) t + sizeof(node));
#elif HASH_AVL_OPT
    __builtin___clear_cache((char *) table, (char *) table + sizeof(node));
#else
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#endif

#ifdef AVL_OPT
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        append(line, t);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";

    assert(findName(input, t->root) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, t->root)->lastName, "zyxel"));
#elif HASH_AVL_OPT
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        append(line, table);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";

    assert(findName(input, table) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, table)->lastName, "zyxel"));
#else
    clock_gettime(CLOCK_REALTIME, &start);
    while (fgets(line, sizeof(line), fp)) {
        while (line[i] != '\0')
            i++;
        line[i - 1] = '\0';
        i = 0;
        e = append(line, e);
    }
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time1 = diff_in_second(start, end);

    /* close file as soon as possible */
    fclose(fp);

    e = pHead;

    /* the givn last name to find */
    char input[MAX_LAST_NAME_SIZE] = "zyxel";
    e = pHead;

    assert(findName(input, e) &&
           "Did you implement findName() in " IMPL "?");
    assert(0 == strcmp(findName(input, e)->lastName, "zyxel"));
#endif

#if defined(__GNUC__)
#ifdef AVL_OPT
    __builtin___clear_cache((char *) t, (char *) t + sizeof(node));
#elif HASH_AVL_OPT
    __builtin___clear_cache((char *) table, (char *) table + sizeof(node));
#else
    __builtin___clear_cache((char *) pHead, (char *) pHead + sizeof(entry));
#endif
#endif

#ifdef AVL_OPT
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    findName(input, t->root);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);
    drop_tree(t);
#elif HASH_AVL_OPT
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    findName(input, table);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);
    drop_tab(table);
#else
    /* compute the execution time */
    clock_gettime(CLOCK_REALTIME, &start);
    findName(input, e);
    clock_gettime(CLOCK_REALTIME, &end);
    cpu_time2 = diff_in_second(start, end);

    FILE *output = fopen(OUT_FILE, "a");
    fprintf(output, "append() findName() %lf %lf\n", cpu_time1, cpu_time2);
    fclose(output);

    printf("execution time of append() : %lf sec\n", cpu_time1);
    printf("execution time of findName() : %lf sec\n", cpu_time2);

    if (pHead->pNext) free(pHead->pNext);
    free(pHead);
#endif
    return 0;
}
