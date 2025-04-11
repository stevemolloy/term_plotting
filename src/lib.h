#ifndef _LIB_H
#define _LIB_H

#include <stdlib.h>

typedef enum {
    DATA_AG_AV,
    DATA_AG_MIN,
    DATA_AG_MAX,
} DataAggregate;

void plot_hist(double *data, size_t num_pts, size_t y_pts, size_t max_x_pts, DataAggregate aggregator);
double sum_terms(double *data, size_t start, size_t *sum_length, size_t total_num_pts);
double max(double *data, size_t num_pts);
double min(double *data, size_t num_pts);
char *agg_string(DataAggregate agg);

#endif // _LIB_H
