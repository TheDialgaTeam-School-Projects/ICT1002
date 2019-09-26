/*
 * ICT1002 Assignment 2, 2018-19 Trimester 1.
 *
 * This file contains the definitions and function prototypes for all of
 * features of the ICT1002 spreadsheet.
 */

#ifndef _SHEET1002_H
#define _SHEET1002_H

#include <stdio.h>

/* the maximum number of characters we expect in a line of input */
#define MAX_INPUT   80

/* the maximum number of characters permitted in a line of output */
#define MAX_OUTPUT  80

/* the maximum length of the text in a cell */
#define MAX_WORD    32

/* the dimensions of the terminal  (here assumed to be a standard 80x24 terminal */
#define TERMINAL_COLS 80
#define TERMINAL_ROWS 24

/* return codes for ws_guess_data_type */
#define WS_DATA_TYPE_ILLEGAL -1
#define WS_DATA_TYPE_TEXT     1
#define WS_DATA_TYPE_FLOAT    2

/* the default column width */
#define DEFAULT_CELL_WIDTH 5

/* the default floating point precision */
#define DEFAULT_FLOAT_PRECISION 0

/* the maximum number of columns in a spreadsheet */
#define MAX_COLUMNS 26

/* integer max buffer */
#define INTEGER_MAX_BUFF (sizeof(int) * 8 + 1)

#define BSIZE 800

/* a structure for representing a worksheet */
typedef struct
{
    int cols;
    int rows;

    int view_column_offset;
    int view_row_offset;

    int cell_width;
    int float_precision;

    char ***sheet;
} WORKSHEET;

/* Current worksheet */
WORKSHEET *worksheet;

/* functions defined in main.c */
int compare_token(const char *token1, const char *token2);
int lscanf(const char *format, ...);

/* functions defined in commands.c */
int do_command(const char *command, const char *arg1, const char *arg2, char *output);
void do_avg(const char *arg1, const char *arg2, char *output);
void do_cursor(const char *arg1, char *output);
void do_load(const char *arg1, char *output);
void do_new(const char *arg1, const char *arg2, char *output);
void do_prec(const char *arg1, char *output);
void do_save(const char *arg1, char *output);
void do_set(const char *arg1, const char *arg2, char *output);
void do_sum(const char *arg1, const char *arg2, char *output);
void do_width(const char *arg1, char *output);
char *string_substring(char *dest, const char *src, const int offset);

/* functions defined in viewport.c */
void viewport_display(int term_cols, int term_rows);
int viewport_get_cellprec(void);
void viewport_set_cellprec(int prec);
void viewport_set_cellwidth(int width);
void viewport_set_cursor(int col, int row);
WORKSHEET *viewport_get_worksheet(void);
void viewport_set_worksheet(WORKSHEET *ws);

/* functions defined in worksheet.c */
float ws_cell_as_float(WORKSHEET *ws, int col, int row);
char *ws_cell_as_string(WORKSHEET *ws, int col, int row, int width, int prec, char *buf);
int ws_guess_data_type(const char *value);
void ws_free(WORKSHEET *ws);
WORKSHEET *ws_new(int cols, int rows);
int ws_read_csv(WORKSHEET *ws, FILE *f);
void ws_set(WORKSHEET *ws, int col, int row, const char *value);
int ws_write_csv(WORKSHEET *ws, FILE *f);
char *string_copy(char *dest, ...);

#endif
