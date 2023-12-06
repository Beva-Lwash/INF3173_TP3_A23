#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*
 * Utilitaire de test qui recopie de l'entrée standard vers la sortie standard
 * avec un délai.
 *
 * Ceci a pour but de tester si un interblocage peut survenir avec un
 * consommateur lent.
 */

#define BUF_SIZE 4096

int main(int argc, char** argv) {
    sleep(1);
    char buf[BUF_SIZE];
    while (1) {
        int rd = read(STDIN_FILENO, buf, BUF_SIZE);
        if (rd <= 0) break;

        int wr = write(STDOUT_FILENO, buf, rd);
        if (wr < rd) break;
    }
    return 0;
}
