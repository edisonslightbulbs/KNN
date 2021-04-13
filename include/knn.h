#ifndef KNN_H
#define KNN_H

#include <string>
#include <vector>

namespace knn {

struct t_point {
    float x;
    float y;
    float z;
};

/** compute
 *    Wraps nanoflann's knn implementation into a tiny interface
 *
 *  @param points
 *    Set of 3D points (x, y, z).
 *
 *  @param k
 *    Value of K, i.e., number of neighbours
 *
 *  @param indexOfQueryPoint
 *    Index of the query point, i.e., index corresponding to first
 *    parameter
 *
 *  @param bucket2NthNn
 *    list of point-distance pairs.
 *
 *  @retval
 *     A list of point-distance pairs corresponding the nearest
 *     neighbours. The points and respective distances from a
 *     query point are sorted in ascending order.
 * */

std::vector<std::pair<knn::t_point, float>> compute(
    std::vector<t_point>& points, const int& k, const int& indexOfQueryPoint,
    std::vector<std::pair<t_point, float>>& bucket2NthNn);
}
#endif /* KNN_H */
