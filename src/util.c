#include "main.h"

#include <errno.h>
#include <stddef.h>
#include <string.h>
#include <unistd.h>

int ft_abs(int n)
{
	return (n < 0 ? -n : n);
}

int get_sign(int n)
{
	if (n < 0)
		return -1;
	else if (n > 0)
		return 1;
	else
		return 0;
}

unsigned int digits_in_unsigned_int(unsigned int number)
{
	unsigned int digits;

	digits = 1;
	while (number > 9)
	{
		number /= 10;
		digits++;
	}
	return (digits);
}

void print_error(char const *message)
{
	print_string(STDERR_FILENO, message);
	if (errno != 0)
	{
		print_string(STDERR_FILENO, ": ");
		print_string(STDERR_FILENO, strerror(errno));
	}
	print_string(STDERR_FILENO, "\n");
}

void print_string(int fd, char const *s)
{
	size_t len;

	len = 0;
	while (s[len])
		len++;

	write(fd, s, len);
}

void print_unsigned_int(unsigned int number)
{
	unsigned int divider;
	char buffer[10];
	size_t buffer_length;

	divider = 1;
	while (divider <= number / 10)
		divider *= 10;

	buffer_length = 0;
	while (divider != 0)
	{
		buffer[buffer_length] = number / divider + '0';
		buffer_length++;

		number %= divider;
		divider /= 10;
	}

	write(STDOUT_FILENO, buffer, buffer_length);
}

int string_to_unsigned_int(char const *s, unsigned int *n)
{
	*n = 0;
	while (*s >= '0' && *s <= '9')
	{
		*n = *n * 10 + *s - '0';
		s++;
	}
	return (!(*s));
}
