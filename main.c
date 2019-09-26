/*
 * ICT1002 Assignment 2, 2018-19 Trimester 1.
 *
 * This file implements the main program, including parsing of commands.
 *
 * DO NOT MODIFY THIS FILE. You may invoke its functions if you like, however.
 */

#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sheet1002.h"

/*
 * Main loop.
 */
int main(int argc, char *argv[])
{
    int num_tokens; /* the number of tokens in the input */
    char command[MAX_INPUT]; /* buffer for holding the command */
    char arg1[MAX_INPUT]; /* buffer for holding the first argument */
    char arg2[MAX_INPUT]; /* buffer for holding the second argument */
    char output[MAX_OUTPUT]; /* buffer for holding the output of the command */
    int done = 0; /* set to 1 to end the main loop */

    /* initialise the output with a welcome message */
    strcpy(output, "Welcome to the ICT1002 Spreadsheet!");

    /* main command loop */
    do
    {
        /* display the worksheet */
        viewport_display(TERMINAL_COLS, TERMINAL_ROWS - 2);

        /* print the output of the last command */
        printf("%s\n", output);

        /* read a line of input */
        do
        {
            printf("? ");
            num_tokens = lscanf("%s%s%s", command, arg1, arg2);
        } while (num_tokens < 1);

        if (num_tokens == 1)
        {
            /* command with no argument */
            done = do_command(command, NULL, NULL, output);
        }
        else if (num_tokens == 2)
        {
            /* command with one argument */
            done = do_command(command, arg1, NULL, output);
        }
        else
        {
            /* command with two arguments */
            done = do_command(command, arg1, arg2, output);
        }
    } while (!done);

    /* print a goodbye message */
    printf("Goodbye!\n");

    return 0;
}

/*
 * Utility function for comparing commands, arguments, etc. case-insensitively.
 *
 * Input:
 *   token1 - the first token
 *   token2 - the second token
 *
 * Returns:
 *   as strcmp()
 */
int compare_token(const char *token1, const char *token2)
{
    int i = 0;
    while (token1[i] != '\0' && token2[i] != '\0')
    {
        if (toupper(token1[i]) < toupper(token2[i]))
            return -1;
        if (toupper(token1[i]) > toupper(token2[i]))
            return 1;
        i++;
    }

    if (token1[i] == '\0' && token2[i] == '\0')
        return 0;
    if (token1[i] == '\0')
        return -1;
    return 1;
}

/*
 * Utility function for parsing a line of input, ensuring that everything is
 * read to the end of the line.
 *
 * The format string for this function also accepts a special format string
 * "%z", which will read the whole line into the first non-format argument.
 * This argument should have space for at least MAX_INPUT characters.
 *
 * Input:
 *   as scanf()
 *
 * Returns:
 *   the number of tokens successfully parsed
 */
int lscanf(const char *format, ...)
{
    char line[MAX_INPUT]; /* buffer for holding the line of input */

    /* initialise variable argument list */
    va_list args;
    va_start(args, format);

    /* read the line */
    fgets(line, MAX_INPUT, stdin);

    int num_tokens = 0;
    if (format[0] == '%' && format[strlen(format) - 1] == 'z')
    {
        /* get the maximum number of characters to read, if any */
        int n = atoi(format + 1);
        if (n <= 0)
            n = MAX_INPUT;

        /* copy the whole line, up to n characters */
        char *output = va_arg(args, char *);
        strncpy(output, line, n);
        num_tokens = 1;

        /* remove the trailing '\n' */
        char *p = strchr(output, '\n');
        if (p != NULL)
            *p = '\0';
    }
    else
    {
        /* pass to scanf */
        num_tokens = vsscanf(line, format, args);
    }

    /* clean up variable argument list */
    va_end(args);

    return num_tokens;
}
