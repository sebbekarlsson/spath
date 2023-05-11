#include <linux/limits.h>
#include <stdio.h>
#include <spath/spath.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {

  char buff[PATH_MAX];
  if (spath_change_extension("/some/directory/image.jpg/hello.php", ".txt", buff)) {
    printf("%s\n", buff);
  }

  return 0;
}
