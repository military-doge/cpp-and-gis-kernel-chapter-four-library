#pragma once

#include <cstddef>
#include <string>
#include <vector>

#include "line_string.h"

namespace cppgis
{
    // OGC Polygon（面要素）：2 维几何，由 1 个外环和 0..n 个内环（洞）组成。
    // 每个环都必须是闭合且连续（首尾点相同）的 LineString。
    // 面积/周长使用坐标单位（平方单位 / 单位）。
    class Polygon
    {
    public:
        explicit Polygon(LineString exterior_ring);
        Polygon(LineString exterior_ring,
                std::vector<LineString> interior_rings);

        const LineString& exterior_ring() const;
        std::size_t num_interior_rings() const;
        const LineString& interior_ring(std::size_t index) const;

        double area() const;
        double perimeter() const;
        bool is_valid() const;

        // OGC WKT: POLYGON ((外环), (内环1), ...)
        std::string to_wkt() const;

    private:
        LineString exterior_;
        std::vector<LineString> interiors_;
    };
}
