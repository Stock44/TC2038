/**
 * This file implements all the computational geometry methods seen in class.
 */
#pragma once

#include <cmath>
#include <optional>
#include <vector>

namespace geo {
    // Used for floating point comparisons
    const double EPSILON = 1e-9;

    /**
     * Structure representing a point in 2D space
     */
    struct Point {
        double x;
        double y;
    };

    /**
     * Structure representing a line in 2D space
     */
    struct Line {
        double a;
        double b;
        double c;

        /**
         * Creates a line from 2 base points
         * @param p1 reference point 1
         * @param p2 reference point 2
         * @return
         */
        static Line fromPoints(Point const &p1, Point const &p2);

        /**
         * Creates a line from a base point and a slope
         * @param p reference point
         * @param m slope
         * @return
         */
        static Line fromPointAndSlope(Point const &p, double m);

    };

    /**
     * Structure that represents a line segment delimited by two points
     */
    struct LineSegment {
        Point p1;
        Point p2;
    };

    /**
     * Structure representing a polygon with vertices stored in a vector
     */
    struct Polygon {
        std::vector<Point> points;
    };

    /**
     * Finds the perimeter of a given polygon
     * @param p
     * @return
     */
    double perimeter(Polygon const &p);

    /**
     * Finds the area of a given polygon
     * @param p
     * @return
     */
    double area(Polygon const &p);

    /**
     * Checks if a point is located within a bounding box delimited by its two
     * opposing corners b1 and b2
     * @param p reference point
     * @param b1 first corner of box
     * @param b2 second corner of box
     * @return
     */
    bool pointInBox(Point const &p, Point const &b1, Point const &b2);

    /**
     * Checks if two lines are parallel to each other
     * @param l1 reference line 1
     * @param l2 reference line 2
     * @return
     */
    bool areParallel(Line const &l1, Line const &l2);

    /**
     * Checks if two line segments intersect each other
     * @param s1 reference line segment 1
     * @param s2 reference line segment 2
     * @return
     */
    bool intersect(LineSegment const &s1, LineSegment const &s2);

    /**
     * Checks if two lines are actually the same line
     * @param l1 line 1
     * @param l2 line 2
     * @return
     */
    bool areSameLine(Line const &l1, Line const &l2);

    /**
     * Returns the intersection point of two lines, if any
     * @param l1 line 1
     * @param l2 line 2
     * @return an optional value for the intersection point
     */
    std::optional<Point> intersectionPoint(Line const &l1, Line const &l2);

    /**
     * Finds the intersection angle of two lines
     * @param l1 line 1
     * @param l2 line 2
     * @return
     */
    double intersectionAngle(Line const &l1, Line const &l2);

    /**
     * Finds the closest point in a line to a reference point
     * @param p reference point
     * @param l line to check
     * @return the point
     */
    std::optional<Point> closestPoint(Point const &p, Line const &l);

    /**
     * Calculates the distance between two points
     * @param p1
     * @param p2
     * @return
     */
    double distance(Point const &p1, Point const &p2);

    /**
     * Rotates a point around the world origin (0,0) for a given angle
     * in radians.
     * @param p
     * @param theta
     * @return
     */
    Point rotate(Point const &p, double theta);
}

