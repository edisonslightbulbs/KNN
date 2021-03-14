#include "io.h"
#include "kdtree.h"
#include "knn.h"
#include "naive.h"
#include "timer.h"
#include "logger.h"

std::vector<float> knn::compute(std::vector<Point>& points)
{
    /** create container for Kth nearest neighbours of all points */
    std::vector<float> knn4;
    {
        Timer timer;
        knn4 = naive::run(points); // <-- ~ ?? ms (100pts) | ~ 260 ms (1000pts)  ... O(N^2)
        LOG(INFO) << timer.getDuration() << " ms : knn (naive) analysis";
    }
    {
        Timer timer;
        kdtree::run(points); // <-- ~ 101 ms O(n log n)
        LOG(INFO) << timer.getDuration() << " ms : knn (kd tree) analysis";
    }
    io::csv(knn4);
    return knn4;
}
