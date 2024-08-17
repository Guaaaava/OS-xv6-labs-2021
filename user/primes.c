#include "kernel/types.h"
#include "user/user.h"

#define MSGSIZE 36
#define ONE '1' // might be prime
#define ZERO '0' // not prime or printed

void prime(int pipe_read, int pipe_write){ // recursive
  char nums[MSGSIZE];
  read(pipe_read, nums, MSGSIZE);

  int val = 0;
  for (int i = 0; i < MSGSIZE; i ++){
    if (nums[i] == ONE){ // first number is prime
      val = i;
      break;
    }
  }
  if (val == 0) // no more primes
    exit(0);

  printf("prime %d\n", val);
  nums[val] = ZERO; // set "ZERO" after printed

  for (int i = 0; i < MSGSIZE; i ++){
    if (i % val == 0){ // multiples of val are not prime
      nums[i] = ZERO;
    }
  }

  int pid = fork();
  if (pid < 0)
    exit(1);

  if (pid > 0){ // main process's child
    write(pipe_write, nums, MSGSIZE);
    close(pipe_write); // close write after write
    wait((int *)0);
  } else { // main process's grandchild
    prime(pipe_read, pipe_write);
    close(pipe_read); // close read after read
  }
}

int
main(int argc, char *argv[])
{
  if (argc != 1){
    fprintf(2, "primes.c: Expect 0 parameter, get %d parameters.\n", argc - 1);
    exit(1);
  }

  int fd[2];
  char buf[MSGSIZE];

  if (pipe(fd) < 0) // Declare as a pipe
    exit(1);

  for (int i = 0; i < MSGSIZE; i ++){
    buf[i] = ONE;
  }

  int pid = fork();
  if (pid < 0) // returns -1 for error
    exit(1);

  if (pid > 0){ // positive for parent(main process)
    buf[0] = ZERO; // 0 is not prime
    buf[1] = ZERO; // 1 is not prime

    write(fd[1], buf, MSGSIZE);
    close(fd[1]); // close write after write
    wait((int *)0);
  } else { // 0 for child
    prime(fd[0], fd[1]);
    close(fd[0]); // close read after read
  }

  exit(0);
}