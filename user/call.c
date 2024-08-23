#include "kernel/param.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int g(int x) {
  return x+3;
}

int f(int x) {
  return g(x);
}

void main(void) {
//  printf("%d %d\n", f(8)+1, 13);
  unsigned int i = 0x00646c72; // for little-endian
//  unsigned int i = 0x726c6400; for big-endian
	printf("H%x Wo%s\n", 57616, &i);
  exit(0);
}
