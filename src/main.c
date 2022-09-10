#include <stdio.h>
#include <spath/spath.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  const char* directory = "/home/user001/files/";
  const char* filename = "hello.bmp";


  char* joined = spath_join(directory, filename);

  printf("%s\n", joined);


  char bare[256];
  spath_get_barename(joined, bare);

  printf("%s\n", bare);

  free(joined);

  return 0;
}
