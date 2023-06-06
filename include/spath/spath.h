#ifndef SPATH_H
#define SPATH_H
#include <limits.h>
char *spath_join(const char *a, const char *b);
int spath_join_no_alloc(const char *a, const char *b, char *out);


int spath_get_barename(const char *path, char *out);

typedef void (*SpathIterDirCallback)(const char *name, const char *path,
                                     void *user_ptr);


int spath_iter_dir(const char *path, SpathIterDirCallback callback,
                   void *user_ptr, int max_level, int levels);

int spath_get_dirname(const char *path, char *out);

int spath_change_extension(const char* path, const char* ext, char out[PATH_MAX]);

#endif
