#include <algorithm>
#include <stack>
#include "geometry.hpp"

namespace geo {
    bool operator==(const Point &p1, const Point &p2) {
        return dsame(p1.x, p2.x) && dsame(p1.y, p2.y);
    }

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
        return Line(p1.y - p2.y, p2.x - p1.x, (p1.x - p2.x) * p1.y + (p2.y - p1.y) * p1.x);
    }

    Line Line::fromPointAndSlope(Point const &p, double m) {
        return Line(-m, 1, -p.x * m + p.y);
    }

    bool operator==(const LineSegment &l1, const LineSegment &l2) {
        return (l1.p1 == l2.p1 && l1.p2 == l2.p2) || (l1.p1 == l2.p2 && l1.p2 == l2.p1);
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
        double m2 = -l1.a / l1.b;
        double m1 = -l2.a / l2.b;
        return std::atan2(m2 - m1, 1 + m1 * m2);
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

    std::vector<Polygon> voronoiDiagram(const std::vector<Point> &points) {
        auto delaunay = bowyerWatson(points);
        std::vector<Point> circumcenters;
        std::ranges::transform(delaunay, std::back_inserter(circumcenters), triangleCircumcentre);

        std::vector<Polygon> voronoi;

        for (auto const &point: points) {
            std::vector<std::pair<Triangle, Point>> candidatePoints;

            for (std::size_t i = 0; i < delaunay.size(); i++) {
                auto triangle = delaunay.at(i);
                auto circumcenter = circumcenters.at(i);

                if (std::ranges::find(triangle.points, point) == triangle.points.end()) continue;

                candidatePoints.emplace_back(triangle, circumcenter);
            }

            std::ranges::sort(candidatePoints, [&point](auto const &c1, auto const &c2) {
                auto p1 = c1.second;
                auto p2 = c2.second;
                double dx1 = p1.x - point.x;
                double dx2 = p2.x - point.x;
                double dy1 = p1.y - point.y;
                double dy2 = p2.y - point.y;

                double theta1 = std::atan2(dy1, dx1);
                double theta2 = std::atan2(dy2, dx2);

                return theta1 < theta2;
            });

            if (candidatePoints.size() < 3) continue;

            bool valid = true;

            for (auto point = candidatePoints.begin() + 1; point != candidatePoints.end(); point++) {
                auto prevPoint = point - 1;
                valid = std::ranges::any_of(triangleEdges(point->first), [&prevPoint](auto const &edge) {
                    return std::ranges::count(triangleEdges(prevPoint->first), edge) > 0;
                });
                if (!valid) break;
            }

            if (!valid) continue;

            auto const &first = candidatePoints.begin()->first;
            auto const &last = (candidatePoints.end() - 1)->first;

            valid = std::ranges::any_of(triangleEdges(first), [&last](auto const &edge) {
                return std::ranges::count(triangleEdges(last), edge) > 0;
            });

            if (valid) {
                Polygon polygon;
                for (auto const &[t, p]: candidatePoints) {
                    polygon.points.push_back(p);
                }
                voronoi.push_back(polygon);
            }
        }

        return voronoi;
    }

    bool pointInTriangleCircumference(const Point &p, const Triangle &t) {
        auto &v = t.points;

        double a = v[0].x - p.x;
        double b = v[0].y - p.y;
        double c = std::pow(a, 2) + std::pow(b, 2);
        double d = v[1].x - p.x;
        double e = v[1].y - p.y;
        double f = std::pow(d, 2) + std::pow(e, 2);
        double g = v[2].x - p.x;
        double h = v[2].y - p.y;
        double i = std::pow(g, 2) + std::pow(h, 2);

        double det = a * e * i + b * f * g + c * d * h - c * e * g - b * d * i - a * f * h;
        return det > 0;
    }

    bool pointInTriangle(const Point &p, const Triangle &t) {
        auto &v = t.points;
        double w1 = triangleArea({p, v[0], v[1]});
        double w2 = triangleArea({p, v[1], v[2]});
        double w3 = triangleArea({p, v[2], v[0]});

        bool hasNeg = (w1 < 0) || (w2 < 0) || (w3 < 0);
        bool hasPos = (w1 > 0) || (w2 > 0) || (w3 > 0);

        return !(hasNeg && hasPos);
    }

    double triangleArea(const Triangle &t) {
        auto const &p = t.points;
        return (p[0].x - p[2].x) * (p[1].y - p[2].y) * (p[1].x - p[2].x) * (p[0].y - p[2].y);
    }

    bool dsame(double d1, double d2) {
        return std::abs(d1 - d2) <= std::max(std::abs(d1), std::abs(d2)) * 2 * std::numeric_limits<double>::epsilon();
    }

    std::vector<Triangle> bowyerWatson(const std::vector<Point> &points) {
        std::map<std::size_t, Triangle> triangles;
        std::size_t nextId = 0;

        double minX = std::numeric_limits<double>::max();
        double maxX = std::numeric_limits<double>::min();
        double minY = std::numeric_limits<double>::max();
        double maxY = std::numeric_limits<double>::min();

        for (auto const &point: points) {
            minX = std::min(minX, point.x);
            maxX = std::max(maxX, point.x);
            minY = std::min(minY, point.y);
            maxY = std::max(maxY, point.y);
        }

        double width = maxX - minX;
        double height = maxY - minY;

        Triangle superTriangle({{{minX - width, minY - height}, {maxX + width * 3, minY - height},
                                 {minX - width, maxY + height * 3}}});

        // add the supertriangle as the first triangle in the triangulation set
        triangles.emplace(nextId, superTriangle);
        nextId++;

        for (auto const &point: points) {
            // store the ids of bad triangles, to be removed at the end
            std::set<std::size_t> badTriangleIds;
            // find bad triangles
            for (auto const &[id, triangle]: triangles) {
                if (pointInTriangleCircumference(point, triangle)) {
                    badTriangleIds.emplace(id);
                }
            }

            // find the polygon of all bad triangles
            std::vector<LineSegment> polygon;

            for (auto const &id: badTriangleIds) {
                auto const &badTriangle = triangles.at(id);
                auto const &p = badTriangle.points;
                // for every edge in this bad triangle
                auto isShared = triangleEdges(badTriangle) | std::views::filter(
                        [id, &triangles, &badTriangleIds](auto edge) {
                            // if the current edge is not within any of the other bad triangles, true
                            return !std::ranges::any_of(badTriangleIds
                                                        | std::views::filter(
                                                                [id](auto otherId) { return otherId != id; })
                                                        | std::views::transform(
                                                                [&triangles](auto otherId) {
                                                                    return triangles.at(otherId);
                                                                }),
                                                        [&edge](
                                                                Triangle const &otherBadTriangle) {
                                                            return std::ranges::count(triangleEdges(otherBadTriangle),
                                                                                      edge) > 0;
                                                        });
                        });
                for (auto const &edge: isShared) {
                    polygon.push_back(edge);
                }
            }
            for (auto triangleId: badTriangleIds) {
                triangles.erase(triangleId);
            }

            for (auto const &edge: polygon) {
                Triangle triangle{edge.p1, edge.p2, point};
                triangles.emplace(nextId, triangle);
                nextId++;
            }
        }

        std::vector<Triangle> triangulation;
        triangulation.reserve(triangles.size());
        for (auto const &[id, triangle]: triangles) {
            if (!std::ranges::any_of(triangle.points, [&superTriangle](auto point) {
                return std::ranges::find(superTriangle.points, point) != superTriangle.points.end();
            })) {
                triangulation.push_back(triangle);
            }
        }
        return triangulation;
    }

    std::array<LineSegment, 3> triangleEdges(const Triangle &triangle) {
        auto const &p = triangle.points;
        return {{
                        {p[0], p[1]},
                        {p[1], p[2]},
                        {p[2], p[0]},
                }};
    }

    Point triangleCircumcentre(const Triangle &t) {
        auto const &a = t.points[0];
        auto const &b = t.points[1];
        auto const &c = t.points[2];

        auto side1 = Line::fromPoints(a, b);
        auto side2 = Line::fromPoints(b, c);
        auto side3 = Line::fromPoints(c, a);

        auto sinA = std::sin(2 * (intersectionAngle(side3, side1)));
        auto sinB = std::sin(2 * (intersectionAngle(side1, side2)));
        auto sinC = std::sin(2 * (intersectionAngle(side2, side3)));

        Point point{};
        point.x = (a.x * sinA + b.x * sinB + c.x * sinC) /
                  (sinA + sinB + sinC);
        point.y = (a.y * sinA + b.y * sinB + c.y * sinC) /
                  (sinA + sinB + sinC);

        return point;
    }

}
