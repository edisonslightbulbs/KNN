// #include "knn.h"
// #include "io.h"
// #include "kdtree.h"
// #include "logger.h"
// #include "naive.h"
// #include "timer.h"
//
// std::vector<float> knn::compute(std::vector<Point>& points)
// {
//     /** create container for Kth nearest neighbours of all points */
//     std::vector<float> knn4;
//     {
//         Timer timer;
//         knn4 = unoptimized::run(points); // ≈ 270 ms to process 1000pts | O(N^2)
//         LOG(INFO) << timer.getDuration() << " ms : knn (!unoptimized!)";
//     }
//     return knn4;
// }
//