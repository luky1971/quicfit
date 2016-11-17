/*
 * Copyright 2016 Ahnaf Siddiqui
 */

#include <float.h>
#include <math.h>
#include <stdio.h>
#include "quicfit.h"


void fit2exp(const double x[], const double y[],
             double xbegin, double dx, int nx,
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
                double x = xbegin + xi * dt;
                double pred = (1 - ai) * exp(-x/bi) + ai;
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

    printf("Options:\n\n");

    printf("Input file with two columns of values (x y).\n\n");

    printf("-a1\t(double) The first value for parameter a that should be tested during fitting.\n");
    printf("-da\t(double) The interval between tested a values during fitting.\n");
    printf("-na\t(int) The number of a values to test during fitting.\n\n");

    printf("-b1\t(double) The first value for parameter b that should be tested during fitting.\n");
    printf("-db\t(double) The interval between tested b values during fitting.\n");
    printf("-nb\t(int) The number of b values to test during fitting.\n\n");
}


int main(int argc, char *argv[]) {
    // if no arguments provided, print program description
    if (argc <= 1) {
        printDesc();
        return 0;
    }

    // parameters
    double abegin = 0.0;
    double da = 0.1;
    int na = 11;

    double bbegin = 1.0;
    double db = 1.0;
    int nb = 1000;

    // parse arguments
    // for (int i = 1; i < argc; ++i) {
    //     if (argv[i][0] == '-') {
    //         // process flags that require an argument
    //         if (i + 1 < argc) {
    //             char *arg = argv[i+1];
    //
    //             if (strcmp(argv[i], "-a1") == 0) {
    //                 //
    //             }
    //         }
    //     }
    // }


    // read autocorrelation data from file
    // FILE *input = fopen(argv[1], "r");

    // read function input
    

    return 0;
}
