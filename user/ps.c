#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/param.h"
#include "kernel/getproc.h"
#include "user/user.h"

static char *
state_name(enum procstate state)
{
  static char *states[] = {
    [UNUSED]   "unused",
    [USED]     "used",
    [SLEEPING] "sleep",
    [RUNNABLE] "runble",
    [RUNNING]  "run",
    [ZOMBIE]   "zombie",
  };

  if(state >= 0 && state < (int)(sizeof(states) / sizeof(states[0])) && states[state])
    return states[state];
  return "?";
}

int
main(void)
{
  struct procinfo procs[NPROC];
  int n;

  n = getprocs(procs);
  if(n < 0){
    fprintf(2, "ps: getprocs failed\n");
    exit(1);
  }

  printf("PID\tSTATE\tSIZE\tNAME\n");
  for(int i = 0; i < n; i++){
    printf("%d\t%s\t%d\t%s\n",
           procs[i].pid,
           state_name(procs[i].state),
           (int)procs[i].sz,
           procs[i].name);
  }

  exit(0);
}
