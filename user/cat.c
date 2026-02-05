#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "user/user.h"

char buf[512];

int
readline(int fd, char *buf, int maxlen)
{
  int n;
  char c;
  int i = 0;

  while((n = read(fd, &c, 1)) > 0){
    buf[i] = c;
    if(c == '\n')
      break;
    i += 1;
    if(i >= (maxlen - 1)){
      fprintf(2, "readline() - line too long\n");
      exit(1);
    }
  }
  if(((n == 0) && (i == 0)) || (n < 0))
    return n;

  i += 1;
  buf[i] = '\0';
  return i;
}

void
cat(int fd, int number_lines, int *line_no)
{
  int n;

  while((n = readline(fd, buf, sizeof(buf))) > 0){
    if(number_lines){
      printf("%d  %s", *line_no, buf);
      (*line_no)++;
    } else {
      if (write(1, buf, n) != n) {
        fprintf(2, "cat: write error\n");
        exit(1);
      }
    }
  }
  if(n < 0){
    fprintf(2, "cat: read error\n");
    exit(1);
  }
}

int
main(int argc, char *argv[])
{
  int fd, i;
  int number_lines = 0;
  int line_no = 1;

  if(argc > 1 && argv[1][0] == '-' && argv[1][1] == 'n' && argv[1][2] == '\0')
    number_lines = 1;

  if(argc <= 1 + number_lines){
    cat(0, number_lines, &line_no);
    exit(0);
  }

  for(i = 1 + number_lines; i < argc; i++){
    if((fd = open(argv[i], O_RDONLY)) < 0){
      fprintf(2, "cat: cannot open %s\n", argv[i]);
      exit(1);
    }
    cat(fd, number_lines, &line_no);
    close(fd);
  }
  exit(0);
}
