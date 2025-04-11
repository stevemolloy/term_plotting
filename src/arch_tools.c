#include <assert.h>
#include <time.h>

#include "arch_tools.h"
#include "sdm_lib.h"

char *parse_arch_file(const char* data_filename, DateTimeStringArray *date_time_array, DoubleArray *val_array) {
    char *buffer = sdm_read_entire_file(data_filename);

    sdm_string_view raw_data = sdm_cstr_as_sv(buffer);

    skip_header(&raw_data);

    sdm_string_view line = sdm_sv_pop_by_delim(&raw_data, '\n');
    while (line.length > 0) {
        parse_line(&line, date_time_array, val_array);
        line = sdm_sv_pop_line(&raw_data);
    }

    assert(date_time_array->length == val_array->length);
    
    return buffer;
}

void parse_line(sdm_string_view *line, DateTimeStringArray *date_time_array, DoubleArray *val_array) {
    SDM_ARRAY_PUSH((*date_time_array), sdm_sv_pop_by_delim(line, ','));
    sdm_sv_trim(line);
    SDM_ARRAY_PUSH((*val_array), sdm_sv_pop_double(line));
}

void skip_header(sdm_string_view *arch_sv) {
    sdm_sv_pop_by_delim(arch_sv, '\n');
    sdm_sv_pop_by_delim(arch_sv, '\n');
}

void skip_chars(char **cursor) {
    while (**cursor == '0' || **cursor == '-') (*cursor)++;
}

int get_next_int(char **cursor) {
    int val = strtol(*cursor, cursor, 0);
    skip_chars(cursor);
    return val;
}

void get_week_number(sdm_string_view datetimesv, char *weeknum) {
    char *cursor = datetimesv.data;

    struct tm date_time;
    memset(&date_time, 0, sizeof(date_time));
    date_time.tm_year = get_next_int(&cursor) - 1900;
    date_time.tm_mon = get_next_int(&cursor) - 1;
    date_time.tm_mday = get_next_int(&cursor);
    mktime(&date_time);

    strftime(weeknum, 3, "%V", &date_time);
}

