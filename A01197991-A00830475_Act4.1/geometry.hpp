#pragma once

#include <cmath>
#include <optional>
#include <vector>

namespace geo {
    const double EPSILON = 1e-9;

    struct Point {
        double x;
        double y;
    };

    struct Line {
        double a;
        double b;
        double c;

        static Line fromPoints(Point const &p1, Point const &p2);

        static Line fromPointAndSlope(Point const &p, double m);

    };

    struct LineSegment {
        Point p1;
        Point p2;
    };

    struct Polygon {
        std::vector<Point> points;
    };

    double perimeter(Polygon const &p);

    double area(Polygon const &p);

    bool pointInBox(Point const &p, Point const &b1, Point const &b2);

    bool areParallel(Line const &l1, Line const &l2);

    bool intersect(LineSegment const &s1, LineSegment const &s2);

    bool areSameLine(Line const &l1, Line const &l2);

    std::optional<Point> intersectionPoint(Line const &l1, Line const &l2);

    double intersectionAngle(Line const &l1, Line const &l2);

    std::optional<Point> closestPoint(Point const &p, Line const &l);

    double distance(Point const &p1, Point const &p2);

    Point rotate(Point const &p);
}

