#define _GNU_SOURCE
#include "interceptor.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "fdset.h"
#include "utils.h"

// macros utiles pour se souvenir que read=0 et write=1
#define R 0
#define W 1

int interceptor_execute(const char* logfile_input, const char* logfile_output,
                        char** argv) {
    int tubes[2];
    int A[2];
    int B[2];
    pipe(A);
    pipe(B);

    // Conserver la référence vers le tube en écriture
    tubes[i] = fd[W];
    fds ensemble_fd;
    fds_reset(ensemble_fd);
    fds_add(ensemble_fd,A[R]);
    fds_add(ensemble_fd,A[W]);
    fds_add(ensemble_fd,B[R]);
    fds_add(ensemble_fd,B[W]);

    int ret = fork();
    if (ret == 0) {
        // Remplacer le descripteur stdout par log
        dup2(A[R],STDIN_FILENO);

        // Remplacer l'entrée standard par le tube
        dup2(B[W],STDOUT_FILENO);

        // Fermer la portion écriture du tube (on fait juste lire)
        close(A[W]);
        close(B[R]);
        // Exécution d'un sous-programme (juste un exemple)
        int exec_status=execvp(argv[0],argv);

        // une erreur est survenue, quitter
        if(exec_status==-1){
            perror("execvp");
            exit(1);
        }
    }else{
        close(A[R]);
        close(B[W]);

        int input=open(logfile_input,O_CREAT|O_TRUNC,0664);
        int output=open(logfile_output,O_CREAT|O_TRUNC,0664);

        int bytes;
        char buf[128];
        while ((bytes = read(STDIN_FILENO, buf, 128)) > 0) {
                write(A[W], buf, bytes);
                write(input, buf, bytes);
        }
        int bytes2;
        char buf2[128];
        while ((bytes2 = read(B[R], buf2, 128)) > 0) {
                write(A[W], buf, bytes);
                write(output, buf, bytes);
        }
    }

    // Fermer la portion lecture du tube (on fait juste écrire)
    close(fd[R]);


    // Indiquer que l'écriture est terminée
    for (int i = 0; i < 2; i++) {
        close(tubes[i]);
    }

    // Attendre la fin des enfants
    for (int i = 0; i < 2; i++) {
        wait(NULL);
    }



    return 1;
}

