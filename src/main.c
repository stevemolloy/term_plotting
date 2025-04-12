#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SDM_LIB_IMPLEMENTATION
#include "sdm_lib.h"
#undef SDM_LIB_IMPLEMENTATION
#include "arch_tools.h"
#include "lib.h"

#define VERT_PTS_DEFAULT 30
#define HORIZ_PTS_DEFAULT 200

void usage(const char *programname) {
    fprintf(stderr, "USAGE:\n%s <filename> [-x <horiz_pts>] [-y <vert_pts>] [--average|--max|--min]\n", programname);
}

void help(const char*programname) {
    fprintf(stdout, "Plot time-series data from a CSV file\n");
    fprintf(stdout, "Assumes the file is organised so that the first two lines are ignored,\n");
    fprintf(stdout, "and that the data is in the second column.\n");
    fprintf(stdout, "\n");
    usage(programname);
    fprintf(stdout, "\n");
    fprintf(stdout, "\"-x\" -- The maximum number of points allowed horizontally. Default = %d\n", HORIZ_PTS_DEFAULT);
    fprintf(stdout, "        Datasets that exceed this will be aggregated in chunks according to the aggregator function\n");
    fprintf(stdout, "\"-y\" -- The number of points on the vertical axis. Default = %d\n", VERT_PTS_DEFAULT);
    fprintf(stdout, "--average|--max|--min -- The aggregator function to use when the number of points exceeds the maximum.\n");
    fprintf(stdout, "                         --average: plot the average of the chunk. (DEFAULT)\n");
    fprintf(stdout, "                         --min:  plot the minimum value of each chunk.\n");
    fprintf(stdout, "                         --max:  plot the maximum value of each chunk.\n");
}

int main(int argc, char *argv[]) {
    const char *programname = sdm_shift_args(&argc, &argv);

    if (argc == 0) {
        fprintf(stderr, "ERROR: No filename provided\n");
        usage(programname);
        return 1;
    }

    const char *filename = NULL;
    size_t horiz_pts = HORIZ_PTS_DEFAULT;
    size_t vert_pts = VERT_PTS_DEFAULT;
    DataAggregate aggregator = DATA_AG_AV;
    while (argc > 0) {
        char *arg = sdm_shift_args(&argc, &argv);

        if (strcmp(arg, "-h") == 0 || strcmp(arg, "--help") == 0) {
            help(programname);
            return 0;
        }
        else if (strcmp(arg, "-x")==0) {
            if (argc == 0) {
                fprintf(stderr, "ERROR: An integer value is expected after \"-x\"");
                usage(programname);
                return 1;
            }
            horiz_pts = strtol(sdm_shift_args(&argc, &argv), NULL, 0);
        }
        else if (strcmp(arg, "-y")==0) {
            if (argc == 0) {
                fprintf(stderr, "ERROR: An integer value is expected after \"-y\"");
                usage(programname);
                return 1;
            }
            vert_pts = strtol(sdm_shift_args(&argc, &argv), NULL, 0);
        }
        else if (strcmp(arg, "--max") == 0) {
            aggregator = DATA_AG_MAX;
        }
        else if (strcmp(arg, "--min") == 0) {
            aggregator = DATA_AG_MIN;
        }
        else if (strcmp(arg, "--average") == 0) {
            aggregator = DATA_AG_AV;
        }
        else {
            filename = arg;
        }
    }

    if (filename == NULL) {
        fprintf(stderr, "ERROR: No file was provided\n");
        usage(programname);
        return 1;
    }

    DateTimeStringArray dt_strings = {0};
    DoubleArray values = {0};
    char *buffer = parse_arch_file(filename, &dt_strings, &values);

    printf("\n%s\n", filename);
    plot_hist(values.data, values.length, vert_pts, horiz_pts, aggregator);
    printf("\n\n");

    free(dt_strings.data);
    free(values.data);
    free(buffer);

    return 0;
}

