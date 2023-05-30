/**
 * This file implements several useful geometric algorithms and data structures.
 */
#pragma once

#include <cmath>
#include <optional>
#include <vector>
#include <unordered_map>
#include <deque>
#include <map>
#include <list>
#include <set>
#include <array>
#include <ranges>
#include <algorithm>

namespace geo {
    /**
     * Compares two floating point numbers and returns whether they're equivalent, in O(1) time.
     * @param d1
     * @param d2
     * @return
     */
    bool dsame(double d1, double d2) {
        return std::abs(d1 - d2) <= std::max(std::abs(d1), std::abs(d2)) * 2 * std::numeric_limits<double>::epsilon();
    }

    /**
     * Structure representing a point in 2D space
     */
    struct Point {
        double x;
        double y;
    };

    /**
     * Using floating point comparison, compares the two points and returns whether they're equivalent.
     * @param p1
     * @param p2
     * @return
     */
    bool operator==(Point const &p1, Point const &p2) {
        return dsame(p1.x, p2.x) && dsame(p1.y, p2.y);
    }

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
        static Line fromPoints(Point const &p1, Point const &p2) {
            return Line(p1.y - p2.y, p2.x - p1.x, (p1.x - p2.x) * p1.y + (p2.y - p1.y) * p1.x);
        }
    };

    /**
     * Structure that represents a line segment delimited by two points
     */
    struct LineSegment {
        Point p1;
        Point p2;
    };

    /**
     * Compares two line segments using floating point comparison and returns whether they're equivalent, even if they're
     * flipped.
     * @param l1
     * @param l2
     * @return
     */
    bool operator==(LineSegment const &l1, LineSegment const &l2) {
        return (l1.p1 == l2.p1 && l1.p2 == l2.p2) || (l1.p1 == l2.p2 && l1.p2 == l2.p1);
    }

    /**
     * Finds the intersection angle of two lines
     * @param l1 line 1
     * @param l2 line 2
     * @return
     */
    double intersectionAngle(Line const &l1, Line const &l2) {
        double m2 = -l1.a / l1.b;
        double m1 = -l2.a / l2.b;
        return std::atan2(m2 - m1, 1 + m1 * m2);
    }

    /**
     * Data structure representing a triangle composed of three points.
     */
    struct Triangle {
        std::array<Point, 3> points;
    };

    /**
     * Obtains an array of the LineSegments that represent the triangle's edges.
     * @param triangle
     * @return
     */
    std::array<LineSegment, 3> triangleEdges(Triangle const &triangle) {
        auto const &p = triangle.points;
        return {{
                        {p[0], p[1]},
                        {p[1], p[2]},
                        {p[2], p[0]},
                }};
    }

    /**
     * Calculates the circumcentre's position for a given triangle in O(1) time.
     * @param t
     * @return
     */
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

    /**
     * Determines whether a given point contained within a triangle's inscribed circumference.
     * @param p
     * @param t
     * @return
     */
    bool pointInTriangleCircumference(Point const &p, Triangle const &t) {
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

    /**
     * Structure representing a polygon with vertices stored in a vector
     */
    struct Polygon {
        std::vector<Point> points;
    };

    /**
     * Obtains the Delaunay triangulation for a set of points via the Bowyer-Watson algorithm.
     * This algorithm has a worst-case time complexity of O(n^2), scaling quadratically as more points are added
     * to the input vector. It finally outputs a vector of Triangle objects, each containing its 3 copies of its points.
     * @param points
     * @return
     */
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

        // add the super triangle as the first triangle in the triangulation set
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

    /**
      * This function obtains all the polygons that compose the Voronoin diagram for a series of points. It first
      * obtains the Delaunay triangulation via the Bowyer Watson algorithm, then obtaining the dual of this graph.
      * Its worst-case complexity is O(n^2), being mostly affected by the time complexity of the Delaunay algorithm's
      * time complexity.It returns a vector of Polygons. This vector skips over any polygon whose edges extend to infinity,
      * only keeping the inner, whole polygons>
      * @param points
      * @return
      */
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
}

