#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

void f() {
  pid_t pid = vfork();
  if (pid < 0) {
    perror("vfork");
    _exit(1);
  } else if (pid == 0) {  // Child
    printf("Child pid = %d\n", getpid());
    return;
  } else {
    printf("Parent after vfork\n");
  }
}

int main() {
  f();
  return 0;
}
