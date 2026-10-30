#pragma once

#include <string>

namespace cppgis
{
    // OGC Point：0 维几何，表示一个坐标位置。
    class Point
    {
    public:
        Point();
        Point(double x, double y);

        double x() const;
        double y() const;
        double distance_to(const Point& other) const;
        bool equals(const Point& other) const;

        // OGC WKT: POINT (x y)
        std::string to_wkt() const;

    private:
        double x_;
        double y_;
    };

    bool operator==(const Point& lhs, const Point& rhs);
    bool operator!=(const Point& lhs, const Point& rhs);
}
