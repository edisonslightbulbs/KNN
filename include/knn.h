#ifndef KNN_H
#define KNN_H

#include <memory>
#include <vector>

#include "point.h"

class Knn {

public:
    Knn() = default;
    ~Knn() = default;
    static std::vector<float> compute(std::vector<Point>& points);
};
#endif /* KNN_H */
