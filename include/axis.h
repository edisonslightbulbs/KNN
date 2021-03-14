#ifndef AXIS_H
#define AXIS_H

#include <algorithm>
#include <vector>

#include "point.h"

namespace axis {

static void sortXAxis(std::vector<Point>& points)
{
    std::sort(points.begin(), points.end(),
        [](const Point& point, const Point& other) {
            return point.m_x < other.m_x;
        });
}

static void sortYAxis(std::vector<Point>& points)
{
    std::sort(points.begin(), points.end(),
        [](const Point& point, const Point& other) {
            return point.m_y < other.m_y;
        });
}

static void sortZAxis(std::vector<Point>& points)
{
    std::sort(points.begin(), points.end(),
        [](const Point& point, const Point& other) {
            return point.m_z < other.m_z;
        });
}

}
#endif /* AXIS_H */
