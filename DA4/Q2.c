/* Q2.c
 * Author : Pranav Karande
 * 20BCE0450
 *
 * Thread Syncronization has been achieved using global variables
 * and while loops for blocking and unblocking threads.
 */

#include <fcntl.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <unistd.h>

const int SIZE = 50;
const char *name = "OS";
int wait_reader = 1;
int wait_writer = 1;

void *writer(void *param);
void *reader(void *param);

int main() {
  printf("Author : Pranav Karande\n");
  printf("20BCE0450\n");

  pthread_t tid_writer, tid_reader;
  pthread_attr_t attr;

  pthread_attr_init(&attr);

  void *arg;
  pthread_create(&tid_writer, &attr, writer, arg);
  pthread_create(&tid_reader, &attr, reader, arg);

  pthread_join(tid_reader, NULL);
  pthread_join(tid_writer, NULL);

  printf("\nAll threads have finished executing.\n");
}

void *writer(void *param) {
  // Taking message as input
  char message[50];
  printf("\n(Writer thread)\tEnter message to write to shared memory: ");
  scanf("%[^\n]s", message);
  getchar();

  // Creating shared memory object
  int shm_fd;
  void *ptr;
  shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd, sizeof(message));
  ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

  // Writing message to shared memory
  sprintf(ptr, "%s", message);

  // Waiting for reader to execute
  printf("(Writer thread)\tMessage has been succesfully writen!!\n\n");
  printf("(Writer thread)\tWaiting for user to cat the shared memory content "
         "and reader to view "
         "the memory.\n");
  printf("(Writer thread)\tPress any key to continue....");
  getchar();

  // Releasing the block on reader thread
  wait_reader = 0;

  // Blocking writer thread
  while (wait_writer) ;

  // Changing the message
  for (int i = 5; i < 11; ++i) {
    message[i] = '-';
  }

  // Writing new message
  sprintf(ptr, "%s", message);
  printf("\n(Writer thread)\tMessage has succesfully been changed.\n");

  // Waiting for user to cat
  printf("(Writer thread)\tWaiting for user to cat the shared memory content "
         "and reader to view "
         "the memory.\n");;
  printf("(Writer thread)\tPress any key to continue....");
  getchar();

  // Releasing the block on reader
  wait_reader = 0;

  // Blocking writer thread
  wait_writer = 1;
  while(wait_writer) ;

  shm_unlink("OS");
  pthread_exit(0);
}

void *reader(void *param) {
  int shm_fd;
  void *ptr;

  // Blocking the reader thread
  while (wait_reader) ;

  // Reading shared memory object
  shm_fd = shm_open(name, O_RDONLY, 0666);
  ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);

  // Printing the message
  printf("\n(Reader thread)\tMessage read from shared memory is: %s\n", (char *)ptr);

  // Waiting for writer to change message
  printf("(Reader thread)\tWaiting for writer to change the content.\n");

  // Relesing the block on writer thread
  wait_writer = 0;

  // Blocking the writer thread
  wait_reader = 1;
  while(wait_reader);
  // Printing new message
  printf("\n(Reader thread)\tMessage read from the writer now is: %s\n", (char *)ptr);

  // Relesing the block on writer thread
  wait_writer = 0;

  pthread_exit(0);
}
