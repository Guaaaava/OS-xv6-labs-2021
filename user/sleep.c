#include "kernel/types.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  if (argc != 2){
    fprintf(2, "sleep.c: wants 1 parameter, get %d parameters.\n", argc - 1);
    exit(1);
  }

  int time;
  time = atoi(argv[1]);
  if (time <= 0)
    exit(1);

  sleep(time);
  exit(0);
}