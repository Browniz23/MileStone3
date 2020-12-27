#include "minCircle.h"

// distance between 2 points
float distance(Point p1, Point p2) {
    return sqrt(pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2));
}

// gets a,b,c floats and pos bool, and return answer of quadratic equation - bigger 'x' if pos is true, the smaller 'x'
// if pos is false. must get proper values so determinanta wont be negative.
float quadratic(float a, float b, float c, bool pos) {
    float preDet = b*b - 4*a*c;
    float x1 = (-b + sqrt(preDet)) / (2*a);
    float x2 = (-b - sqrt(preDet)) / (2*a);
    if (pos) {
        return x1>x2 ? x1:x2;
    }
    return x1>x2 ? x2:x1;
}

// return true if point is inside circle
bool isInCircle(Circle c, Point p) {
    return (distance(c.center, p) <= c.radius);
}

// return circle that has 2 points on border.
Circle twoPoints(Point p1, Point p2) {
    // 2 points are on same diameter
    float x = (p1.x + p2.x) / 2;
    float y = (p1.y + p2.y) / 2;
    float r = sqrt(pow(p1.x-p2.x,2) + pow(p1.y-p2.y,2)) / 2;                        //todo change to distance func
    return Circle(Point(x,y), r);
}

// return circle that has 3 points on border.
Circle threePoints(Point p1, Point p2, Point p3) {
    // e's are the triangle's edges created by points.
    float e12 = distance(p1,p2);
    float e13 = distance(p1,p3);
    float e23 = distance(p2,p3);
    float angle3 = acos((pow(e13, 2) + pow(e23, 2) - pow(e12, 2)) / (2 * e13 * e23));
    float r = e12 / (2 * sin(angle3));
    // me12 - the middle ankh from e12 to circle's center. mx12,my12- middle point on e12 the ankh goes from.
    float me12 = sqrt(r*r - pow(e12/2, 2));
    float mx12 = (p1.x + p2.x) / 2;
    float my12 = (p1.y + p2.y) / 2;
    // m - the slope of e12, and then changed to slope of me12.
    float m;
    if (p1.x - p2.x == 0)
        m = 0;
    else {
        m = (p1.y-p2.y) / (p1.x-p2.x);
        m = -1 / m;
    }
    // creating line equation of me12.
    float n = my12 - m * mx12;
    float x1, x2;
    float a = 1+m*m, b = -2*mx12 + 2*m*n -2*m*my12, c = mx12*mx12 + n*n -2*n*my12 + my12*my12 -me12*me12;
    // x1,x2 are 2 possible 'x' center of circle, because in the right distance from middle of e12.
    x1 = quadratic(a, b, c, true);
    x2 = quadratic(a, b, c, false);
    float y1 = x1*m + n;
    Point center1(x1,y1);
    float y2 = x2*m + n;
    Point center2(x2,y2);
    // check which point is the 'true' center circle and which is the 'leftover' from quadratic.
    if (fabs(distance(center1, p3) - r) < fabs(distance(center2, p3) - r))
        return Circle(center1, r);
    return Circle(center2, r);

}

// gets vector of |size|<=3 points. returns circle which the points are on its border.
Circle trivial(vector<Point> points, size_t size) {
    switch (size) {
        case 3:
            return threePoints(points[0], points[1], points[2]);
        case 2:
            return twoPoints(points[0], points[1]);
        case 1:
            return Circle(points[0], 0);
        default:
            return Circle(Point(0,0),0);
    }
}

// gets pointers pointers to |size| points, and vector of |size2|<=3 points on border.
// returns the minimum circle contains all points using recursion.
Circle findMinCircleRec(Point** points,size_t size, vector<Point> onBorder, size_t size2) {
    // base - trivial. 3 points on border creates circle.
    if (size == 0 || size2 == 3) {
        return trivial(onBorder, size2);
    }
    // gets each iterate point out of points, calculates the minimum circle without it, and checks if its inside circle.
    // if the point is inside - return the circle. else - finds again when this point is on border of circle.
    Point p = *points[size-1];
    Circle c = findMinCircleRec(points, size-1, onBorder, size2);
    if (isInCircle(c, p)) {
        return c;
    } else {
        onBorder.push_back(p);
        return findMinCircleRec(points, size-1, onBorder, size2+1);
    }
}

// gets pointers of pointers of points and return the minimum circle contains all points.
Circle findMinCircle(Point** points,size_t size) {
    vector<Point> onBoundry;
    return findMinCircleRec(points, size, onBoundry, 0);
}