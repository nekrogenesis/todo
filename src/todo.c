#include "todo.h"

int main(int argc, char **argv) {
    #ifdef COLOR
        atexit(reset_term_color);
    #endif

    char *path = get_path();
    /* init */
    if (argc > 1 && (STRCMP(argv[1], "init") || STRCMP(argv[1], "i"))) {
        FILE *fp = fopen(path, "a");
        CHECK_PTR(fp, "could not create: %s\n", path);
        fclose(fp);
        exit(EXIT_SUCCESS);
    }
    List *list = get_list(path);
    if (argc == 1) {
        if (list->size == 0) {
            printf("empty\n");
            return EXIT_SUCCESS;
        }
        print_list(list);
    } else {
        /* add <string> ... */
        if (STRCMP(argv[1], "add") || STRCMP(argv[1], "a")) {
            if (argc < 3)
                FAIL("too few arguments\n");
            for (int i = 2; i < argc; i++) {
                list->size++;
                list = realloc(list, sizeof(List) + sizeof(char *) * list->size);
                CHECK_PTR(list, "failed to allocate memory\n");
                list->line[list->size - 1] = malloc(strlen(argv[i]) + 3);
                CHECK_PTR(list->line[list->size - 1], "failed to allocate memory\n");
                list->line[list->size - 1][0] = ' ';
                strcpy(&list->line[list->size - 1][1], argv[i]);
                list->line[list->size - 1][strlen(argv[i]) + 1] = '\n';
                list->line[list->size - 1][strlen(argv[i]) + 2] = '\0';
            }
        }

        /* check <entry> ... */
        else if (STRCMP(argv[1], "check") || STRCMP(argv[1], "c")) {
            if (argc < 3)
                FAIL("too few arguments\n");
            size_t entry;
            for (int i = 2; i < argc; i++) {
                if (sscanf(argv[i], "%zu", &entry) != 1) {
                    EPRINTF("invalid argument: %s\n", argv[i]);
                    continue;
                }
                if ((entry > list->size) || entry <= 0) {
                    EPRINTF("invalid index: %li\n", entry);
                    continue;
                }
                list->line[entry - 1][0] = 'x';
            }
        }

        /* uncheck <entry> ... */
        else if (STRCMP(argv[1], "uncheck") || STRCMP(argv[1], "u")) {
            if (argc < 3)
                FAIL("too few arguments\n");
            size_t entry;
            for (int i = 2; i < argc; i++) {
                if (sscanf(argv[i], "%zu", &entry) != 1) {
                    EPRINTF("invalid argument: %s\n", argv[i]);
                    continue;
                }
                if ((entry > list->size) || entry <= 0) {
                    EPRINTF("invalid index: %li\n", entry);
                    continue;
                }
                list->line[entry - 1][0] = ' ';
            }
        }

        /* remove <entry> ... */
        else if (STRCMP(argv[1], "remove") || STRCMP(argv[1], "r")) {
            if (argc < 3)
                FAIL("too few arguments\n");
            size_t entry;
            for (int i = 2; i < argc; i++) {
                if (sscanf(argv[i], "%zu", &entry) != 1) {
                    EPRINTF("invalid argument: %s\n", argv[i]);
                    continue;
                }
                if ((entry > list->size) || entry <= 0) {
                    EPRINTF("invalid index: %li\n", entry);
                    continue;
                }
                list->line[entry - 1][0] = '\0';
            }
        }

        /* delete */
        else if (STRCMP(argv[1], "delete") || STRCMP(argv[1], "d")) {
            for (size_t i = 0; i < list->size; i++)
                if (list->line[i][0] == 'x')
                    list->line[i][0] = '\0';
        }

        /* edit <entry> <string> */
        else if (STRCMP(argv[1], "edit") || STRCMP(argv[1], "e")) {
            if (argc < 4)
                FAIL("too few arguments\n");
            size_t entry;
            if (sscanf(argv[2], "%zu", &entry) != 1)
                FAIL("invalid argument: %s\n", argv[2]);
            if ((entry > list->size) || entry <= 0)
                FAIL("invalid index: %li\n", entry);
            list->line[entry - 1] = realloc(list->line[entry - 1], strlen(argv[3]) + 3);
            CHECK_PTR(list->line[entry - 1], "failed to allocate memory\n");
            strcpy(&list->line[entry - 1][1], argv[3]);
            list->line[entry - 1][0] = ' ';
            list->line[entry - 1][strlen(argv[3]) + 1] = '\n';
            list->line[entry - 1][strlen(argv[3]) + 2] = '\0';
        }

        /* move <entry> <entry> */
        else if (STRCMP(argv[1], "move") || STRCMP(argv[1], "m")) {
            if (argc < 4)
                FAIL("too few arguments\n");
            ssize_t from;
            ssize_t to;
            if (sscanf(argv[2], "%zi", &from) != 1)
                FAIL("invalid argument: %s\n", argv[2]);
            if ((from > list->size) || from <= 0)
                FAIL("invalid index: %li\n", from);
            if (sscanf(argv[3], "%zi", &to) != 1)
                FAIL("invalid argument: %s\n", argv[3]);
            if ((to > list->size) || to <= 0)
                FAIL("invalid index: %li\n", to);
            from--;
            to--;
            char *tmp = list->line[from];
            ssize_t step = to > from ? 1 : -1;
            for (ssize_t e = from; e != to; e += step)
                list->line[e] = list->line[e + step];
            list->line[to] = tmp;
        }

        /* help */
        else if (STRCMP(argv[1], "help") || STRCMP(argv[1], "h")) {
            printf(
                "   i, init\n"\
                "   a, add <string> ...\n"\
                "   c, check <entry> ... \n"\
                "   u, uncheck <entry> ...\n"\
                "   r, remove <entry> ...\n"\
                "   d, delete \n"\
                "   e, edit <entry> <string> \n"\
                "   m, move <from entry> <to entry> \n"
            );
        }

        else {
            EPRINTF("unknown command\n");
        }

    }
    write_list(path, list);
    free_list(list);
    free(path);
    return EXIT_SUCCESS;
}

