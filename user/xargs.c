#include "kernel/types.h"
#include "kernel/param.h"
#include "user/user.h"

#define MSGSIZE 16 

int
main(int argc, char *argv[])
{
  sleep(10); // wait for the previous process to output

  // 1. Get the standardized output of the previous command
  char buf[MSGSIZE];
  read(0, buf, MSGSIZE); // process reads from file descriptor 0 (standard input)

  // 2. Get its own parameters
  char *xargv[MAXARG];
  int xargc = 0;
  for (int i = 1; i < argc; i ++){
    xargv[xargc] = argv[i];
    xargc ++;
  }

  char *p = buf;
  for (int i = 0; i < MSGSIZE; i ++){
    if (buf[i] == '\n'){
      int pid = fork();
      if (pid < 0){
        exit(1);
      }
      
      if (pid > 0){
        p = buf + i + 1;
        wait((int *)0);
      } else {
        // 3. Execute command using exec()
        buf[i] = 0;
        xargv[xargc] = p; // string before '\n'
        xargc ++;
        xargv[xargc] = 0;
        xargc ++;

        exec(xargv[0], xargv); // exec's argv = xargv + 1
        exit(0);
      }
    }
  }

  wait((int *)0);
  exit(0);
}