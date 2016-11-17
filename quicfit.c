/*
 * Copyright 2016 Ahnaf Siddiqui
 */

#include <ctype.h>
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
    fprintf(stderr, "quicfit quickly fits a given set of points to a curve.\n");
    fprintf(stderr, "Currently, it can only fit to the two-parameter function (1-a)*exp(-x/b) + a\n\n");

    fprintf(stderr, "Usage: quicfit [OPTIONS] < FILE\n\n");
    fprintf(stderr, "The input file should have two columns of values (x y).\n");
    fprintf(stderr, "Lines starting with a non-numerical value (besides whitespace and decimal point) "\
                    "are automatically ignored.\n\n");

    fprintf(stderr, "Options:\n\n");

    fprintf(stderr, "-a1\t(double) The first value for parameter a that should be tested during fitting.\n");
    fprintf(stderr, "-da\t(double) The interval between tested a values during fitting.\n");
    fprintf(stderr, "-na\t(int) The number of a values to test during fitting.\n\n");

    fprintf(stderr, "-b1\t(double) The first value for parameter b that should be tested during fitting.\n");
    fprintf(stderr, "-db\t(double) The interval between tested b values during fitting.\n");
    fprintf(stderr, "-nb\t(int) The number of b values to test during fitting.\n\n");

    fprintf(stderr, "-delim\t(string) The set of characters to use as delimiters between values "\
                    "(x and y) in a row of the input file.\n");
}


int main(int argc, char *argv[]) {
    // parameters
    double abegin = 0.0;
    double da = 0.1;
    int na = 11;

    double bbegin = 1.0;
    double db = 1.0;
    int nb = 1000;

    // the delimiter between x and y in a row of the input file
    const char *delims = " \t";

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

                // input delimiter
                else if (strcmp(argv[i], "-delim") == 0) {
                    delims = arg;
                }
            }
        }
    }


    // DEBUG
    // fprintf(stderr, "%f\n", abegin);
    // fprintf(stderr, "%f\n", da);
    // fprintf(stderr, "%d\n", na);
    // fprintf(stderr, "%f\n", bbegin);
    // fprintf(stderr, "%f\n", db);
    // fprintf(stderr, "%d\n", nb);

    char buffer[BUFSIZ];
    double *x = NULL;
    double *y = NULL;
    int nx = 0;
    int alloc_size = 100;

    // allocate memory for x and y values of input function
    x = (double*)malloc(sizeof(double) * alloc_size);
    y = (double*)malloc(sizeof(double) * alloc_size);

    if (!x || !y) {
        fputs("Memory error. Aborting.\n", stderr);
        return 1;
    }

    // read all the lines from the input stream
    while (fgets(buffer, sizeof(buffer), stdin)) {
        if (strlen(buffer) > 0) {
            // Only process the current line if the first character
            // is either a numerical digit, whitespace, or a decimal point.
            // This is to allow for comments/other text in the input file.
            if (isdigit(buffer[0]) || isspace(buffer[0]) || buffer[0] == '.') {
                // get x and y from the current line of the input stream
                const char *xstr = strtok(buffer, delims);
                const char *ystr = NULL;
                if (xstr) ystr = strtok(NULL, delims);

                if (xstr && ystr) {
                    if (nx >= alloc_size) {
                        // reallocate memory if needed
                        alloc_size *= 2;
                        x = (double*)realloc(x, sizeof(double) * alloc_size);
                        y = (double*)realloc(y, sizeof(double) * alloc_size);
                    }

                    x[nx] = atof(xstr);
                    y[nx] = atof(ystr);

                    ++nx;
                }
            }
        }
    }

    // DEBUG
    // for (int i = 0; i < nx; ++i) {
    //     printf("%f\t%f\n", x[i], y[i]);
    // }

    // fit parameters and output results
    double a, b;
    fit2exp(x, y, nx, abegin, da, na, bbegin, db, nb, &a, &b);

    printf("%f\n%f\n", a, b);


    free(x);
    free(y);

    return 0;
}
