#include "geometry2d.h"

#include <fstream>

using namespace geometry2d;

Point::Point(double x, double y) : x(x), y(y){};

ShapeType Point::type() { return ShapeType::POINT; }

LineSegment::LineSegment(Point p1, Point p2) : p1(p1), p2(p2){};

ShapeType LineSegment::type() { return ShapeType::LINE; }

Arc::Arc(Point c, Point p1, Point p2) : c(c), p1(p1), p2(p2){};

ShapeType Arc::type() { return ShapeType::ARC; }

Shape* GeometryFactory::createShape(double x, double y) const {
  return new Point(x, y);
};
Shape* GeometryFactory::createShape(Point p1, Point p2) const {
  return new LineSegment(p1, p2);
};
Shape* GeometryFactory::createShape(Point c, Point p1, Point p2) const {
  return new Arc(c, p1, p2);
}

std::vector<Shape*> geometry2d::readGeometry(const IGeometryFactory* factory,
                                             std::istream& stream) {
  std::vector<Shape*> result;

  std::string line;
  while (std::getline(stream, line)) {
    std::istringstream iss(line);
    std::string type;
    iss >> type;

    std::vector<double> coords;
    double coord;
    while (iss >> coord) {
      coords.push_back(coord);
    }

    bool found = false;

    if (type == "P") {
      for (int i = 0; i < result.size(); ++i) {
        auto point = dynamic_cast<Point*>(result[i]);

        if (!point) {
          continue;
        }

        if (point->x == coords[0] && point->y == coords[1]) {
          found = true;
          break;
        }
      }

      if (!found) {
        result.push_back(factory->createShape(coords[0], coords[1]));
      }
    } else if (type == "L") {
      for (int i = 0; i < result.size(); ++i) {
        auto line = dynamic_cast<LineSegment*>(result[i]);

        if (line) {
          if (line->p1.x == coords[0] && line->p1.y == coords[1] &&
              line->p2.x == coords[2] && line->p2.y == coords[2]) {
            found = true;
            break;
          }
        }
      }

      if (!found) {
        result.push_back(factory->createShape(Point(coords[0], coords[1]),
                                              Point(coords[2], coords[3])));
      }
    } else if (type == "A") {
      for (int i = 0; i < result.size(); ++i) {
        auto arc = dynamic_cast<Arc*>(result[i]);

        if (arc) {
          if (arc->c.x == coords[0] && arc->c.y == coords[1] &&
              arc->p1.x == coords[2] && arc->p1.y == coords[3] &&
              arc->p2.x == coords[4] && arc->p2.y == coords[5]) {
            found = true;
            break;
          }
        }
      }

      result.push_back(factory->createShape(Point(coords[0], coords[1]),
                                            Point(coords[2], coords[3]),
                                            Point(coords[4], coords[5])));
    }
  }

  return result;
}

std::vector<Shape*> geometry2d::readGeometry(const IGeometryFactory* factory,
                                             const std::filesystem::path& path)

{
  std::ifstream fs(path);
  return geometry2d::readGeometry(factory, fs);
}

Point geometry2d::centroid(const std::vector<Shape*>& shapes) {
  Point p(0, 0);

  for (int i = 0; i < shapes.size(); ++i) {
    if (shapes[i]->type() == ShapeType::POINT) {
      p.x += dynamic_cast<Point*>(shapes[i])->x;
      p.y += dynamic_cast<Point*>(shapes[i])->y;
    }

    if (shapes[i]->type() == ShapeType::LINE) {
      p.x += dynamic_cast<LineSegment*>(shapes[i])->p1.x;
      p.y += dynamic_cast<LineSegment*>(shapes[i])->p1.y;

      p.x += dynamic_cast<LineSegment*>(shapes[i])->p2.x;
      p.y += dynamic_cast<LineSegment*>(shapes[i])->p2.y;
    }

    if (shapes[i]->type() == ShapeType::ARC) {
      p.x += dynamic_cast<Arc*>(shapes[i])->p1.x;
      p.y += dynamic_cast<Arc*>(shapes[i])->p1.y;

      p.x += dynamic_cast<Arc*>(shapes[i])->p1.x;
      p.y += dynamic_cast<Arc*>(shapes[i])->p1.y;

      p.x += dynamic_cast<Arc*>(shapes[i])->c.x;
      p.y += dynamic_cast<Arc*>(shapes[i])->c.y;
    }
  }

  p.x /= shapes.size();
  p.y /= shapes.size();

  return p;
}

ShapeType BoundingBox::type() { return ShapeType::BOX; }

void BoundingBox::add(Point p) {
  if (p.x < p1.x) {
    p1.x = p.x;
  }

  if (p.y < p1.y) {
    p1.y = p.y;
  }

  if (p.x > p2.x) {
    p.x = p2.x;
  }

  if (p.y > p2.y) {
    p.y = p2.y;
  }
}

void BoundingBox::add(LineSegment line) {
  add(line.p1);
  add(line.p2);
}

void BoundingBox::add(Arc arc) {
  add(arc.c);
  add(arc.p1);
  add(arc.p2);
}

Point* Intersect::intersect(LineSegment l1, LineSegment l2) { return nullptr; }
Point* Intersect::intersect(LineSegment segment, Arc arc) { return nullptr; }
Point* Intersect::intersect(Point p, LineSegment l) { return nullptr; }
Point* Intersect::intersect(Point p, Arc l) { return nullptr; }
