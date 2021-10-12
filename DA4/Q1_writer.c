/* Q1_writer.c
 * Author : Pranav Karande
 * 20BCE0450
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <unistd.h>

int main() {
  printf("Author : Pranav Karande\n");
  printf("20BCE0450\n");

  const int SIZE = 50;
  const char *name = "OS";
  int shm_fd;
  void *ptr;

  // Taking message as input
  char message[50];
  printf("\nEnter message to write to shared memory: ");
  scanf("%[^\n]s", message);
  getchar();

  // Creating shared memory object
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd, sizeof(message));
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

  // Writing message to shared memory
  sprintf(ptr, "%s", message);

  // Waiting for reader to execute
  printf("Message has been succesfully writen!!\n\n");
  printf("Waiting for user to cat the shared memory content and reader to view "
         "the memory.\n");
  printf("Press any key to continue....");
  getchar();

  // Changing the message
  for (int i = 5; i < 11; ++i) {
    message[i] = '-';
  }

  // Writing new message
  sprintf(ptr, "%s", message);
  printf("\nMessage has succesfully been changed.\n");

  // Waiting for user to cat
  printf("Waiting for user to cat the shared memory content and reader to view "
         "the memory.\n");
  printf("Press any key to end the program....");
  getchar();

  shm_unlink("OS");
  return 0;
}
