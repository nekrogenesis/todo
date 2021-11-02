#ifndef TODO_H
#define TODO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define STRCMP(s1, s2) (!strcmp(s1, s2))

#ifndef COLOR
#define FAIL(fmt, ...) do { fprintf(stderr, fmt, ##__VA_ARGS__); exit(EXIT_FAILURE); } while (0)
#define CHECK_PTR(ptr, fmt, ...) do { if (!ptr) { fprintf(stderr, fmt, ##__VA_ARGS__); exit(EXIT_FAILURE); } } while (0)
#define EPRINTF(fmt, ...) fprintf(stderr, fmt, ##__VA_ARGS__)
#else
#define FAIL(fmt, ...) do { fprintf(stderr, "\033[31m" fmt, ##__VA_ARGS__); exit(EXIT_FAILURE); } while (0)
#define CHECK_PTR(ptr, fmt, ...) do { if (!ptr) { fprintf(stderr, "\033[31m" fmt, ##__VA_ARGS__); exit(EXIT_FAILURE); } } while (0)
#define EPRINTF(fmt, ...) fprintf(stderr, "\033[31m" fmt, ##__VA_ARGS__)
#endif /* COLOR */


typedef struct List{
    size_t size;
    char *line[];
} List;

char *get_path();
List *get_list(const char *path);
void write_list(const char *path, const List *l);
void print_list(const List *list);
void free_list(List *list);
void reset_term_color();

#endif /* TODO_H */
