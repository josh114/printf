/*
 * File: _printf.c
 * Auth: Joshua kenedinum
 */

#include "main.h"

void clearCode(va_list args, buffer_t *output);
int run(const char *format, va_list args, buffer_t *output);
int _printf(const char *format, ...);

/**
 * clearCode - Peforms cleanup operations for _printf.
 * @args: A va_list of arguments provided to _printf.
 * @output: A buffer_t struct.
 */
void clearCode(va_list args, buffer_t *output)
{
	va_end(args);
	write(1, output->start, output->len);
	free_buffer(output);
}

/**
 * run_printf - Reads through the format string for _printf.
 * @format: Character string to print - may contain directives.
 * @output: A buffer_t struct containing a buffer.
 * @args: A va_list of arguments.
 *
 * Return: The number of characters stored to output.
 */
int run(const char *format, va_list args, buffer_t *output)
{
	int i, wid, prec, ret = 0;
	char kip;
	unsigned char flags, len;
	unsigned int (*f)(va_list, buffer_t *,
			unsigned char, int, int, unsigned char);

	for (i = 0; *(format + i); i++)
	{
		len = 0;
		if (*(format + i) == '%')
		{
			kip = 0;
			flags = handle_flags(format + i + 1, &kip);
			wid = handle_width(args, format + i + kip + 1, &kip);
			prec = handle_precision(args, format + i + kip + 1,
					&kip);
			len = handle_length(format + i + kip + 1, &kip);

			f = handle_specifiers(format + i + kip + 1);
			if (f != NULL)
			{
				i += kip + 1;
				ret += f(args, output, flags, wid, prec, len);
				continue;
			}
			else if (*(format + i + kip + 1) == '\0')
			{
				ret = -1;
				break;
			}
		}
		ret += _memcpy(output, (format + i), 1);
		i += (len != 0) ? 1 : 0;
	}
	cleanup(args, output);
	return (ret);
}

/**
 * _printf - Outputs a formatted string.
 * @format: Character string to print - may contain directives.
 *
 * Return: The number of characters printed.
 */
int _printf(const char *format, ...)
{
	buffer_t *output;
	va_list args;
	int ret;

	if (format == NULL)
		return (-1);
	output = init_buffer();
	if (output == NULL)
		return (-1);

	va_start(args, format);

	ret = run(format, args, output);

	return (ret);
}
