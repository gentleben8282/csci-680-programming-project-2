/* CSCI 680: Programming Project #2: Synchronization
 * Programmer (Student ID): Ben Corriette (@02956064)
 * Last modified date: 03/23/2022
 * 
 * Summary: A program that implements a distributed 
 * application for managing queues.
 *
 * References:
 * https://replit.com/@BenCorriette/csci-680-programming-project-1
 * https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/
 * https://www.geeksforgeeks.org/queue-linked-list-implementation/
 * https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-mkfifo-make-fifo-special-file
 * https://linux.die.net/man/2/umask
 * https://man7.org/linux/man-pages/man3/mkfifo.3.html
 */ 

#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  int read_fd, write_fd;
  char in[80], out[80];

  // Named file path
  char * path_name = "temp.fifo";

  // Test: Collect user input
  fgets(out, 80, stdin);

  // Creating the named file
  if (mkfifo(path_name, S_IRWXU) != 0) {
    perror("mkfifo() error");
  } else {
    // Open both ends of the named file
    if ((read_fd = open(path_name, O_RDONLY|O_NONBLOCK)) < 0)
      perror("open() error for read end");
    else {
      if ((write_fd = open(path_name, O_WRONLY)) < 0)
        perror("open() error for write end");
      else {
        if (write(write_fd, out, strlen(out)+1) == -1)
          perror("write() error");
        else if (read(read_fd, in, sizeof(in)) == -1)
          perror("read() error");
        else printf("read '%s' from the FIFO\n", in);
        close(write_fd);
      }
      close(read_fd);
    }
    unlink(path_name);
  }

  
  /*while (1)
  {
      // Open FIFO for write only
      fd = open(myfifo, O_WRONLY);

      // Take an input arr2ing from user.
      // 80 is maximum length
      fgets(arr2, 80, stdin);

      // Write the input arr2ing on FIFO
      // and close it
      write(fd, arr2, strlen(arr2)+1);
      close(fd);

      // Open FIFO for Read only
      fd = open(myfifo, O_RDONLY);

      // Read from FIFO
      read(fd, arr1, sizeof(arr1));

      // Print the read message
      printf("User2: %s\n", arr1);
      close(fd);
  }*/
  return 0;
}