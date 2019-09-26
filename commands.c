/*
 * ICT1002 Assignment 2, 2018-19 Trimester 1.
 *
 * This file contains one function corresponding to each command understood
 * by the spreadsheet. Each function accepts zero, one or two input arguments
 * (arg1 and/or arg2) as documented in the assignment specification, and a
 * pointer to buffer (output) into which the results of the command should be
 * placed. The contents of the output buffer will be displayed on the screen
 * at the end of the command loop (make it empty if you don't want to print
 * anything).
 *
 * The contents of arg1 and arg2 are guaranteed to be no more than MAX_INPUT
 * characters in length. The contents of the output buffer should not exceed
 * MAX_OUTPUT characters (including the terminating null).
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sheet1002.h"

/*
 * Execute a command.
 *
 * Input:
 *   command - the name of the command
 *   arg1 - the first argument (may be NULL)
 *   arg2 - the second argument (may be NULL)
 *   output - a buffer into which the output of the command is to be placed (must be at least MAX_OUTPUT characters long)
 *
 * Returns:
 *   0, if the program should continue
 *   1, if the command indicates that the interpreter should exit
 */
int do_command(const char *command, const char *arg1, const char *arg2, char *output)
{
    int done = 0; /* return value */

    if (command == NULL || strlen(command) == 0)
    {
        /* blank line; do nothing and return */
        return 0;
    }

    /* determine which command was given and execute the appropriate function */
    if (compare_token(command, "avg") == 0)
        do_avg(arg1, arg2, output);
    else if (compare_token(command, "cursor") == 0)
        do_cursor(arg1, output);
    else if (compare_token(command, "load") == 0)
        do_load(arg1, output);
    else if (compare_token(command, "new") == 0)
        do_new(arg1, arg2, output);
    else if (compare_token(command, "prec") == 0)
        do_prec(arg1, output);
    else if (compare_token(command, "save") == 0)
        do_save(arg1, output);
    else if (compare_token(command, "set") == 0)
        do_set(arg1, arg2, output);
    else if (compare_token(command, "sum") == 0)
        do_sum(arg1, arg2, output);
    else if (compare_token(command, "width") == 0)
        do_width(arg1, output);
    else if (compare_token(command, "exit") == 0)
        done = 1;
    else
        snprintf(output, MAX_OUTPUT, "Unrecognised command: %s.", command);

    return done;
}

/*
 * AVG command.
 *
 * Input:
 *   arg1 - one corner of the rectangle to be averaged
 *   arg2 - the opposite corner of the rectangle to be averaged
 */
void do_avg(const char *arg1, const char *arg2, char *output)
{
    int column = 0;
    int row = 0;

    int columnLast = 0;
    int rowLast = 0;

    int i;
    int i2;

    float tt;
    float avg = 0;
    float sum = 0;
    int counter = 0;

    // if the length of the cell identifier is less than 2 or the first character is not alphabetic [A-Z]
    if (strlen(arg1) < 2 || !isalpha(arg1[0]))
    {
        snprintf(output, MAX_OUTPUT, "Invalid cell identifier given.");
        return;
    }

    //get column for arg1 (B)
    column = toupper(arg1[0]) - 'A' + 1;

    //get rows for arg1 (3)
    char row_string[MAX_INPUT];
    string_substring(row_string, arg1, 1);
    row = atoi(row_string);

    //get column for arg2 (E)
    columnLast = toupper(arg2[0]) - 'A' + 1;

    //get rows for arg2 (6)
    char row_string_last[MAX_INPUT];
    string_substring(row_string_last, arg2, 1);
    rowLast = atoi(row_string_last);

    if (column <= columnLast)
    {
        //Loop through 
        for (i = column; i <= columnLast; i++)
        {
            for (i2 = row; i2 <= rowLast; i2++)
            {
                if (ws_guess_data_type(viewport_get_worksheet()->sheet[i2 - 1][i - 1]) == 1)
                {
                    tt = 0;
                }
                else
                {
                    tt = ws_cell_as_float(viewport_get_worksheet(), i, i2);
                    if (tt != 0)
                    {
                        sum = sum + tt;
                        counter = counter + 1;
                    }
                }
            }
        }
    }
    else
    {
        //Loop through 
        for (i = columnLast; i <= column; i++)
        {
            for (i2 = rowLast; i2 <= row; i2++)
            {
                if (ws_guess_data_type(viewport_get_worksheet()->sheet[i2 - 1][i - 1]) == 1)
                {
                    tt = 0;
                }
                else
                {
                    tt = ws_cell_as_float(viewport_get_worksheet(), i, i2);
                    if (tt != 0)
                    {
                        sum = sum + tt;
                        counter = counter + 1;
                    }
                }
            }
        }
    }

    // Validation
    if (column > viewport_get_worksheet()->cols || row < 1 || row > viewport_get_worksheet()->rows)
    {
        snprintf(output, MAX_OUTPUT, "Invalid cell identifier given.");
        return;
    }

    if (arg2 != NULL && strlen(arg2) >= MAX_WORD)
    {
        snprintf(output, MAX_OUTPUT, "The word to set is too long.");
        return;
    }

    if (arg2 != NULL && ws_guess_data_type(arg2) == WS_DATA_TYPE_ILLEGAL)
    {
        snprintf(output, MAX_OUTPUT, "Illegal text format inputted.");
        return;
    }

    avg = sum / counter;
    snprintf(output, MAX_OUTPUT, "The average is: %f", avg);
}

