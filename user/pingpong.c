#include "kernel/types.h"
#include "user/user.h"

#define MSGSIZE 8

int
main(int argc, char *argv[])
{
  if (argc != 1){
    fprintf(2, "pingpong.c: Expect 0 parameter, get %d parameters.\n", argc - 1);
    exit(1);
  }

  int fd1[2], fd2[2];
  char buf[MSGSIZE];
  
  if (pipe(fd1) < 0 || pipe(fd2) < 0) // Declare as a pipe
    exit(1);

  int pid = fork(); // fork() returns child's pid
  if (pid < 0) // returns -1 for error
    exit(1);

  if (pid > 0){ // positive for parent
    write(fd1[1], "ping", 5);
    wait((int *)0);
    read(fd2[0], buf, MSGSIZE);
    fprintf(1, "%d: received %s\n", getpid(), buf);
  } else { // 0 for child
    read(fd1[0], buf, MSGSIZE);
    fprintf(1, "%d: received %s\n", getpid(), buf);
    write(fd2[1], "pong", 5);
  }

  close(fd1[0]);
  close(fd1[1]);
  close(fd2[0]);
  close(fd2[1]);

  exit(0);
}