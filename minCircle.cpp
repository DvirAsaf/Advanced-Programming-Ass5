/**
 * @author : Dvir Asaf 313531113.
 */
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include "anomaly_detection_util.h"
#include "minCircle.h"

using namespace std;


// cast array of Points to vector
vector<Point> array_to_vector(Point** points,size_t size){
    vector<Point> points_vector;
    points_vector.reserve(size);
    for(size_t i = 0; i < size; i++){
        points_vector.push_back(*points[i]);
    }
    return points_vector;
}

// calculate average of two numbers
float average(float x, float y){
    return (x + y) / 2;
}

//calculate pithargoras formula for two numbers
float pithagoras(float x, float y){
    return pow(x, 2) + pow(y, 2);
}

// calculate 2 dimensional distance of two points
float distance(const Point a, const Point b)
{
    float xDistance = a.x -b.x;
    float yDistance = a.y -b.y;
    float squaredDistance = pithagoras(xDistance, yDistance);
    float distance = sqrt(squaredDistance);
    return distance;
}

// check if a points found inside a circle
bool is_in_circle(const Circle& c, const Point& p)
{
    float demanded_distance = distance(c.center, p);
    float radius = c.radius;
    if (demanded_distance <= radius)
        return true;
    return false;
}

// Function to check whether a circle encloses the given points
bool check_is_valid(const Circle& c, const vector<Point>& point)
{
    int i = 0;
    while (i < point.size()){
        if (!is_in_circle(c, point[i]))
            return false;
        i++;
    }
    return true;
}

// get a circle defined by 3 points
Point get_circle_center(float bx, float by, float cx, float cy)
{
    float calc1 = pow(bx, 2) + pow(by, 2);
    float calc2 = pow(cx, 2) + pow(cy, 2);
    float calc3 = bx * cy - by * cx;
    float x = 0.5 * (cy * calc1 - by * calc2) / calc3;
    float y = 0.5 * (bx * calc2 - cx * calc1) / calc3;
    return Point(x,y);
}

// define the smallest circle that intersects 2 points
Circle get_circle_by_two_points(const Point& p1, const Point& p2)
{
    // Set the center to be the midpoint of p1 and p2
    Point C = Point(average(p1.x, p2.x), average(p1.y, p2.y)) ;
    float diameter = distance(p1, p2);
    float radius = diameter * 0.5;
    Circle cir (C, radius);
    return cir;
}

// define the smallest circle that intersects 3 points
Circle get_circle_by_three_points(const Point& p1, const Point& p2,const Point& p3)
{
    float bx = p2.x - p1.x;
    float by = p2.y - p1.y;
    float cx = p3.x - p1.x;
    float cy = p3.y - p1.y;
    Point p = get_circle_center(bx, by, cx, cy);
    p.y += p1.y;
    p.x += p1.x;
    float dist = distance(p,p1);
    Circle c (p,dist);
    return c;
}

//return the minimum enclosing circle for n <= 3
Circle min_possible_circle(vector<Point>& point)
{
    if (point.size() == 0) {
        Circle circle (Point(0,0), 0);
        return circle;
    }
    else if (point.size() == 1) {
        float r = 0;
        Circle circle (point[0], r);
        return circle;
    }
    else if (point.size() == 2) {
        return get_circle_by_two_points(point[0], point[1]);
    }

    // To check if MEC can be determined
    // by 2 points only
    Circle circle(Point(0,0), 0);
    int size = 3;
    int i = 0, j = 0;
    while (i < size){
        while(j < size){
            if(point[i].x == point[j].x && point[i].y == point[j].y){
                j++;
                continue;
            }
            circle = get_circle_by_two_points(point[i], point[j]);
            if (check_is_valid(circle, point)) {
                return circle;
            }
            j++;
        }
        i++;
    }
    return get_circle_by_three_points(point[0], point[1], point[2]);
}

// Returns minimal enclosing circle using Welzl's algorithm
// Takes a set of input points p1 and a set p2 points on the circle boundary.
Circle welzl_algorithem(vector<Point>& p1, vector<Point> p2, int n)
{
    // Base case when all points processed or |p2| = 3
    if (n == 0) {
        return min_possible_circle(p2);
    }
    if(p2.size() == 3){
        return min_possible_circle(p2);
    }

    // Pick a random point
    int index = rand() % n;
    Point p = p1[index];

    // swap between chosen point and nth point
    Point temp = p1[index];
    p1[index] = p1[n - 1];
    p1[n-1] = temp;

    // Get the minimal enclosing circle circle called alg_circle from the set of points
    Circle alg_circle = welzl_algorithem(p1, p2, n - 1);

    // If alg_circle contains p, return alg_circle
    if (is_in_circle(alg_circle, p)) {
        return alg_circle;
    }

    // push to vector
    p2.push_back(p);

    // recursion
    return welzl_algorithem(p1, p2, n - 1);
}

// the algorithm that find minimum circle by using welzl algorithm
Circle findMinCircle(Point** points,size_t size){
    vector<Point> vector_of_points = array_to_vector(points, size);
    return welzl_algorithem(vector_of_points, {}, vector_of_points.size());
}


