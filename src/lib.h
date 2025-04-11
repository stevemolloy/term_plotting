#ifndef _LIB_H
#define _LIB_H

#include <stdlib.h>

void plot_hist(double *data, size_t num_pts, size_t y_pts, size_t max_x_pts);
double sum_terms(double *data, size_t start, size_t *sum_length, size_t total_num_pts);
double max(double *data, size_t num_pts);
double min(double *data, size_t num_pts);

#endif // _LIB_H
