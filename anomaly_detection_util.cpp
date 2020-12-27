// Oz Browner 316482355

#include <math.h>
#include "anomaly_detection_util.h"

/**
 * avg returns mean of x's floats.
 * @param x - array of floats.
 * @param size - x's length.
 * @return the average.
 */
float avg(float* x, int size) {
    // if size is proper calculates mean of x floats.
    if (size <= 0) {
        return 0;
    }
    // sum - sums all floats in x, then divided by the size.
    float sum = 0;
    for (int i = 0; i < size; ++i) {
        sum = sum + x[i];
    }
    return sum / size;
}

/**
 * var returns the variance of X and Y
 * @param x array of floats.
 * @param size - x's length.
 * @return  the variance of X and Y
 */
float var(float* x, int size) {
    // var is splitted to 2 parts, avg1 and avg2 square according to var equation.
    float avg1, avg2;
    float* x_square = new float[size];
    // creates new array of x's value square.
    for (int i = 0; i < size; ++i) {
        x_square[i] = x[i] * x[i];
    }
    avg1 = avg(x_square, size);
    delete[] x_square;
    avg2 = avg(x, size);
    return avg1 - (avg2 * avg2);
}

/**
 * cov calculates the co-variance of x and y float arrays.
 * @param x
 * @param y
 * @param size
 * @return covariance
 */
float cov(float* x, float* y, int size) {
    // calculates the cov by using x and y means and new arrays.
    float xMean, yMean;
    float* newX = new float[size];
    float* newY = new float[size];
    float* newXY = new float[size];
    xMean = avg(x, size);
    yMean = avg(y, size);
    for (int i = 0; i < size; ++i) {
        newX[i] = x[i] - xMean;
        newY[i] = y[i] - yMean;
        newXY[i] = newX[i] * newY[i];
    }
    float cov = avg(newXY, size);
    delete[] newX;
    delete[] newY;
    delete[] newXY;
    return cov;
}

/**
 * pearson - -1 to 1 - returns the Pearson correlation coefficient of X and Y
 * @param x
 * @param y
 * @param size
 * @return the Pearson correlation coefficient of X and Y
 */
float pearson(float* x, float* y, int size) {
    // calculates by using x and y co-variance and standard deviations.
    float cova, sdX, sdY;
    sdX = sqrt(var(x, size));
    sdY = sqrt(var(y, size));
    cova = cov(x, y, size);
    return cova / (sdX * sdY);
}

/**
 * linear_reg performs a linear regression and return s the line equation
 * @param points.
 * @param size - points number.
 * @return appropriate Line object.
 */
Line linear_reg(Point** points, int size) {
    // a - line slope, b - line meet with y axe.
    float a, b;
    float* x = new float[size];
    float* y = new float[size];
    for (int i = 0; i < size; ++i) {
        x[i] = points[i]->x;
        y[i] = points[i]->y;
    }
    float xVar = var(x, size);
    a = cov(x, y, size) / xVar;
    b = avg(y, size) - a * avg(x, size);
    delete[] x;
    delete[] y;
    return Line(a, b);
}

/**
 * returns the deviation between point p and the line equation of the points
 * @param p - a point
 * @param points - points which creates line equation
 * @param size - number of points.
 * @return the deviation
 */
float dev(Point p,Point** points, int size) {
    Line l = linear_reg(points, size);
    return dev(p, l);
}

/**
 * returns the deviation between point p and the line equation of the points
 * @param p - a point
 * @param l - a line
 * @return the deviation
 */
float dev(Point p,Line l) {
    float lineY = l.a * p.x + l.b;
    return fabs(lineY - p.y);
}

