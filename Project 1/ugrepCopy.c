/*
 * file: ugrep.c - project-1-part2 micro-program
 * Created
 *  Peter Desnoyers, Northeastern Khoury College, 2019
 */

#include "elf64.h"
#include "sysdefs.h"

#define BUFFER_SIZE 201
char BUF[200];
char *argv[10];
int argc;

int read(int fd, void *ptr, int len)
{
	if (len < 1)
	{
		return -1;
	}

	syscall(__NR_read, fd, ptr, len);
	return 1;
}

int write(int fd, void *ptr, int len)
{
	if (len < 1)
	{
		return -1;
	}
	syscall(__NR_write, fd, ptr, len);
	return 1;
}

/* simple function to split a line:
 *   char buffer[200];
 *   <read line into 'buffer'>
 *   char *argv[10];
 *   int argc = split(argv, 10, buffer);
 *   ... pointers to words are in argv[0], ... argv[argc-1]
 */
int split(char **argv, int max_argc, char *line)
{
	int i = 0;
	char *p = line;

	while (i < max_argc)
	{
		while (*p != 0 && (*p == ' ' || *p == '\t' || *p == '\n'))
			*p++ = 0;
		if (*p == 0)
			return i;
		argv[i++] = p;
		while (*p != 0 && *p != ' ' && *p != '\t' && *p != '\n')
			p++;
	}
	return i;
}

void readline(char *buf, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		read(0, &buf[i], 1);
		if (buf[i] == '\n')
		{
			buf[++i] = '\0';
			break;
		}
	}
}

void print(char *buf)
{
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
char *getarg(int i)
{
	int argc = split(argv, 10, BUF);
	// print(argv[0]);
	if (i >= argc)
	{
		return 0;
	}
	return argv[i];
}

/*
 * Crude string functions.
 */
int strings_equal(char *s1, char *s2)
{
	while (*s1 != 0)
		if (*s1 != *s2)
			return 0;
		else
			s1++, s2++;
	return 1;
}

int string_contains(char *pattern, char *str)
{
	while (*str != 0)
	{
		if (strings_equal(pattern, str))
			return 1;
		str++;
	}
	return 0;
}

int main(void)
{
	char buf[128];
	char *pattern = getarg(1);

	if (pattern == 0 || getarg(2) != 0)
	{
		print("usage: upreg <pattern>\n");
		return 0;
	}

	print("ugrep: enter blank line to quit\n");

	for (;;)
	{
		readline(buf, sizeof(buf));
		if (buf[0] == '\n' || buf[0] == 0)
			break;
		if (string_contains(pattern, buf))
		{
			print("-- ");
			print(buf);
		}
	}
	return 0;
}
