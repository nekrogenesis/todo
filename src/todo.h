#ifndef TODO_H
#define TODO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#define STRCMP(s1, s2) (!strcmp(s1, s2))
#define FAIL(fmt, ...) do { fprintf(stderr, fmt, ##__VA_ARGS__); exit(EXIT_FAILURE); } while (0)
#define CHECK_PTR(ptr, fmt, ...) do { if (!ptr) { fprintf(stderr, fmt, ##__VA_ARGS__); exit(EXIT_FAILURE); } } while (0)

typedef struct List{
    size_t size;
    char *line[];
} List;

char *get_path();
List *get_list(const char *path);
void write_list(const char *path, const List *l);
void print_list(const List *list);
void free_list(List *list);

#endif /* TODO_H */