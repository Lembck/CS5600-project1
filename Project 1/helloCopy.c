/*
 * file: hello.c - project1-part-2 micro-program
 * Created 
 *  Peter Desnoyers, Northeastern Khoury College, 2019
 */

#define BUFFER_SIZE 201
#include "elf64.h"
#include "sysdefs.h"

int write(int fd, void *ptr, int len) {
      if (len < 1)
   {
      return -1;
   }
   syscall(__NR_write, fd, ptr, len);
   return 1;
}

void print(char *buf){
	int i;
	for (i = 0; i < BUFFER_SIZE; i++)
   {
      write(1, &buf[i], 1);
      if (buf[i] == '\0')
      {
         break;
      }
   }
}

int main(void)
{
    print("Hello world!\n");
    return 0;
}

