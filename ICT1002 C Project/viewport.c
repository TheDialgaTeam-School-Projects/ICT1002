/*
 * ICT1002 Assignment 2, 2018-19 Trimester 1.
 *
 * This file implements all of the functions required for drawing a worksheet
 * in the screen.
 *
 * The viewport_display() function is invoked by the main loop when it wants
 * to display the worksheet on the screen. The other functions are invoked by
 * commands such as CURSOR, PREC, etc, that modify the way in which the
 * worksheet is displayed. The display does NOT need to be updated immediately
 * after these functions have been invoked; the new settings will come into
 * effect the next time viewport_display() is invoked.
 */

#include <stdio.h>
#include "sheet1002.h"
#include <stdlib.h>

/*
 * Print the current viewing window to the screen.
 *
 * Input:
 *   term_cols - the width of the space in which to display the window, in characters
 *   term_rows - the height of the space in which to display the window, in characters
 */
void viewport_display(int term_cols, int term_rows)
{
    if (viewport_get_worksheet() == NULL)
        return;

    const int max_rows = term_rows;
    const int max_columns = term_cols / viewport_get_worksheet()->cell_width;

    char column_width_string[INTEGER_MAX_BUFF];
    itoa(viewport_get_worksheet()->cell_width, column_width_string, 10);

    char string_format[INTEGER_MAX_BUFF + 2];
    string_copy(string_format, "%", column_width_string, "s", NULL);

    char char_format[INTEGER_MAX_BUFF + 3];
    string_copy(char_format, "%-", column_width_string, "c", NULL);

    char integer_format[INTEGER_MAX_BUFF + 3];
    string_copy(integer_format, "%-", column_width_string, "d", NULL);

    for (int row = 0; row < max_rows; row++)
    {
        for (int column = 0; column < max_columns; column++)
        {
            if (row == 0)
            {
                if (column == 0)
                    printf(string_format, "");
                else if (column + viewport_get_worksheet()->view_column_offset <= viewport_get_worksheet()->cols)
                    printf(char_format, 'A' + column - 1 + viewport_get_worksheet()->view_column_offset);
            }
            else if (row + viewport_get_worksheet()->view_row_offset <= viewport_get_worksheet()->rows)
            {
                if (column == 0)
                    printf(integer_format, row + viewport_get_worksheet()->view_row_offset);
                else if (column + viewport_get_worksheet()->view_column_offset <= viewport_get_worksheet()->cols)
                {
                    char string_to_print[MAX_WORD];
                    ws_cell_as_string(viewport_get_worksheet(), column + viewport_get_worksheet()->view_column_offset,
                                      row + viewport_get_worksheet()->view_row_offset,
                                      viewport_get_worksheet()->cell_width, viewport_get_worksheet()->float_precision,
                                      string_to_print);

                    printf(string_format, string_to_print);
                }
            }
        }

        printf("\n");
    }
}

/*
 * Get the precisiion with which floating-point numbers will be displayed.
 *
 * Returns:
 *   the number of decimal places that will be shown
 */
int viewport_get_cellprec(void)
{
    return viewport_get_worksheet()->float_precision;
}

/*
 * Set the precision with which floating-point numbers will be displayed.
 *
 * Input:
 *   prec - the number of decimal places to show
 */
void viewport_set_cellprec(int prec)
{
    viewport_get_worksheet()->float_precision = prec;
}

/*
 * Set the width in which the cells will be displayed.
 *
 * Input:
 *   width - the number of characters in each cell to be displayed
 */
void viewport_set_cellwidth(int width)
{
    viewport_get_worksheet()->cell_width = width;
}

/*
 * Set the position of the cursor.
 *
 * Input:
 *   col - the column number
 *   row - the row number
 */
void viewport_set_cursor(int col, int row)
{
    viewport_get_worksheet()->view_column_offset = col - 1;
    viewport_get_worksheet()->view_row_offset = row - 1;
}

/*
 * Get a pointer to the worksheet currently being displayed.
 *
 * Returns:
 *   a pointer to the WORKSHEET structure currently displayed
 *   NULL, if there is no worksheet displayed
 */
WORKSHEET *viewport_get_worksheet(void)
{
    return worksheet;
}

/*
 * Set the worksheet to be displayed.
 *
 * Input:
 *   ws - a pointer to the worksheet to be displayed
 */
void viewport_set_worksheet(WORKSHEET *ws)
{
    worksheet = ws;
}
