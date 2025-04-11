#include <assert.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib.h"

#define GRAPH_MARK 'X'

#define BETWEEN(x, lo, hi) ((hi) > (x) && (x) >= (lo))

void plot_hist(double *data, size_t num_pts, size_t y_pts, size_t max_x_pts, DataAggregate aggregator) {
    double *data_to_plot;
    size_t plot_N;
    if (num_pts > max_x_pts) {
        size_t window_size = num_pts / max_x_pts;
        if (num_pts % max_x_pts > 0) window_size++;
        printf("Number of data points (%zu) exceeds the desired maximum (%zu).\nAggregating data with \"%s\" function over a window of %zu.\n\n",
               num_pts, max_x_pts, agg_string(aggregator), window_size);

        data_to_plot = malloc(max_x_pts * sizeof(double));
        for (size_t i=0; i<max_x_pts; i++) {
            if (i*window_size >= num_pts) {
                max_x_pts = i;
                break;
            }
            size_t temp_num = window_size;
            switch (aggregator) {
                case DATA_AG_AV:
                    data_to_plot[i] = sum_terms(data, i*window_size, &temp_num, num_pts) / temp_num;
                    break;
                case DATA_AG_MIN:
                    if (num_pts - (i*window_size) < temp_num) temp_num = num_pts - (i*window_size);
                    data_to_plot[i] = min(data+(i*window_size), temp_num);
                    break;
                case DATA_AG_MAX:
                    if (num_pts - (i*window_size) < temp_num) temp_num = num_pts - (i*window_size);
                    data_to_plot[i] = max(data+(i*window_size), temp_num);
                    break;
            }
        }
        plot_N = max_x_pts;
    } else {
        data_to_plot = data;
        plot_N = num_pts;
    }

    double min_val = min(data_to_plot , plot_N);
    double max_val = max(data_to_plot , plot_N);
    double step = (max_val - min_val) / (y_pts - 1);

    double *lims = malloc((y_pts+1) * sizeof(double));
    double low  = min_val - step/2.0;
    for (size_t i=0; i<y_pts; i++)
        lims[i] = low + i * step;
    lims[y_pts] = low + y_pts*step;

    for (int i=y_pts; i>0; i--) {
        if (i==(int)y_pts)
            printf("%+0.2e ^", lims[i]-step/2.0);
        else if (i==1)
            printf("%+0.2e -", lims[i]-step/2.0);
        else
            printf("          |");

        for (size_t j=0; j<plot_N; j++) {
            if (BETWEEN(data_to_plot[j], lims[i-1], lims[i]))
                putchar(GRAPH_MARK);
            else if (i!=1) putchar(' ');
            else if (j!=(plot_N-1) )putchar('-');
            else putchar('>');
        }
        putchar('\n');
    }

    free(lims);
    if (data != data_to_plot) {
        free(data_to_plot);
    }
}

double sum_terms(double *data, size_t start, size_t *sum_length, size_t total_num_pts) {

    assert(start < total_num_pts);
    double sum = 0.0;

    size_t i;
    for (i=start; i<total_num_pts && i<(start+*sum_length); i++)
        sum += data[i];

    *sum_length = i - start;

    return sum;
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

char *agg_string(DataAggregate agg) {
    switch (agg) {
        case (DATA_AG_AV): return "AVERAGE";
        case (DATA_AG_MAX): return "MAX";
        case (DATA_AG_MIN): return "MIN";
    }
}

