/*
 * UQAM INF3173-233-TP1
 *
 * Utilitaire permettant de mesurer le temps d'exécution d'une commande passée
 * en paramètre.
 *
 * Le programme doit répéter l'exécution de la commande le nombre de fois
 * spécifié et enregistrer dans le fichier resultats.dat les résultats de mesure
 * pour chaque exécution, soit:
 *
 *  - Le temps écoulé
 *  - Le temps processeur en mode utilisateur
 *  - Le temps processeur passé dans les appels systèmes
 *
 * Ce fichier sera analysé par l'utilitaire de rapport.
 *
 * Utilisez clock_gettime() et getrusage() pour obtenir les statistiques du
 * processus enfant.
 */
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interceptor.h"

void print_usage() {
    fprintf(stderr,
            "Usage: interceptor [-i LOG_INPUT] [-o LOG_OUTPUT] commande "
            "[arguments]");
    exit(1);
}

int main(int argc, char** argv) {
    struct option options[] = {
        {"stdin", 1, 0, 'i'},   //
        {"stdout", 1, 0, 'o'},  //
        {"help", 0, 0, 'h'},    //
        {0, 0, 0, 0}            //
    };

    const char* logfile_input = "input.bin";
    const char* logfile_output = "output.bin";

    int opt;
    int idx;
    while ((opt = getopt_long(argc, argv, "i:o:h", options, &idx)) != -1) {
        fprintf(stderr, "opt=%d optarg=%s\n", opt, optarg);
        switch (opt) {
            case 'i':
                logfile_input = optarg;
                break;
            case 'o':
                logfile_output = optarg;
                break;
            case 'h':
            default:
                print_usage();
        }
    }

    // Sous-commande
    if (optind >= argc) {
        fprintf(stderr, "Error: missing subcommand -- ");
        return EXIT_FAILURE;
    }

    // Affichage des options
    {
        fprintf(stderr, "options:\n");
        fprintf(stderr, " input: %s\n", logfile_input);
        fprintf(stderr, " output: %s\n", logfile_output);
        fprintf(stderr, " optind: %d\n", optind);
        fprintf(stderr, " argv  : ");
        for (int i = optind; i < argc; i++) {
            fprintf(stderr, "%s ", argv[i]);
        }
        fprintf(stderr, "\n");
    }

    return interceptor_execute(logfile_input, logfile_output, argv + optind);
}