char *get_path() {
    char *path = strdup(getenv("HOME"));
    CHECK_PTR(path, "failed to allocate memory\n");
    path = realloc(path, strlen(path) + 11);
    CHECK_PTR(path, "failed to allocate memory\n");
    strcat(path, "/.todofile");
    return path;
}

List *get_list(const char *path) {
    FILE *file = fopen(path, "r");
    CHECK_PTR(file, "file: %s not found\n", path);
    size_t allocd = 32;
    List *list = calloc(1, sizeof(List) + sizeof(char *) * allocd);
    CHECK_PTR(list, "failed to allocate memory\n");
    char *buf = NULL;
    size_t buf_size = 0;
    ssize_t read = getline(&buf, &buf_size, file);
    while (read >= 0) {
        if (strlen(buf) == 1) {
            read = getline(&buf, &buf_size, file);
            continue;
        }
        list->line[list->size] = malloc(sizeof(char) * strlen(buf) + 1);
        CHECK_PTR(list->line[list->size], "failed to allocate memory\n");
        strcpy(list->line[list->size], buf);
        list->size++;
        read = getline(&buf, &buf_size, file);
        if (list->size == allocd) {
            allocd += allocd;
            list = realloc(list, sizeof(List) + sizeof(char *) * allocd);
            CHECK_PTR(list, "failed to allocate memory\n");
        }
    }
    fclose(file);
    free(buf);
    return list;
}

void write_list(const char *path, const List *list) {
    FILE *file = fopen(path, "w");
    CHECK_PTR(file, "failed to write to: %s\n", path);
    for (size_t i = 0; i < list->size; i++) {
        if (list->line[i][0] == '\0')
            continue;
        if (fprintf(file, "%s", list->line[i]) != strlen(list->line[i]))
            FAIL("failed to write to: %s\n", path);
    }
    fclose(file);
}

void print_list(const List *list) {
    for (size_t i = 0; i < list->size; i++) {
        #ifndef COLOR
            printf("%-2li %s %s", 
                i + 1, 
                list->line[i][0] == 'x' ? "[x]" : "[ ]", 
                &list->line[i][1]
            );
        #else
            printf("%-2li %s %s", 
                i + 1, 
                list->line[i][0] == 'x' ? "\033[1;32m[X]\033[0m" : "\033[1;31m[ ]\033[0m", 
                &list->line[i][1]
            );
        #endif /* COLOR */

    }
}

void free_list(List *list) {
    for (size_t i = 0; i < list->size; i++)
        free(list->line[i]);
    free(list);
}

void reset_term_color() {
    printf("\033[0m");
}
