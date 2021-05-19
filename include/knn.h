#ifndef KNN_H
#define KNN_H

#include <string>
#include <vector>

#include "point.h"

namespace knn {

/** compute
 *    Computes the K nearest neighbours of a queryPoint.
 *
 *  @param points
 *    Set of 3D points (x, y, z).
 *
 *  @param k
 *    K value.
 *
 *  @param queryPoint
 *    query point
 *
 *  @retval
 *    The indexes of the nearest neighbors from closest
 *    neighbor to furthest neighbour
 * */

std::vector<int> compute(
    std::vector<Point>& points, const Point& queryPoint, const int& k);
}
#endif /* KNN_H */
