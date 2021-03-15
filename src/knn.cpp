#include "io.h"
#include "kdtree.h"
#include "knn.h"
#include "naive.h"
#include "timer.h"
#include "logger.h"

std::vector<float> Knn::compute(std::vector<Point>& points)
{
    /** create container for Kth nearest neighbours of all points */
    std::vector<float> knn4;
    {
        Timer timer;
        knn4 = unoptimized::run(points); // ≈ 270 ms to process 1000pts | O(N^2)
        LOG(INFO) << timer.getDuration() << " ms : knn (!unoptimized!)";
    }
    // {
    //     Timer timer;
    //     kdtree::run(points); // <-- ~ 101 ms O(n log n)
    //     LOG(INFO) << timer.getDuration() << " ms : knn (kd build) analysis";
    // }

    // io::knns(knn4);
    return knn4;
}
