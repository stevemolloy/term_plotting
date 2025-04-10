#include <stdio.h>

#include "lib.h"

int main(void) {
    double data[] = 
        {10, 6, 0, 14, 7, 8, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};
    size_t num_pts = sizeof(data) / sizeof(data[0]);

    printf("\n\n");
    plot_hist(data, num_pts);
    printf("\n\n");

    return 0;
}

