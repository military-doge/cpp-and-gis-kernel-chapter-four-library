#pragma once

#include <cstddef>
#include <initializer_list>
#include <string>
#include <vector>

#include "point.h"

namespace cppgis
{
    // OGC LineString（线要素）：1 维几何，由有序点列连接的折线。
    // 有效 LineString 为空或至少包含 2 个点。
    class LineString
    {
    public:
        LineString() = default;
        LineString(std::initializer_list<Point> points);
        explicit LineString(std::vector<Point> points);

        std::size_t num_points() const;
        const Point& point_n(std::size_t index) const;
        const std::vector<Point>& points() const;

        bool is_closed() const;
        bool is_valid() const;

        // 坐标单位下的折线总长度。
        double length() const;

        // OGC WKT: LINESTRING (x1 y1, x2 y2, ...)
        std::string to_wkt() const;

    private:
        std::vector<Point> points_;
    };
}
