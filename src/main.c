#include <stdio.h>
#include <stdlib.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"
#undef SDM_LIB_IMPLEMENTATION
#include "arch_tools.h"
#include "lib.h"

#define VERT_PTS 15
#define HORIZ_PTS 200

void usage(const char *programname) {
    fprintf(stderr, "USAGE:\n%s <filename>\n", programname);
}

int main(int argc, char *argv[]) {
    const char *filename;
    const char *programname = sdm_shift_args(&argc, &argv);

    if (argc == 0) {
        fprintf(stderr, "ERROR: No filename provided\n");
        usage(programname);
        return 1;
    }

    while (argc > 0) {
        filename = sdm_shift_args(&argc, &argv);
    }

    DateTimeStringArray dt_strings = {0};
    DoubleArray values = {0};
    char *buffer = parse_arch_file(filename, &dt_strings, &values);

    printf("\n\n%s\n\n", filename);
    plot_hist(values.data, values.length, VERT_PTS, HORIZ_PTS);
    printf("\n\n");

    free(dt_strings.data);
    free(values.data);
    free(buffer);

    return 0;
}

