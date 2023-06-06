#include <dirent.h>
#include <limits.h>
#include <spath/macros.h>
#include <spath/spath.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  char *left;
  char *right;
} PATHStringSlice;

int spath_string_slice_at(const char *str, uint64_t pos,
                          PATHStringSlice *slice) {
  slice->left = 0;
  slice->right = 0;
  if (!str)
    return 0;
  uint64_t len = strlen(str);
  if (!len)
    return 0;
  pos = pos % len;

  const char *right = &str[pos];
  if (!right)
    return 0;
  uint64_t right_len = strlen(right);
  if (!right_len)
    return 0;
  int64_t left_len = len - right_len;

  if (left_len <= 0)
    return 0;

  char *r = strdup(right);
  char *l = (char *)calloc(left_len + 1, sizeof(char));
  memcpy(&l[0], &str[0], left_len * sizeof(char));

  slice->left = l;
  slice->right = r;

  return 1;
}

void spath_string_slice_clear(PATHStringSlice *slice) {
  if (slice->left)
    free(slice->left);
  if (slice->right)
    free(slice->right);

  slice->left = 0;
  slice->right = 0;
}

int spath_get_barename(const char *path, char *out) {
  if (!path)
    return 0;

  char *ptr = strchr(path, '/');

  if (!ptr) {
    sprintf(out, "%s", path);
    return 1;
  }

  int64_t pos = (ptr - path) + 1;

  PATHStringSlice slice = {0};
  if (!spath_string_slice_at(path, pos, &slice))
    return 0;

  if (!slice.right) {
    spath_string_slice_clear(&slice);
    return 0;
  }

  sprintf(out, "%s", slice.right);

  spath_string_slice_clear(&slice);

  if (strchr(out, '/') != 0) {
    return spath_get_barename(out, out);
  }

  return 1;
}

int spath_get_dirname(const char *path, char *out) {
  if (!path || !out)
    return 0;

  char *ptr = strrchr(path, '/'); // strstr(path, bare);
  if (!ptr)
    return 0;

  int64_t len = ptr - path;
  memcpy(&out[0], &path[0], (len + 1) * sizeof(char));

  out[len] = 0;
  return 1;
}

char *spath_string_append(char **inputstr, const char *other) {
  if (!inputstr || !*inputstr) {
    *inputstr = (char *)calloc(2, sizeof(char));
  }
  char *str = *inputstr;

  uint32_t old_len = strlen(str);
  uint32_t other_len = strlen(other);
  uint32_t new_len = old_len + other_len;

  str = (char *)realloc(str, (new_len + 1) * sizeof(char));
  strcat(str, other);
  *inputstr = str;

  return str;
}

unsigned int spath_string_starts_with_char(const char *str, char c) {
  if (!str)
    return 0;
  return str[0] == c;
}
unsigned int spath_string_ends_with_char(const char *str, char c) {
  if (!str)
    return 0;
  uint32_t len = strlen(str);
  return str[MAX(0, len - 1)] == c;
}

int spath_join_no_alloc(const char *a, const char *b, char *out) {
  if (!a || !b || !out) {
    return 0;
  }

  size_t a_len = strlen(a);
  size_t b_len = strlen(b);

  if (a[a_len - 1] == '/' && b[0] == '/') {
    if (sprintf(out, "%s%s", a, b + 1) < 0)
      return 0;
  } else if (a[a_len - 1] != '/' && b[0] != '/') {
    if (sprintf(out, "%s/%s", a, b) < 0)
      return 0;
  } else {
    if (sprintf(out, "%s%s", a, b) < 0)
      return 0;
  }

  return 1;
}

char *spath_join(const char *a, const char *b) {
  uint32_t len_a = strlen(a);
  uint32_t len_b = strlen(b);
  if (len_a >= PATH_MAX || len_b >= PATH_MAX)
    return 0;
  char buff_a[len_a + 1];
  char buff_b[len_b + 1];
  memcpy(&buff_a[0], a, (len_a + 1) * sizeof(char));
  memcpy(&buff_b[0], b, (len_b + 1) * sizeof(char));

  unsigned int needs_slash = 0;

  if (spath_string_ends_with_char(a, '/') &&
      spath_string_starts_with_char(b, '/')) {
    buff_a[MAX(len_a - 1, 0)] = 0;
  } else if (!spath_string_ends_with_char(a, '/') &&
             !spath_string_starts_with_char(b, '/')) {
    needs_slash = 1;
  }

  char *newstr = 0;
  spath_string_append(&newstr, buff_a);
  if (needs_slash) {
    spath_string_append(&newstr, "/");
  }
  spath_string_append(&newstr, buff_b);

  return newstr;
}

int spath_iter_dir(const char *path, SpathIterDirCallback callback,
                   void *user_ptr, int max_level, int levels) {

  if (levels >= max_level && max_level > -1)
    return 1;

  struct dirent *entry;
  DIR *dp;

  dp = opendir(path);
  if (dp == NULL) {
    fprintf(stderr, "(GLE): iter_dir, could not open %s\n", path);
    return 0;
  }

  while ((entry = readdir(dp))) {
    if (strcmp(entry->d_name, ".") == 0)
      continue;
    if (strcmp(entry->d_name, "..") == 0)
      continue;
    char *next_path = spath_join(path, entry->d_name);
    if (!next_path)
      continue;

    if (entry->d_type == DT_DIR) {
      spath_iter_dir(next_path, callback, user_ptr, max_level, levels + 1);
    } else {
      callback(entry->d_name, next_path, user_ptr);
    }

    free(next_path);
  }

  closedir(dp);
  return 1;
}

int spath_change_extension(const char *path, const char *ext,
                           char out[PATH_MAX]) {
  if (!path || !ext)
    return 0;

  memset(&out[0], 0, PATH_MAX * sizeof(char));

  char *dot = strrchr(path, '.');
  if (!dot)
    return 0;

  int64_t len = strlen(path);
  if (len <= 0)
    return 0;
  int64_t ext_len = strlen(ext);
  if (ext_len <= 0)
    return 0;

  int64_t pos = dot - path;

  memcpy(&out[0], &path[0], pos * sizeof(char));
  memcpy(&out[pos], &ext[0], ext_len * sizeof(char));
  return 1;
}
