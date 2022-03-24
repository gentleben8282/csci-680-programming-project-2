/* CSCI 680: Programming Project #2: Synchronization
 * Programmer (Student ID): Ben Corriette (@02956064)
 * Last modified date: 03/23/2022
 * 
 * Summary: A program that implements a distributed 
 * application for managing queues.
 *
 * Comments: This solution is implemented via named pipes
 * and a custom queue class.
 *
 * References:
 * https://www.geeksforgeeks.org/named-pipe-fifo-example-c-program/
 * https://www.geeksforgeeks.org/queue-linked-list-implementation/
 * https://www.ibm.com/docs/en/zos/2.3.0?topic=functions-mkfifo-make-fifo-special-pipe
 * https://linux.die.net/man/2/umask
 * https://man7.org/linux/man-pages/man3/mkfifo.3.html
 * https://overiq.com/c-programming-101/array-of-strings-in-c/
 * https://replit.com/@BenCorriette/csci-680-programming-project-1
 */ 

#include "queue.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {

  printf("Starting program that uses named pipes to manage queues...\n");
  
  // File descriptors and message arrays
  int read_fd, write_fd;
  char messages_to_send[3][20] = {
    "Message 1",
    "Message 2",
    "Message 3"
  };
  char message_to_receive[20];

  // Named pipe path
  char * path_name = "temp.fifo";

  // Initialize two queues (sender and receiver)
  struct Queue* sender_queue = createQueue();
  struct Queue* receiver_queue = createQueue();
  
  printf("Created a sender and a receiver queue...\n");

  // Add the messages to send to the sender queue
  for (int i = 0; i < 3; i++) {
    enQueue(sender_queue, messages_to_send[i]);
    printf("Added '%s' to the sender queue...\n", messages_to_send + i);
  }

  printf("Creating the named pipe...\n");
  
  // Creating the named pipe
  if (mkfifo(path_name, S_IRWXU) != 0) {
    perror("mkfifo() error");
  } else {
    // Open both ends of the named pipe
    if ((read_fd = open(path_name, O_RDONLY|O_NONBLOCK)) < 0)
      perror("open() error for read end");
    else {
      if ((write_fd = open(path_name, O_WRONLY)) < 0)
        perror("open() error for write end");
      else {
        printf("Opened both ends (read and write) of the named pipe...\n");
        
        // Implement for loop to send all three messages
        while (sender_queue->front != NULL) {
          // Write queued messages to the named pipe
          if (write(write_fd, sender_queue->front->message, strlen(sender_queue->front->message)+1) == -1) {
            perror("write() error");
          }

          printf("Wrote '%s' to the named pipe...\n", sender_queue->front->message);

          printf("Removing '%s' from the sender queue...\n", sender_queue->front->message);
          
          // Remove the message from the sender queue
          deQueue(sender_queue);          

        // Read the message from the named pipe
        if (read(read_fd, message_to_receive, sizeof(message_to_receive)) == -1) {
          perror("read() error");
        }

          printf("Read '%s' from the named pipe...\n", message_to_receive);
          
          // Add the received message in the receiver queue
          enQueue(receiver_queue, message_to_receive);

          printf("Added '%s' to the receiver queue...\n", message_to_receive);
            
          printf("Read '%s' from the receiver queue...\n", receiver_queue->front->message);
        }
        printf("Closed the read file descriptor...\n");
        close(read_fd);
      }
      printf("Closed the write file descriptor...\n");
      close(write_fd);
    }
    printf("Removing the named pipe...\n");
    // Removes the named pipe 
    unlink(path_name);
  }
  printf("Ending the program...\n");
  return 0;
}