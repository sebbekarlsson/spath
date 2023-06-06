#include <linux/limits.h>
#include <stdio.h>
#include <spath/spath.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {

  char tmp[PATH_MAX];


  memset(&tmp[0], 0, PATH_MAX*sizeof(char));
  spath_join_no_alloc("/home/ianertson", "image.png", tmp);
  printf("%s\n", tmp);

  memset(&tmp[0], 0, PATH_MAX*sizeof(char));
  spath_join_no_alloc("/home/ianertson/", "image.png", tmp);
  printf("%s\n", tmp);

  memset(&tmp[0], 0, PATH_MAX*sizeof(char));
  spath_join_no_alloc("/home/ianertson/", "/image.png", tmp);
  printf("%s\n", tmp);

  return 0;
}
