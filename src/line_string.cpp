#include "line_string.h"

#include <sstream>
#include <utility>

namespace cppgis
{
    LineString::LineString(std::initializer_list<Point> points)
        : points_(points) {}

    LineString::LineString(std::vector<Point> points)
        : points_(std::move(points)) {}

    std::size_t LineString::num_points() const { return points_.size(); }

    const Point& LineString::point_n(std::size_t index) const
    {
        return points_.at(index);
    }

    const std::vector<Point>& LineString::points() const { return points_; }

    bool LineString::is_closed() const
    {
        return points_.size() >= 2 && points_.front() == points_.back();
    }

    bool LineString::is_valid() const
    {
        // OGC: LineString 为空，或至少包含 2 个点。
        return points_.empty() || points_.size() >= 2;
    }

    double LineString::length() const
    {
        double total = 0.0;
        for (std::size_t i = 1; i < points_.size(); ++i)
        {
            total += points_[i - 1].distance_to(points_[i]);
        }
        return total;
    }

    std::string LineString::to_wkt() const
    {
        if (points_.empty())
        {
            return "LINESTRING EMPTY";
        }

        std::ostringstream os;
        os << "LINESTRING (";
        for (std::size_t i = 0; i < points_.size(); ++i)
        {
            if (i != 0)
            {
                os << ", ";
            }
            os << points_[i].x() << ' ' << points_[i].y();
        }
        os << ')';
        return os.str();
    }
}