/*
 * CURSOR command.
 *
 * Input:
 *   arg1 - the identifier of the cell to which to move the cursor
 */
void do_cursor(const char *arg1, char *output)
{
    int column = 0;
    int row = 0;

    // if the length of the cell identifier is less than 2 or the first character is not alphabetic [A-Z]
    if (strlen(arg1) < 2 || !isalpha(arg1[0]))
    {
        snprintf(output, MAX_OUTPUT, "Invalid cell identifier given.");
        return;
    }

    column = toupper(arg1[0]) - 'A' + 1;

    char row_string[MAX_INPUT];
    string_substring(row_string, arg1, 1);

    row = atoi(row_string);

    if (column > viewport_get_worksheet()->cols || row < 1 || row > viewport_get_worksheet()->rows)
    {
        snprintf(output, MAX_OUTPUT, "Invalid cell identifier given.");
        return;
    }

    viewport_set_cursor(column, row);

    char cell_upper[MAX_OUTPUT];
    strcpy(cell_upper, arg1);
    cell_upper[0] = toupper(cell_upper[0]);

    snprintf(output, MAX_OUTPUT, "Set the cursor to %s.", cell_upper);
}

/*
 * LOAD command.
 *
 * Input:
 *   arg1 - the filename
 */
void do_load(const char *arg1, char *output)
{
    int rows = 0;
    int columns = 0;
    char filename[] = {arg1};
    char buffer[BSIZE];
    FILE *f;
    char *field;
    int Intake;
    char Cluster, Type;
    float Percentage;
    f = fopen(arg1, "r");
    if (f == NULL)
    {
        printf("Unable to open file '%s'\n", filename);
        exit(1);
    }
    /* process the data */
    /* the file contains 4 fields in a specific order:
        cluster,intake,blank,percentage
       separated by commas */
    while (fgets(buffer, BSIZE, f))
    {
        /* get Cluster */
        field = strtok(buffer, ",");
        char Cluster[MAX_INPUT];
        strcpy(Cluster, field);
        /* get Intake */
        field = strtok(NULL, ",");
        char Intake[MAX_INPUT];
        strcpy(Intake, field);
        /* get Type */
        field = strtok(NULL, ",");
        char Type[MAX_INPUT];
        strcpy(Type, field);
        /* get Percentage */
        field = strtok(NULL, ",");
        char Percentage[MAX_INPUT];
        strcpy(Percentage, field);

        /* display the result in the proper format */

        printf(" %s, %s ,%s ,%s",

               Cluster,
               Intake,
               Type,
               Percentage);
    }

    /* close file */
    fclose(f);
    snprintf(output, MAX_OUTPUT, "\n Loaded CSV file successfuly, ");
}

