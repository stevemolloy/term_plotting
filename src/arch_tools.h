#ifndef _ARCH_TOOLS_H
#define _ARCH_TOOLS_H

#include <stdlib.h>

#define SDM_MALLOC malloc
#define SDM_REALLOC realloc
#include "sdm_lib.h"

typedef struct {
    sdm_string_view *data;
    size_t length;
    size_t capacity;
} DateTimeStringArray;

typedef struct {
    double *data;
    size_t length;
    size_t capacity;
} DoubleArray;

char *parse_arch_file(const char* data_filename, DateTimeStringArray *date_time_array, DoubleArray *val_array);
void skip_header(sdm_string_view *arch_sv);
void parse_line(sdm_string_view *line, DateTimeStringArray *date_time_array, DoubleArray *val_array);
void skip_chars(char **cursor);
int get_next_int(char **cursor);
void get_week_number(sdm_string_view datetimesv, char *weeknum);

#endif // _ARCH_TOOLS_H

