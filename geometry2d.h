#pragma once

#include <filesystem>
#include <vector>

namespace geometry2d {

enum ShapeType { POINT, LINE, ARC, BOX };

class Shape {
 public:
  virtual ShapeType type() = 0;
};

class Point : public Shape {
 public:
  explicit Point(double x, double y);
  ShapeType type() override;

  double x = 0.;
  double y = 0.;
};

class LineSegment : public Shape {
 public:
  explicit LineSegment(Point p1, Point p2);
  ShapeType type() override;

  double slope() const;

  Point p1;
  Point p2;
};

class Arc : public Shape {
 public:
  explicit Arc(Point c, Point p1, Point p2);
  ShapeType type() override;

  Point c;
  Point p1;
  Point p2;
};

class BoundingBox : public Shape {
 public:
  ShapeType type() override;

  void add(Point p);
  void add(LineSegment line);
  void add(Arc arc);

  Point p1;
  Point p2;
};

class IGeometryFactory {
 public:
  virtual Shape* createShape(double x, double y) const = 0;
  virtual Shape* createShape(Point p1, Point p2) const = 0;
  virtual Shape* createShape(Point c, Point p1, Point p2) const = 0;
};

class GeometryFactory : public IGeometryFactory {
 public:
  Shape* createShape(double x, double y) const override;
  Shape* createShape(Point p1, Point p2) const override;
  Shape* createShape(Point c, Point p1, Point p2) const override;
};

std::vector<Shape*> readGeometry(const IGeometryFactory* factory,
                                 std::istream& stream);

std::vector<Shape*> readGeometry(const IGeometryFactory* factory,
                                 const std::filesystem::path& path);

Point centroid(const std::vector<Shape*>& shapes);

class Intersect {
 public:
  static Point* intersect(LineSegment l1, LineSegment l2);
  static Point* intersect(LineSegment segment, Arc arc);
  static Point* intersect(Point p, LineSegment l);
  static Point* intersect(Point p, Arc l);
};

}  // namespace geometry2d
