#ifndef INF3173_MESURE_H_
#define INF3173_MESURE_H_

#ifdef __cplusplus
extern "C" {
#endif

int interceptor_execute(const char* logfile_input, const char* logfile_output,
                        char** argv);

#ifdef __cplusplus
}
#endif

#endif
