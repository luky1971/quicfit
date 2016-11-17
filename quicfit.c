/*
 * Copyright 2016 Ahnaf Siddiqui
 */

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "quicfit.h"


void fit2exp(const double x[], const double y[], int nx,
             double abegin, double da, int na,
             double bbegin, double db, int nb,
             double *a, double *b) {
    double min_cost = FLT_MAX;
    double best_a = abegin;
    double best_b = bbegin;

    // calculate cost for all pairs of a and b in given ranges
    for (int acount = 0; acount < na; ++acount) {
        for (int bcount = 0; bcount < nb; ++bcount) {
            double cost = 0.0;

            double ai = abegin + acount * da;
            double bi = bbegin + bcount * db;

            // calculate cost for this pair of a and b
            // as sum of squared residuals over range of x
            for (int xi = 0; xi < nx; ++xi) {
                double pred = (1 - ai) * exp(-x[xi]/bi) + ai;
                double residual = pred - y[xi];

                cost += residual * residual;
            }

            // check if this is the lowest-cost pair of a and b so far
            if (cost < min_cost) {
                min_cost = cost;
                best_a = ai;
                best_b = bi;
            }
        }
    }

    *a = best_a;
    *b = best_b;
}


void printDesc() {
    printf("quicfit quickly fits a given set of points to a curve.\n");
    printf("Currently, it can only fit to the two-parameter function (1-a)*exp(-x/b) + a\n\n");

    printf("Usage: quicfit [OPTIONS] < FILE\n\n");
    printf("The input file should have two columns of values (x y).\n");
    printf("Lines starting with a non-numerical value are automatically ignored.\n\n");

    printf("Options:\n\n");

    printf("-a1\t(double) The first value for parameter a that should be tested during fitting.\n");
    printf("-da\t(double) The interval between tested a values during fitting.\n");
    printf("-na\t(int) The number of a values to test during fitting.\n\n");

    printf("-b1\t(double) The first value for parameter b that should be tested during fitting.\n");
    printf("-db\t(double) The interval between tested b values during fitting.\n");
    printf("-nb\t(int) The number of b values to test during fitting.\n\n");
}


int main(int argc, char *argv[]) {
    // parameters
    double abegin = 0.0;
    double da = 0.1;
    int na = 11;

    double bbegin = 1.0;
    double db = 1.0;
    int nb = 1000;

    // parse arguments
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] == '-') {
            // process flags that don't require an argument
            char *flag = argv[i] + 1;

            if (*flag) {
                // -h help
                if (strcmp(flag, "h") == 0) {
                    printDesc();
                    return 0;
                }
            }

            // process flags that require an argument
            if (i + 1 < argc) {
                // the argument for the current flag
                char *arg = argv[i+1];

                // a values
                if (strcmp(argv[i], "-a1") == 0) {
                    abegin = atof(arg);
                }
                else if (strcmp(argv[i], "-da") == 0) {
                    da = atof(arg);
                }
                else if (strcmp(argv[i], "-na") == 0) {
                    na = atoi(arg);
                }

                // b values
                else if (strcmp(argv[i], "-b1") == 0) {
                    bbegin = atof(arg);
                }
                else if (strcmp(argv[i], "-db") == 0) {
                    db = atof(arg);
                }
                else if (strcmp(argv[i], "-nb") == 0) {
                    nb = atoi(arg);
                }
            }
        }
    }


    // DEBUG
    // printf("%f\n", abegin);
    // printf("%f\n", da);
    // printf("%d\n", na);
    // printf("%f\n", bbegin);
    // printf("%f\n", db);
    // printf("%d\n", nb);

    // read stream input
    char buffer[BUFSIZ];

    while (fgets(buffer, sizeof buffer, stdin)) {
        printf("%s", buffer);
    }

    return 0;
}
