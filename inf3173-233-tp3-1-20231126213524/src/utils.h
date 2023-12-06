#ifndef INF3173_UTILS_H_
#define INF3173_UTILS_H_

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

struct timespec;
struct timeval;

int is_regular_file(const char* path);
int is_dir(const char* path);
int ends_with(const char* str, const char* suffix);
uint64_t timespec_sub_ns(struct timespec* a, struct timespec* b);
uint64_t timeval_sub_ns(struct timeval* a, struct timeval* b);
char* human_readable_time(double time);
char** make_split(const char* str, const char* separator, int* count);
void free_split(char** result);

#ifdef __cplusplus
}
#endif

#endif
