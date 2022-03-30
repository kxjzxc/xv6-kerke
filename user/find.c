#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

void find(char *path, char *file_name)
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

  if(st.type != T_DIR)
  {
    fprintf(2, "find: cannot find in %s\n", path);
    close(fd);
    return;
  }
  if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){//路径太长了
    return;
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
    char *name = fmtname(buf);
    switch (st.type)
    {
    case T_FILE:
      if(strcmp(name, file_name) == 0)
      {
        printf("%s\n", buf);
      }
      break;
    case T_DIR:
      if(strcmp(name, ".") == 0 || strcmp(name, "..") == 0)
        continue;
      find(buf, file_name);
      break;
    default:
      break;
    }
  }
  close(fd);
}

int main(int argc, char *argv[])
{

  if(argc < 3){
    printf("please input find [path] [file_name]\n");
    exit(0);
  }
  find(argv[1], argv[2]);
  exit(0);
}
