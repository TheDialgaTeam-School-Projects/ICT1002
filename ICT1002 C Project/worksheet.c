/*
 * ICT1002 Assignment 2, 2018-19 Trimester 1.
 *
 * This file implements all of the functions required for representing and
 * manipulating a worksheet.
 */

#include <ctype.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "sheet1002.h"
#include <stdarg.h>

/*
 * Get the contents of a cell as a floating-point number.
 *
 * Input:
 *   ws - a pointer to the worksheet
 *   col - the column of the desired cell
 *   row - the row of the desired cell
 *
 * Returns:
 *   a floating-point number with the value specified in the cell, if the cell contains a number
 *   NAN (defined in math.h), otherwise
 */
float ws_cell_as_float(WORKSHEET *ws, int col, int row)
{
    // Get the cell value.
    char *cell_value = ws->sheet[row - 1][col - 1];

    // Convert the cell value into float.
    const float float_value = atof(cell_value);

    // If it is a valid float value, return it.
    if (float_value != 0)
        return float_value;

    // If it is truly zero, return it.
    if (atof(cell_value) == 0 && (strcmp(cell_value, "0") == 0 || strcmp(cell_value, "0.0") == 0))
        return 0;

    // Convert failure, return NAN.
    return NAN;
}

/*
 * Get the contents of a cell as a string.
 *
 * Input:
 *   ws - a pointer to the worksheet
 *   col - the column of the desired cell
 *   row - the row of the desired cell
 *   width - the number of characters in the output string
 *   prec - the number of decimal places to use for numbers (ignored if the cell contains text)
 *   buf - a pointer to a buffer to receive the string; it should be at least width + 1 characters in size
 *
 * Returns:
 *   buf
 */
char *ws_cell_as_string(WORKSHEET *ws, int col, int row, int width, int prec, char *buf)
{
    // Get the cell value.
    const char *cell_value = ws->sheet[row - 1][col - 1];

    // Get the cell value length.
    const int cell_value_length = strlen(cell_value);

    // Buffer for column width value.
    char column_width[INTEGER_MAX_BUFF];
    itoa(width, column_width, 10);

    // Buffer for float precision value.
    char float_precision[INTEGER_MAX_BUFF];
    itoa(prec, float_precision, 10);

    // Buffer for string format.
    char string_format[INTEGER_MAX_BUFF + 3];
    string_copy(string_format, "%-", column_width, "s", NULL);

    // Buffer for float format.
    char float_format[INTEGER_MAX_BUFF + 4];
    string_copy(float_format, "%-", ".", float_precision, "f", NULL);

    // Buffer for size overflow value.
    char invalid_hash[MAX_WORD];

    for (int i = 0; i < cell_value_length; i++)
        invalid_hash[i] = '#';

    invalid_hash[cell_value_length] = '\0';

    if (ws_guess_data_type(cell_value) == WS_DATA_TYPE_FLOAT)
    {
        snprintf(buf, MAX_WORD, float_format, ws_cell_as_float(ws, col, row));
        const int buffer_length = strlen(buf);

        if (buffer_length > width)
            snprintf(buf, MAX_WORD, string_format, invalid_hash);
        else
            snprintf(buf, MAX_WORD, float_format, ws_cell_as_float(ws, col, row));
    }
    else
    {
        if (cell_value_length > width)
            snprintf(buf, MAX_WORD, string_format, invalid_hash);
        else
            snprintf(buf, MAX_WORD, string_format, cell_value);
    }

    return buf;
}

/*
 * Guess the intended data type of a string, in the manner of Excel's "General"
 * cell format.
 *
 * The input string will be assumed to represent a floating-point number if
 * either atof() returns a non-zero value, or the string looks like 0 or 0.0.
 *
 * The input string will be assumed to represent text if atof() returns
 * zero, the string is not 0 or 0.0, and it contains only characters
 * for which isalnum() returns a true value.
 *
 * Otherwise, the input string is considered to be illegal.
 *
 * Input:
 *   value - the string to be tested
 *
 * Returns:
 *   WS_DATA_TYPE_FLOAT, if the value appears to be a floating-point number
 *   WS_DATA_TYPE_TEXT, if the value appears to be text
 *   WS_DATA_TYPE_ILLEGAL, otherwise
 */
int ws_guess_data_type(const char *value)
{
    if (atof(value) != 0 || strcmp(value, "0") == 0 || strcmp(value, "0.0") == 0)
        return WS_DATA_TYPE_FLOAT;

    const int string_length = strlen(value);

    for (int i = 0; i < string_length; i++)
    {
        if (isalnum(value[i]))
            continue;

        return WS_DATA_TYPE_ILLEGAL;
    }

    return WS_DATA_TYPE_TEXT;
}

