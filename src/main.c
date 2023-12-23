#include <linux/limits.h>
#include <stdio.h>
#include <spath/spath.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char* argv[]) {

  const char* some_path = "/home/johndoe/Documents/secret/tmp/xyz/123/Pictures/dog.jpg";

  char ext[256] = {0};

  spath_get_extension(some_path, ext, 256);

  printf("ext(%s)\n", ext);

  return 0;
}
