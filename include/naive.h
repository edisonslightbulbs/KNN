#ifndef NAIVEKNN_H
#define NAIVEKNN_H

#include <vector>

#include "logger.h"
#include "point.h"
#include "timer.h"

namespace unoptimized {

/** K including core point, viz, 4 nearest neighbour */
const int K = 5;

std::vector<float> run(const std::vector<Point>& points)
{
    std::vector<float> knn;
    std::vector<std::vector<Point>> neighoursLists;

    /** O(n^2) search */
    for (const auto& point : points) {

        /** create mutable clone */
        std::vector<Point> clone(points);

        /** track the distance to ever other neighbour */
        std::vector<Point> neighbours;
        for (auto& otherPoint : clone) {
            float distance = otherPoint.distance(point);
            otherPoint.m_distance = { point.m_id, distance };
            neighbours.push_back(otherPoint);
        }
        neighoursLists.push_back(neighbours);
    }

    /** quick sort points using distances */
    for (auto& neighourhood : neighoursLists) {
        Point::sort(neighourhood);
    }

    /** pick out the Kth nearest neighbour */
    for (auto& neighbourhood : neighoursLists) {
        // std::cout << neighbourhood[0].m_id << ": " <<  neighbourhood[0]
        // <<std::endl;
        knn.emplace_back(neighbourhood[K].m_distance.second);
    }
    return knn;
}
}
#endif /* NAIVEKNN_H */
