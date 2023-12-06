#include "fdset.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "utils.h"

void fds_reset(struct fds* s) {
    FD_ZERO(&s->set);
    s->max_fd = -1;
    s->num_fd = 0;
}

void fds_add(struct fds* s, int fd) {
    // on n'ajoute pas un descripteur invalide ou fermé
    if (fd < 0 || fcntl(fd, F_GETFD) < 0) {
        return;
    }
    FD_SET(fd, &s->set);
    s->max_fd = MAX(s->max_fd, fd + 1);
    s->num_fd++;
}

int fds_contains(struct fds* s, int fd) { return FD_ISSET(fd, &s->set); }

void fds_debug(struct fds* s) {
    fprintf(stderr, "num_fd: %d max_fd: %d\n", s->num_fd, s->max_fd);
    fprintf(stderr, "fds  : ");
    for (int i = 0; i < s->max_fd; i++) {
        fprintf(stderr, "%2d ", i);
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "isset: ");
    for (int i = 0; i < s->max_fd; i++) {
        fprintf(stderr, "%2d ", FD_ISSET(i, &s->set));
    }
    fprintf(stderr, "\n");
}
