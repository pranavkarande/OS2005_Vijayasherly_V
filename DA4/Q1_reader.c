/* Q1_reader.c
 * Author : Pranav Karande
 * 20BCE0450
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <unistd.h>

int main(void) {
  printf("Author : Pranav Karande\n");
  printf("20BCE0450\n");

  const char *name = "OS";
  const int SIZE = 50;
  int shm_fd;
  void *ptr;

  // Reading shared memory object
  shm_fd = shm_open(name, O_RDONLY, 0666);
  ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

  // Printing the message
  printf("\nMessage read from shared memory is: %s\n", (char *)ptr);


  // Waiting for writer to change message
  printf("Waiting for writer to change the content.\n");
  printf("Press any key to continue....");
  getchar();

  // Printing new message
  printf("\nMessage read from the writer now is: %s\n", (char *)ptr);

  return 0;
}
