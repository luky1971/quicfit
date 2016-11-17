/*
 * Copyright 2016 Ahnaf Siddiqui
 */

void fit2exp(const double x[], const double y[], 
             double xbegin, double dx, int nx, 
             double abegin, double da, int na,
             double bbegin, double db, int nb, 
             double *a, double *b);
/* Performs a single exponential fit of the given function y = f(x)
 * from x = xbegin to x = xbegin + dx * (nx - 1).
 * The given array of y values should be size nx,
 * where y[0] is the function value at x = xbegin, 
 * and subsequent values are at the previous x plus dx.
 * The given function is fit to (1-a)*exp(-x/b) + a
 * by optimizing a and b to minimize the cost function of regression.
 * The optimization is performed by a global search from a = abegin to a = abegin + (na - 1) * da using increments of da,
 * and from b = bbegin to b = bbegin + (nb - 1) * db using increments of db.
 * Stores the optimized a in *a and the optimized b in *b.
 */