/*
 * De-allocate the memory used by a worksheet.
 *
 * Input:
 *   ws - a pointer to the worksheet to be de-allocated.
 */
void ws_free(WORKSHEET *ws)
{
    for (int row = 0; row < ws->rows; row++)
    {
        for (int column = 0; column < ws->cols; column++)
            free(ws->sheet[row][column]);

        free(ws->sheet[row]);
    }

    free(ws->sheet);
    free(ws);
}

/*
 * Create a new worksheet.
 *
 * Input:
 *   cols - the number of columns in the sheet
 *   rows - the number of rows in the sheet
 *
 * Returns:
 *   a pointer to the new WORKSHEET structure, if successful
 *   NULL, if there was a memory allocation failure
 */
WORKSHEET *ws_new(int cols, int rows)
{
    // Allocate memory for a new worksheet.
    WORKSHEET *worksheet = malloc(sizeof(WORKSHEET));

    // If allocation failure, return null.
    if (worksheet == NULL)
        return NULL;

    worksheet->cols = cols;
    worksheet->rows = rows;

    worksheet->view_column_offset = 0;
    worksheet->view_row_offset = 0;

    worksheet->cell_width = DEFAULT_CELL_WIDTH;
    worksheet->float_precision = DEFAULT_FLOAT_PRECISION;

    // Allocate memory for a new sheet containing X rows, X columns, X chars.
    char ***sheet = malloc(rows * sizeof(char**));

    // If allocation failure, return null.
    if (sheet == NULL)
        return NULL;

    for (int row = 0; row < rows; row++)
    {
        sheet[row] = malloc(cols * sizeof(char*));

        if (sheet[row] == NULL)
            return NULL;

        for (int column = 0; column < cols; column++)
        {
            sheet[row][column] = malloc(MAX_WORD * sizeof(char));

            if (sheet[row][column] == NULL)
                return NULL;

            strcpy(sheet[row][column], "");
        }
    }

    worksheet->sheet = sheet;

    return worksheet;
}

/*
 * Read data from a CSV file.
 *
 * If the data in the file has more columns or rows than the worksheet
 * supplied, the excess rows or columns will be ignored. If the data in the
 * file has fewer columns or rows than the worksheet, the extra rows and
 * columns will be made blank.
 *
 * If a cell in the file contains data that cannot be represented in a
 * worksheet according to the assignment specification, the cell will be
 * left blank.
 *
 * Input:
 *  ws - a pointer to the worksheet to receive the data
 *  f - a pointer to a FILE structure open for reading in text mode
 *
 * Returns:
 *   the number of rows successfully read and inserted into the worksheet (may be less than the number of rows in the file)
 */
int ws_read_csv(WORKSHEET *ws, FILE *f)
{
    return 0;
}

/*
 * Set the value of a cell.
 *
 * Input:
 *   ws - a pointer to the worksheet
 *   col - the column number of the cell
 *   row - the row number of the cell
 *   value - the new value of the cell (NULL to erase)
 */
void ws_set(WORKSHEET *ws, int col, int row, const char *value)
{
    if (value == NULL)
        strcpy(ws->sheet[row - 1][col - 1], "");
    else
        strcpy(ws->sheet[row - 1][col - 1], value);
}

/*
 * Write a worksheet to a CSV file.
 *
 * Input:
 *   ws - a pointer to the worksheet
 *   f - a pointer to a FILE open for writing in text mode
 *
 * Returns:
 *   the number of rows successfully written
 */
int ws_write_csv(WORKSHEET *ws, FILE *f)
{
    return 0;
}

/**
 * \brief Utility function for copying an array of strings into destination char pointer.
 * \param dest Destination of the string buffer that you want to copy into.
 * \param ... Char pointer that you want to copy into the destination string including the null terminating char. End with NULL.
 * \return The destination string that is copied into.
 */
char *string_copy(char *dest, ...)
{
    va_list args_list;
    va_start(args_list, dest);

    int is_valid = 1;
    int should_concatenate = 0;

    do
    {
        const char *args = va_arg(args_list, char*);

        if (args == NULL || args == '\0')
            is_valid = 0;
        else if (should_concatenate == 1)
            strcat(dest, args);
        else
        {
            strcpy(dest, args);
            should_concatenate++;
        }
    } while (is_valid == 1);

    va_end(args_list);

    return dest;
}
