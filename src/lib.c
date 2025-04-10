#include <float.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib.h"

#define GRAPH_MARK 'X'
#define VERT_PTS 15

void plot_hist(double *data, size_t num_pts) {
    double min_val = min(data, num_pts);
    double max_val = max(data, num_pts);
    double step = (max_val - min_val) / (VERT_PTS - 1);

    double *lims = malloc((VERT_PTS+1) * sizeof(double));
    double low  = min_val - step/2.0;
    for (size_t i=0; i<VERT_PTS; i++)
        lims[i] = low + i * step;
    lims[VERT_PTS] = low + VERT_PTS*step;

    for (int i=VERT_PTS; i>0; i--) {
        if (i==VERT_PTS)
            printf("%+0.3e ^", lims[i]-step/2.0);
        else if (i==1)
            printf("%+0.3e -", lims[i]-step/2.0);
        else
            printf("           |");

        for (size_t j=0; j<num_pts; j++) {
            if (lims[i] > data[j] && data[j] >= lims[i-1])
                putchar(GRAPH_MARK);
            else if (i!=1) putchar(' ');
            else if (j!=(num_pts-1) )putchar('-');
            else putchar('>');
        }
        putchar('\n');
    }

    free(lims);
}

double max(double *data, size_t num_pts) {
    double max_val = DBL_MIN;
    for (size_t i=0; i< num_pts; i++) 
        if (data[i] > max_val) max_val = data[i];
    return max_val;
}

double min(double *data, size_t num_pts) {
    double min_val = DBL_MAX;
    for (size_t i=0; i< num_pts; i++)
        if (data[i] < min_val) min_val = data[i];
    return min_val;
}

