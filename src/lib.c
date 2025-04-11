#include <assert.h>
#include <float.h>
#include <stdlib.h>
#include <stdio.h>

#include "lib.h"

#define GRAPH_MARK 'X'

void plot_hist(double *data, size_t num_pts, size_t y_pts, size_t max_x_pts) {
    double *data_to_plot;
    size_t pts_to_plot;
    if (num_pts > max_x_pts) {
        size_t window_size = num_pts / max_x_pts;
        if (num_pts % max_x_pts > 0) window_size++;
        data_to_plot = malloc(max_x_pts * sizeof(double));
        for (size_t i=0; i<max_x_pts; i++) {
            size_t temp_num = window_size;
            if (i*window_size >= num_pts) {
                max_x_pts = i;
                break;
            }
            double sum_data = sum_terms(data, i*window_size, &temp_num, num_pts);
            data_to_plot[i] = sum_data / temp_num;
        }
        pts_to_plot = max_x_pts;
    } else {
        data_to_plot = data;
        pts_to_plot = num_pts;
    }

    double min_val = min(data_to_plot , pts_to_plot);
    double max_val = max(data_to_plot , pts_to_plot);
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

        for (size_t j=0; j<pts_to_plot; j++) {
            if (lims[i] > data_to_plot[j] && data_to_plot[j] >= lims[i-1])
                putchar(GRAPH_MARK);
            else if (i!=1) putchar(' ');
            else if (j!=(pts_to_plot-1) )putchar('-');
            else putchar('>');
        }
        putchar('\n');
    }

    free(lims);
    if (num_pts > max_x_pts) {
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

