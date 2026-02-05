#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];

void
wc(int fd, char *name, int show_l, int show_w, int show_c)
{
  int i, n;
  int l, w, c, inword;

  l = w = c = 0;
  inword = 0;
  while((n = read(fd, buf, sizeof(buf))) > 0){
    for(i=0; i<n; i++){
      c++;
      if(buf[i] == '\n')
        l++;
      if(strchr(" \r\t\n\v", buf[i]))
        inword = 0;
      else if(!inword){
        w++;
        inword = 1;
      }
    }
  }
  if(n < 0){
    printf("wc: read error\n");
    exit(1);
  }
  int printed = 0;
  if(show_l){
    printf("%d", l);
    printed = 1;
  }
  if(show_w){
    if(printed)
      printf(" %d", w);
    else
      printf("%d", w);
    printed = 1;
  }
  if(show_c){
    if(printed)
      printf(" %d", c);
    else
      printf("%d", c);
    printed = 1;
  }
  if(name[0] != '\0')
    printf(" %s", name);
  printf("\n");
}

int
main(int argc, char *argv[])
{
  int fd, i;
  int show_l = 0, show_w = 0, show_c = 0;
  int file_count = 0;

  for(i = 1; i < argc; i++){
    if(argv[i][0] == '-' && argv[i][1] != '\0'){
      int j;
      for(j = 1; argv[i][j] != '\0'; j++){
        if(argv[i][j] == 'l')
          show_l = 1;
        else if(argv[i][j] == 'w')
          show_w = 1;
        else if(argv[i][j] == 'c')
          show_c = 1;
        else
          break;
      }
      if(argv[i][j] == '\0')
        continue;
    }
    file_count++;
  }

  if(show_l == 0 && show_w == 0 && show_c == 0){
    show_l = show_w = show_c = 1;
  }

  if(file_count == 0){
    wc(0, "", show_l, show_w, show_c);
    exit(0);
  }

  for(i = 1; i < argc; i++){
    if(argv[i][0] == '-' && argv[i][1] != '\0'){
      int j;
      for(j = 1; argv[i][j] != '\0'; j++){
        if(argv[i][j] != 'l' && argv[i][j] != 'w' && argv[i][j] != 'c')
          break;
      }
      if(argv[i][j] == '\0')
        continue;
    }
    if((fd = open(argv[i], O_RDONLY)) < 0){
      printf("wc: cannot open %s\n", argv[i]);
      exit(1);
    }
    wc(fd, argv[i], show_l, show_w, show_c);
    close(fd);
  }
  exit(0);
}
