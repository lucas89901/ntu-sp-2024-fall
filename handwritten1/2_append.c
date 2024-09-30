#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main() {
  char buffer[16];

  int fd = open("2_append.txt", O_RDWR | O_CREAT | O_APPEND,
                S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (fd < 0) {
    perror("open error");
    return 1;
  }
  if (write(fd, "abcdefgh", 8) < 0) {
    perror("write error");
    return 2;
  }

  // Can read from anywhere.
  if (lseek(fd, 2, SEEK_SET) < 0) {
    perror("lseek error");
    return 3;
  }
  if (read(fd, buffer, 3) < 0) {
    perror("read error");
    return 4;
  }
  assert(strncmp(buffer, "cde", 3) == 0);

  // Writing will always append.
  assert(lseek(fd, 0, SEEK_CUR) == 5);  // Current offset == 5.
  if (write(fd, "ijklmnop", 8) < 0) {
    perror("write error");
    return 5;
  }
  assert(lseek(fd, 0, SEEK_CUR) == 16);  // Confirm offset is at EOF.

  // Read the entire file and confirm that the second write is indeed append.
  if (lseek(fd, 0, SEEK_SET) < 0) {
    perror("lseek error");
    return 6;
  }
  if (read(fd, buffer, 16) < 0) {
    perror("read error");
    return 7;
  }
  assert(strncmp(buffer, "abcdefghijklmnop", 16) == 0);

  return 0;
}
