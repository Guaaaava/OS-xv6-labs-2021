#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), '\0', DIRSIZ-strlen(p)); // transform into same form of target
  return buf;
}

int
recurse(char *path) // when . or .. no recurse
{
  char *buf = fmtname(path);
  if (buf[0] == '.' && buf[1] == '\0'){
    return 0; // 0 for no recurse
  }
  if (buf[0] == '.' && buf[1] == '.' && buf[2] == '\0'){
    return 0;
  }
  return 1; // 1 for recurse
}

void
find(char *path, char *target)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (strcmp(fmtname(path), target) == 0){
    printf("%s\n", path);
  }

  switch(st.type){
    case T_FILE:
      break;

    case T_DIR:
      if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
        printf("find: path too long\n");
        break;
      }
      strcpy(buf, path);
      p = buf+strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if(stat(buf, &st) < 0){
          printf("find: cannot stat %s\n", buf);
          continue;
        }

        if (recurse(buf) == 1){ // Don't recurse into "." and ".."
          find(buf, target);
        }
      }
      break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc < 2 || argc > 3){
    fprintf(2, "find.c: Expect at least 1 parameter, get %d parameters.\n", argc - 1);
    exit(1);
  }

  if (argc == 2){
    find(".", argv[1]);
    exit(0);
  }

  if (argc == 3){
    find(argv[1], argv[2]);
    exit(0);
  }

  exit(0);
}
