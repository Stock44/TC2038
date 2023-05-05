#include "geometry.hpp"

namespace geo {
    double distance(Point const &p1, Point const &p2) {
        using std::pow;
        using std::sqrt;
        return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
    }

    bool areParallel(Line const &l1, Line const &l2) {
        using std::abs;
        return abs(l1.a - l2.a) <= EPSILON &&
               abs(l1.b - l2.b) <= EPSILON;
    }

    Point rotate(Point const &p, double theta) {
        using std::sin;
        using std::cos;
        return Point(p.x * cos(theta) - p.y * sin(theta),
                     p.x * sin(theta) + p.y * cos(theta));
    }

    Line Line::fromPoints(Point const &p1, Point const &p2) {
        using std::abs;
        double m = (p1.y - p2.y) / (p1.x - p2.x);
        return Line(m, -1.0, p1.y - m * p1.x);
    }

    Line Line::fromPointAndSlope(Point const &p, double m) {
        return Line(-m, 1, -p.x * m + p.y);
    }

    bool areSameLine(Line const &l1, Line const &l2) {
        using std::abs;
        return areParallel(l1, l2) && abs(l1.c - l2.c) <= EPSILON;
    }

    std::optional<Point> intersectionPoint(Line const &l1, Line const &l2) {
        using std::abs;

        if (areParallel(l1, l2)) {
            return {};
        }

        Point point{};
        point.x = ((l1.b * l2.c) - (l2.b * l1.c)) /
                  ((l1.a * l2.b) - (l2.a * l1.b));

        if (abs(l1.b) < EPSILON) {
            point.y = -(l2.a * point.x + l2.c) / l2.b;
        } else {
            point.y = -(l1.a * point.x + l1.c) / l1.b;
        }

        return point;
    }

    double intersectionAngle(Line const &l1, Line const &l2) {
        using std::atan;
        return atan((l1.a * l2.b - l2.a * l1.b) /
                    (l1.a * l2.a - l1.b * l2.b));
    }

    std::optional<Point> closestPoint(Point const &p, Line const &l) {
        using std::abs;
        if (abs(l.b) <= EPSILON) {
            return Point{-l.c, p.y};
        }

        if (abs(l.a) <= EPSILON) {
            return Point{p.x, -l.c};
        }

        auto aux = Line::fromPointAndSlope(p, 1.0 / l.a);
        return intersectionPoint(l, aux);
    }

    bool pointInBox(Point const &p, Point const &b1, Point const &b2) {
        using std::min;
        using std::max;
        return p.x >= min(b1.x, b2.x) &&
               p.x <= max(b1.x, b2.x) &&
               p.y >= min(b1.y, b2.y) &&
               p.y <= max(b1.y, b2.y);
    }

    bool intersect(LineSegment const &s1, LineSegment const &s2) {
        auto l1 = Line::fromPoints(s1.p1, s1.p2);
        auto l2 = Line::fromPoints(s2.p1, s2.p2);

        if (areParallel(l1, l2)) return false;

        if (areSameLine(l1, l2)) {
            return pointInBox(s1.p1, s2.p1, s2.p2) ||
                   pointInBox(s1.p2, s2.p1, s2.p2) ||
                   pointInBox(s2.p1, s1.p1, s1.p2) ||
                   pointInBox(s2.p2, s1.p1, s1.p2);
        }

        auto p = intersectionPoint(l1, l2);
        if (!p.has_value()) return false;
        return pointInBox(p.value(), s1.p1, s1.p2) &&
               pointInBox(p.value(), s2.p1, s2.p2);
    }

    double perimeter(Polygon const &p) {
        double total = 0.0;

        if (p.points.size() <= 2) {
            return 0;
        }

        for (auto prevPoint = p.points.begin(), point = prevPoint + 1; point != p.points.end(); point++, prevPoint++) {
            total += distance(*prevPoint, *point);
        }

        return total;
    }

    double area(Polygon const &p) {
        using std::abs;
        double result = 0;
        for (auto prevPoint = p.points.begin(), point = prevPoint + 1; point != p.points.end(); prevPoint++, point++) {
            double x1 = prevPoint->x;
            double x2 = point->x;
            double y1 = prevPoint->y;
            double y2 = point->y;

            result += x1 * y2 - x2 * y1;
        }

        return abs(result) / 2.0;
    }

}