/*
 * NEW command.
 *
 * Input:
 *   arg1 - the number of columns
 *   arg2 - the number of rows
 */
void do_new(const char *arg1, const char *arg2, char *output)
{
    const int columns = atoi(arg1);
    const int rows = atoi(arg2);

    if (columns < 1 || rows < 1 || columns > MAX_COLUMNS)
    {
        snprintf(output, MAX_OUTPUT, "ERROR: Invalid number of columns or rows.");
        return;
    }

    if (viewport_get_worksheet() != NULL)
        ws_free(viewport_get_worksheet());

    viewport_set_worksheet(ws_new(columns, rows));

    snprintf(output, MAX_OUTPUT, "New worksheet created with %d columns and %d rows.", viewport_get_worksheet()->cols,
             viewport_get_worksheet()->rows);
}

/*
 * PREC command.
 *
 * Input:
 *   arg1 - the number of decimal places to show
 */
void do_prec(const char *arg1, char *output)
{
    const int float_precision = atoi(arg1);

    if (float_precision == 0 && strcmp(arg1, "0") != 0 && strcmp(arg1, "0.0") != 0)
        snprintf(output, MAX_OUTPUT, "Invalid number.");
    else if (float_precision < 0)
        snprintf(output, MAX_OUTPUT, "Floating point precision cannot be less than 0.");
    else
        viewport_set_cellprec(float_precision);
}

/*
 * SAVE command.
 *
 * Input:
 *  arg1 - the filename
 */
void do_save(const char *arg1, char *output)
{
    if (viewport_get_worksheet() == NULL)
    {
        snprintf(output,MAX_OUTPUT, "Worksheet not found");
        return;
    }

    if (arg1 == NULL)
    {
        snprintf(output, MAX_OUTPUT, "No input detected");
        return;
    }

    strcat(arg1, ".csv");
    FILE *fp = fopen(arg1, "w");

    int i = ws_write_csv(viewport_get_worksheet(), fp);

    if (i < 0)
    {
        snprintf(output, MAX_OUTPUT, "Error in creating");
    }
    else
    {
        snprintf(output, MAX_OUTPUT, "File created, %d rows created", i);
    }
}

/*
 * SET command.
 *
 * Input:
 *   arg1 - the cell identifier
 *   arg2 - the value for the cell (NULL to make the cell blank)
 */
void do_set(const char *arg1, const char *arg2, char *output)
{
    int column = 0;
    int row = 0;

    // if the length of the cell identifier is less than 2 or the first character is not alphabetic [A-Z]
    if (strlen(arg1) < 2 || !isalpha(arg1[0]))
    {
        snprintf(output, MAX_OUTPUT, "Invalid cell identifier given.");
        return;
    }

    column = toupper(arg1[0]) - 'A' + 1;

    char row_string[MAX_INPUT];
    string_substring(row_string, arg1, 1);

    row = atoi(row_string);

    if (column > viewport_get_worksheet()->cols || row < 1 || row > viewport_get_worksheet()->rows)
    {
        snprintf(output, MAX_OUTPUT, "Invalid cell identifier given.");
        return;
    }

    if (arg2 != NULL && strlen(arg2) >= MAX_WORD)
    {
        snprintf(output, MAX_OUTPUT, "The word to set is too long.");
        return;
    }

    if (arg2 != NULL && ws_guess_data_type(arg2) == WS_DATA_TYPE_ILLEGAL)
    {
        snprintf(output, MAX_OUTPUT, "Illegal text format inputted.");
        return;
    }

    ws_set(viewport_get_worksheet(), column, row, arg2);

    char cell_upper[MAX_OUTPUT];
    strcpy(cell_upper, arg1);
    cell_upper[0] = toupper(cell_upper[0]);

    if (arg2 != NULL)
        snprintf(output, MAX_OUTPUT, "Cell %s set to %s.", cell_upper, arg2);
    else
        snprintf(output, MAX_OUTPUT, "Cell %s is cleared.", cell_upper);
}

/*
 * SUM command.
 *
 * Input:
 *   arg1 - one corner of the rectangle to be summed
 *   arg2 - the opposite corner of the rectangle to be summed
 */
