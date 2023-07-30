#include <linux/limits.h>
#include <stdio.h>
#include <spath/spath.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]) {

  const char* some_path = "/home/johndoe/Documents/secret/tmp/xyz/123/Pictures/dog.jpg";

  SPathIterator it = spath_iterator_init(some_path);

  char slice[PATH_MAX] = {0};
  while (spath_iterator_next(&it, slice, PATH_MAX-1)) {
    printf("%s\n", slice);
  }

  return 0;
}
