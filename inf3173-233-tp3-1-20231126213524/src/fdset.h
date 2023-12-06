#ifndef INF3173_FILE_SET_H_
#define INF3173_FILE_SET_H_

#include <sys/select.h>

#ifdef __cplusplus
extern "C" {
#endif

// structure pour aider à gérer les ensemble de descripteurs de fichiers
struct fds {
    fd_set set;
    int max_fd;
    int num_fd;
};

void fds_reset(struct fds* s);
void fds_add(struct fds* s, int fd);
int fds_contains(struct fds* s, int fd);
void fds_debug(struct fds* s);

#ifdef __cplusplus
}
#endif

#endif
