#include <stdio.h>
#include <signal.h>

static int child_id[5];
static int k;

// function to activate when a signal is caught
int sigCatcher() {
  signal(SIGINT, sigCatcher);
  printf("PID %d caught one\n", getpid());
  if (k > -1)
    kill (child_id[k], SIGINT);    // send signal to next child
}

int main() {
  int i;
  int zChild;
  int status;
  int PID;

  // sets a handler for INT signal
  signal(SIGINT, sigCatcher);

  for (i = 0; i < 5; i++) {
    PID = fork();
    if (PID == 0) {
      printf("PID %d ready\n", getpid());
      k = i - 1;
      pause();  // wait
      exit(0);  // end process
    } else
  
     child_id[i] = PID;
  }
  sleep(1);  // allow children time to enter pause
  kill (child_id[4], SIGINT);  // send signal to first child
  sleep(1); // allow children to get signal

  for (i = 0; i < 5; i++) {
    zChild = wait(&status);// collect zombies
    printf("%d is dead\n", zChild);
  }
  exit(0);
}
