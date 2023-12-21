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
    int A[2];
    int B[2];
    pipe(A);
    pipe(B);


    struct fds ensemble_fd;
    fds_reset(&ensemble_fd);
    fds_add(&ensemble_fd,STDIN_FILENO);
    fds_add(&ensemble_fd,B[R]);
    struct timeval timeout;

    int ret = fork();
    if (ret == 0) {
        dup2(A[R],STDIN_FILENO);
        dup2(B[W],STDOUT_FILENO);
        close(A[W]);
        close(B[R]);

        int exec_status=execvp(argv[0],argv);
        if(exec_status==-1){
            perror("execvp");
            exit(1);
        }
    }else{
        close(A[R]);
        close(B[W]);

        int input=open(logfile_input,O_CREAT|O_TRUNC|O_WRONLY,0600);
        int output=open(logfile_output,O_CREAT|O_TRUNC|O_WRONLY,0600);
        while(!fds_contains(&ensemble_fd, STDIN_FILENO) && !fds_contains(&ensemble_fd, B[R])){
            timeout.tv_sec = 0;
            timeout.tv_usec = 500000;
            int pret=select(ensemble_fd.max_fd+1,&ensemble_fd.set,NULL,NULL,&timeout);
            if(pret==-1){
                perror("select()");
            }else if(pret){
                if(fds_contains(&ensemble_fd,STDIN_FILENO)){
                    int bytes;
                    char buf[150];
                    if((bytes = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
                        write(A[W], buf, bytes);
                        write(input, buf, bytes);
                    }else{
                        close(A[W]);
                    }
                }

                if(fds_contains(&ensemble_fd,B[R])){
                    int bytes2;
                    char buf2[150];
                    if((bytes2 = read(B[R], buf2, sizeof(buf2))) > 0) {
                        write(STDOUT_FILENO,buf2,bytes2);
                        write(output, buf2, bytes2);
                    }else{
                        close(B[R]);
                    }


                }
            }
        }
        close(A[W]);
        close(B[R]);
        int status;
        waitpid(ret, &status,0);
        close(input);
        close(output);

    }


    return 0;
}

