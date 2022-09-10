#ifndef SPATH_H
#define SPATH_H
char *spath_join(const char *a, const char *b);

int spath_get_barename(const char *path, char *out);

typedef void (*SpathIterDirCallback)(const char *name, const char *path,
                                     void *user_ptr);


int spath_iter_dir(const char *path, SpathIterDirCallback callback,
                   void *user_ptr, int max_level, int levels);
#endif
