/**
 * @author : Dvir Asaf 313531113.
 */
#ifndef MINCIRCLE_H_
#define MINCIRCLE_H_

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "anomaly_detection_util.h"

using namespace std;

class Circle{
public:
    Point center;
    float radius;
    Circle(Point c,float r):center(c),radius(r){}
};

// cast array of Points to vector
vector<Point> array_to_vector(Point** points,size_t size);

float average(float x, float y);

//calculate pithargoras formula for two numbers
float pithagoras(float x, float y);


// calculate 2 dimensional distance of two points
float distance(const Point a, const Point b);

// check if a points found inside a circle
bool is_in_circle(const Circle& c, const Point& p);

// Function to check whether a circle encloses the given points
bool check_is_valid(const Circle& c, const vector<Point>& point);

// get a circle defined by 3 points
Point get_circle_center(float bx, float by, float cx, float cy);

// define the smallest circle that intersects 2 points
Circle get_circle_by_two_points(const Point& p1, const Point& p2);

// define the smallest circle that intersects 3 points
Circle get_circle_by_three_points(const Point& p1, const Point& p2,const Point& p3);

//return the minimum enclosing circle for n <= 3
Circle min_possible_circle(vector<Point>& point);

// Returns minimal enclosing circle using Welzl's algorithm
// Takes a set of input points p1 and a set p2 points on the circle boundary.
Circle welzl_algorithem(vector<Point>& p1, vector<Point> p2, int n);

// the algorithm that find minimum circle by using welzl algorithm
Circle findMinCircle(Point** points,size_t size);

#endif /* MINCIRCLE_H_ */