#ifndef ELBOW_H
#define ELBOW_H

#include <algorithm>
#include <cmath>
#include <vector>

#include "point.h"

namespace elbow {

float find(std::vector<float>& knn4)
{
    /** sort in descending order */
    std::sort(knn4.begin(), knn4.end(), std::greater<>());

    /** evaluate successive angular distances */
    std::vector<std::pair<float, float>> angles;
    for (int i = 1; i < knn4.size() - 1; i++) {
        float angle = std::asin(knn4[i + 1] - knn4[i]);
        angles.emplace_back(knn4[i], angle);
    }

    /** find the max second derivative */
    float sd = 0;
    for (int i = 1; i < angles.size() - 1; i++) {
        float d1 = angles[i - 1].second - angles[i].second;
        float d2 = angles[i + 1].second - angles[i].second;

        /** evaluate second derivative-based central difference */
        float candidate = d1 + d2 - 2 * angles[i].second;

        /** elbow corresponds to distance with the max second derivative */
        if (candidate > sd) {
            sd = angles[i].first;
        }
    }
    return sd;
}
}
#endif /* ELBOW_H */
