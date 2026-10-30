#include "polygon.h"

#include <cmath>
#include <sstream>
#include <utility>

namespace cppgis
{
    namespace
    {
        // 鞋带公式：返回环的有向面积。
        // 逆时针为正，顺时针为负。
        double signed_ring_area(const LineString& ring)
        {
            const std::vector<Point>& pts = ring.points();
            const std::size_t n = pts.size();
            if (n < 3)
            {
                return 0.0;
            }

            double sum = 0.0;
            for (std::size_t i = 0; i < n; ++i)
            {
                const Point& a = pts[i];
                const Point& b = pts[(i + 1) % n];
                sum += a.x() * b.y() - b.x() * a.y();
            }
            return sum * 0.5;
        }

        // OGC 环的有效性：闭合，且至少 4 个点（含重复的闭合点）。
        bool ring_is_valid(const LineString& ring)
        {
            return ring.num_points() >= 4 && ring.is_closed();
        }

        void append_ring(std::ostringstream& os, const LineString& ring)
        {
            os << '(';
            const std::vector<Point>& pts = ring.points();
            for (std::size_t i = 0; i < pts.size(); ++i)
            {
                if (i != 0)
                {
                    os << ", ";
                }
                os << pts[i].x() << ' ' << pts[i].y();
            }
            os << ')';
        }
    }

    Polygon::Polygon(LineString exterior_ring)
        : exterior_(std::move(exterior_ring)) {}

    Polygon::Polygon(LineString exterior_ring,
                     std::vector<LineString> interior_rings)
        : exterior_(std::move(exterior_ring)),
          interiors_(std::move(interior_rings)) {}

    const LineString& Polygon::exterior_ring() const { return exterior_; }

    std::size_t Polygon::num_interior_rings() const
    {
        return interiors_.size();
    }

    const LineString& Polygon::interior_ring(std::size_t index) const
    {
        return interiors_.at(index);
    }

    double Polygon::area() const
    {
        // 外环面积减去所有内环（洞）面积。
        double result = std::abs(signed_ring_area(exterior_));
        for (const LineString& hole : interiors_)
        {
            result -= std::abs(signed_ring_area(hole));
        }
        return result > 0.0 ? result : 0.0;
    }

    double Polygon::perimeter() const
    {
        // OGC 边界长度：外环 + 所有内环的长度。
        double result = exterior_.length();
        for (const LineString& hole : interiors_)
        {
            result += hole.length();
        }
        return result;
    }

    bool Polygon::is_valid() const
    {
        if (!ring_is_valid(exterior_))
        {
            return false;
        }
        for (const LineString& hole : interiors_)
        {
            if (!ring_is_valid(hole))
            {
                return false;
            }
        }
        return true;
    }

    std::string Polygon::to_wkt() const
    {
        std::ostringstream os;
        os << "POLYGON (";
        append_ring(os, exterior_);
        for (const LineString& hole : interiors_)
        {
            os << ", ";
            append_ring(os, hole);
        }
        os << ')';
        return os.str();
    }
}
