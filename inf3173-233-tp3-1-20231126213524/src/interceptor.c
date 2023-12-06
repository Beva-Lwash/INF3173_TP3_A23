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
    return 1;
}
