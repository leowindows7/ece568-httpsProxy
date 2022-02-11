#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdio>
#include <cstdlib>

#define STDIN 0
#define STDOUT 1
#define STDERR 2

void create_daemon() {
  // get rid of tty
  pid_t pid = fork();
  if (pid < 0) {
    // TODO: refactor this to throw exception
    perror("Error");
    exit(EXIT_FAILURE);
  }

  if (pid != 0) {
    exit(EXIT_SUCCESS);
  }  // exit parent

  if (setsid() < 0) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  pid = fork();  // deamon become non-session leader
  if (pid < 0) {
    perror("Error");
    exit(EXIT_FAILURE);
  }

  if (pid != 0) {
    exit(EXIT_SUCCESS);
  }  // exit parent

  // redirect stdin, stderr, stdout to /dev/null
  int dev_null_fd = open("/dev/null", O_RDWR);
  if (dev_null_fd == -1) {
    perror("Error:");
    exit(EXIT_FAILURE);
  }

  int ret_in = dup2(STDIN, dev_null_fd);
  int ret_out = dup2(STDOUT, dev_null_fd);
  int ret_err = dup2(STDERR, dev_null_fd);

  if (ret_in == -1 || ret_err == -1 || ret_out == -1) {
    perror("Error:");
    exit(EXIT_FAILURE);
  }

  chdir("/");
  umask(0);

  // TODO: register signal for reconfiguration
}

int main() {
  printf("Creating daemon\n");
  create_daemon();
  printf("Successful create daemon\n");
  while (1) {
    sleep(2);
  }
}
