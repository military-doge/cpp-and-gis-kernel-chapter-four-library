#include <iostream>

#include "line_string.h"
#include "point.h"
#include "polygon.h"

int main()
{
    // Point
    const cppgis::Point a(0.0, 0.0);
    const cppgis::Point b(3.0, 4.0);
    std::cout << b.to_wkt() << '\n';
    std::cout << "  distance(a, b) = " << a.distance_to(b) << '\n';
    std::cout << "  a == b ? " << std::boolalpha << (a == b) << '\n';

    // LineString
    const cppgis::LineString line{{0.0, 0.0}, {3.0, 4.0}, {6.0, 0.0}};
    std::cout << line.to_wkt() << '\n';
    std::cout << "  points = " << line.num_points()
              << ", length = " << line.length()
              << ", closed = " << line.is_closed() << '\n';

    // Polygon：外环 + 一个洞
    const cppgis::Polygon polygon(
        cppgis::LineString{{0.0, 0.0}, {10.0, 0.0}, {10.0, 10.0},
                           {0.0, 10.0}, {0.0, 0.0}},
        {cppgis::LineString{{2.0, 2.0}, {2.0, 4.0}, {4.0, 4.0},
                            {4.0, 2.0}, {2.0, 2.0}}});
    std::cout << polygon.to_wkt() << '\n';
    std::cout << "  area = " << polygon.area()
              << ", perimeter = " << polygon.perimeter()
              << ", valid = " << polygon.is_valid() << '\n';

    return 0;
}
