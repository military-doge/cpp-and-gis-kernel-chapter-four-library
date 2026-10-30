#include "point.h"

#include <cmath>
#include <sstream>

namespace cppgis
{
    Point::Point() : x_(0.0), y_(0.0) {}

    Point::Point(double x, double y) : x_(x), y_(y) {}

    double Point::x() const { return x_; }

    double Point::y() const { return y_; }

    double Point::distance_to(const Point& other) const
    {
        const double dx = x_ - other.x_;
        const double dy = y_ - other.y_;
        return std::sqrt(dx * dx + dy * dy);
    }

    bool Point::equals(const Point& other) const
    {
        return x_ == other.x_ && y_ == other.y_;
    }

    std::string Point::to_wkt() const
    {
        std::ostringstream os;
        os << "POINT (" << x_ << ' ' << y_ << ')';
        return os.str();
    }

    bool operator==(const Point& lhs, const Point& rhs)
    {
        return lhs.equals(rhs);
    }

    bool operator!=(const Point& lhs, const Point& rhs)
    {
        return !(lhs == rhs);
    }
}
