#include <stdio.h>
#include <stdlib.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"
#undef SDM_LIB_IMPLEMENTATION
#include "arch_tools.h"
#include "lib.h"

#define VERT_PTS 15
#define HORIZ_PTS 100

int main(void) {
    const char *filename = "./data/brm40_short.csv";

    DateTimeStringArray dt_strings = {0};
    DoubleArray values = {0};
    char *buffer = parse_arch_file(filename, &dt_strings, &values);

    printf("\n\n");
    printf("%s\n\n", filename);
    plot_hist(values.data, values.length, VERT_PTS);
    plot_hist(values.data, values.length, VERT_PTS, HORIZ_PTS);
    printf("\n\n");

    free(dt_strings.data);
    free(values.data);
    free(buffer);

    return 0;
}

