#define _GNU_SOURCE
#include "utils.h"

#include <float.h>
#include <limits.h>
#include <math.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

// Source:
// https://stackoverflow.com/questions/4553012/checking-if-a-file-is-a-directory-or-just-a-file
int is_regular_file(const char* path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return 0;
    }
    return S_ISREG(path_stat.st_mode);
}

int is_dir(const char* path) {
    struct stat path_stat;
    if (stat(path, &path_stat) != 0) {
        return 0;
    }
    return S_ISDIR(path_stat.st_mode);
}

// Source:
// https://stackoverflow.com/questions/744766/how-to-compare-ends-of-strings-in-c
int ends_with(const char* str, const char* suffix) {
    size_t str_len = strlen(str);
    size_t suffix_len = strlen(suffix);

    return (str_len >= suffix_len)
           && (!memcmp(str + str_len - suffix_len, suffix, suffix_len));
}

/*
 * Soustraire deux structure timespec: a - b
 * Retourne le résultat en nanoseconde
 */
uint64_t timespec_sub_ns(struct timespec* a, struct timespec* b) {
    uint64_t sec = a->tv_sec - b->tv_sec;
    uint64_t nsec = a->tv_nsec - b->tv_nsec;
    if (nsec < 0) {
        sec--;
        nsec += 1000000000UL;
    }
    return sec * 1000000000UL + nsec;
}

/*
 * Soustraire deux structure timeval: a - b
 * Retourne le résultat en nanoseconde (et non en microseconde)
 */
uint64_t timeval_sub_ns(struct timeval* a, struct timeval* b) {
    uint64_t sec = a->tv_sec - b->tv_sec;
    uint64_t nsec = (a->tv_usec - b->tv_usec) * 1000UL;
    if (nsec < 0) {
        sec--;
        nsec += 1000000000UL;
    }
    return sec * 1000000000UL + nsec;
}

char* human_readable_time(double time_ns) {
    static char buf[NAME_MAX];
    char* unit;
    double value;

    double time = time_ns / 1E9;

    if (time < 1e-6) {
        unit = "ns";
        value = time * 1e9;
    } else if (time < 1e-3) {
        unit = "us";
        value = time * 1e6;
    } else if (time < 1) {
        unit = "ms";
        value = time * 1e3;
    } else if (time < 60) {
        unit = "s";
        value = time;
    } else if (time < 3600) {
        unit = "m";
        value = time / 60;
    } else {
        unit = "h";
        value = time / 3600;
    }

    snprintf(buf, NAME_MAX, "%.2f %s", value, unit);
    return buf;
}

char** make_split(const char* str, const char* separator, int* count) {
    int i, num_splits = 0;
    char* token = NULL;
    char* str_copy = NULL;
    char* tmp = NULL;
    char** result = NULL;

    // Créer une copie pour strtok
    str_copy = tmp = strdup(str);
    if (!str_copy) {
        return NULL;
    }

    // Compter le nombre d'items
    int seplen = strlen(separator);
    while ((token = strstr(tmp, separator)) != NULL) {
        num_splits++;
        tmp = token + seplen;
    }

    // Allouer le tableau de pointeurs (terminé par NULL)
    result = malloc((num_splits + 2) * sizeof(char*));
    if (!result) {
        goto out_tmp;
    }

    tmp = str_copy;  // recommencer
    i = 0;

    // Extraire les sous-chaines
    while ((token = strstr(tmp, separator)) != NULL) {
        *token = '\0';  // Terminer la chaine à la place du séparateur
        result[i++] = strdup(tmp);
        tmp = token + seplen;
    }

    // Dernière partie
    result[i++] = strdup(tmp);
    result[i] = NULL;  // Terminer par zéro

out_tmp:
    free(str_copy);

    *count = num_splits + 1;  // set the count (number of strings)
    return result;
}

void free_split(char** result) {
    char** temp = result;
    while (*temp) {
        free(*temp);
        temp++;
    }
    free(result);
}