void do_sum(const char *arg1, const char *arg2, char *output)
{
    int column = 0;
    int row = 0;

    int columnLast = 0;
    int rowLast = 0;

    int i;
    int i2;

    float tt = 0;
    float sum = 0;

    // if the length of the cell identifier is less than 2 or the first character is not alphabetic [A-Z]
    if (strlen(arg1) < 2 || !isalpha(arg1[0]))
    {
        snprintf(output, MAX_OUTPUT, "Invalid cell identifier given.");
        return;
    }

    //get column for arg1 (B)
    column = toupper(arg1[0]) - 'A' + 1;

    //get rows for arg1 (3)
    char row_string[MAX_INPUT];
    string_substring(row_string, arg1, 1);
    row = atoi(row_string);

    //get column for arg2 (E)
    columnLast = toupper(arg2[0]) - 'A' + 1;

    //get rows for arg2 (6)
    char row_string_last[MAX_INPUT];
    string_substring(row_string_last, arg2, 1);
    rowLast = atoi(row_string_last);

    if (column <= columnLast)
    {
        //Loop through 
        for (i = column; i <= columnLast; i++)
        {
            for (i2 = row; i2 <= rowLast; i2++)
            {
                if (ws_guess_data_type(viewport_get_worksheet()->sheet[i2 - 1][i - 1]) == 1)
                {
                    tt = 0;
                }
                else
                {
                    tt = ws_cell_as_float(viewport_get_worksheet(), i, i2);
                    if (tt != 0)
                    {
                        sum = sum + tt;
                    }
                }
            }
        }
    }
    else
    {
        //Loop through 
        for (i = columnLast; i <= column; i++)
        {
            for (i2 = rowLast; i2 <= row; i2++)
            {
                if (ws_guess_data_type(viewport_get_worksheet()->sheet[i2 - 1][i - 1]) == 1)
                {
                    tt = 0;
                }
                else
                {
                    tt = ws_cell_as_float(viewport_get_worksheet(), i, i2);
                    if (tt != 0)
                    {
                        sum = sum + tt;
                    }
                }
            }
        }
    }

    // Validation
    if (column > viewport_get_worksheet()->cols || row < 1 || row > viewport_get_worksheet()->rows)
    {
        snprintf(output, MAX_OUTPUT, "Invalid cell identifier given.");
        return;
    }

    if (arg2 != NULL && strlen(arg2) >= MAX_WORD)
    {
        snprintf(output, MAX_OUTPUT, "The word to set is too long.");
        return;
    }

    if (arg2 != NULL && ws_guess_data_type(arg2) == WS_DATA_TYPE_ILLEGAL)
    {
        snprintf(output, MAX_OUTPUT, "Illegal text format inputted.");
        return;
    }

    snprintf(output, MAX_OUTPUT, "The sum is: %f", sum);
}

/*
 * WIDTH command.
 *
 * Input:
 *   arg1 - the number of characters for each column
 */
void do_width(const char *arg1, char *output)
{
    const int column_width = atoi(arg1);

    if (column_width == 0 && strcmp(arg1, "0") != 0 && strcmp(arg1, "0.0") != 0)
        snprintf(output, MAX_OUTPUT, "Invalid number.");
    else if (column_width <= 0)
        snprintf(output, MAX_OUTPUT, "Column width is too small.");
    else
        viewport_set_cellwidth(column_width);
}

/**
 * \brief Utility function for getting the substring.
 * \param dest Destination of the string after getting the substring.
 * \param src Source of the string.
 * \param offset Starting offset of where to get the substring.
 * \return A char pointer that consist of the substring.
 */
char *string_substring(char *dest, const char *src, const int offset)
{
    const int string_length = strlen(src);

    if (offset == 0)
    {
        strcpy(dest, src);
        return dest;
    }

    if (offset >= string_length)
    {
        strcpy(dest, NULL);
        return dest;
    }

    for (int i = offset; i < string_length; i++)
        dest[i - offset] = src[i];

    dest[string_length - offset] = '\0';

    return dest;
}
