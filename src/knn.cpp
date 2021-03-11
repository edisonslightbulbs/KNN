#include <algorithm>
#include <cmath>

#include "io.h"
#include "kdtree.h"
#include "knn.h"
#include "naive.h"
#include "timer.h"
#include "logger.h"

std::vector<Point> seed(std::vector<Point> points)
{
    const int SAMPLE_SIZE = 12;

    /** find centroid */
    Point center = Point::centroid(points);

    /** compute distances from centroid for each point */
    for (auto& point : points) {
        float distance = point.distance(center);
        point.m_distance.second = distance;
    }

    /** sort points using computed distance */
    Point::sort(points);

    /** sample edge of cluster points */
    std::vector<Point> sample = std::vector<Point>(
            points.begin() + points.size() - SAMPLE_SIZE, points.end());

    /** visual check of sample size */
    std::cout << "sampled " << sample.size() << " points" << std::endl;

    return sample;
}

std::vector<float> compute(std::vector<Point>& points)
{
    /** create container for Kth nearest neighbours of all points */
    std::vector<float> knn4;

    /** benchmarking ********************************************************************/
    {
        Timer timer;
        //knn4 = naive::run(points); // <-- ~ ?? ms (100pts) | ~ 260 ms (1000pts)  ... O(N^2)
        LOG(INFO) << timer.getDuration() << " ms : knn (naive) analysis";
    }
    {
        Timer timer;
        knn4 = kdtree::run(points); // <-- ~ ?? 101 ms O(n log n)
        LOG(INFO) << timer.getDuration() << " ms : knn (kd tree) analysis";
    }

    /** benchmarking ********************************************************************/

    /** output 4th nearest neighbour of all points */
    io::csv(knn4);
    return knn4;
}

float knn::elbow(std::vector<Point>& points)
{
    Timer timer;
    /** sample points (todo: revise sampling strategy)  */
    std::vector<Point> sample(seed(points));

    /** distance of 4th nearest neighbour including core point */
    std::vector<float> nn4 = compute(sample);
    std::sort(nn4.begin(), nn4.end(), std::greater<>());

    /** use Snell's Law to evaluate relative angular distances */
    std::vector<std::pair<float, float>> angles;
    float vec2 = 0;
    for (int i = 1; i < nn4.size() - 1; i++) {
        vec2 = nn4[i + 1] - nn4[i];
        float angle = std::asin(vec2);
        angles.emplace_back(nn4[i], angle);
    }

    /** find the maximum absolute second derivative (masd) */
    float masd = 0;
    for (int i = 1; i < angles.size() - 1; i++) {
        float d1 = angles[i - 1].second - angles[i].second;
        float d2 = angles[i + 1].second - angles[i].second;
        float cd = 2 * angles[i].second;

        /** evaluate the second derivative (sd) - central difference (cd) */
        float sd = d1 + d2 - cd;

        /** determine elbow as it corresponds to the distance with the masd */
        if (sd > masd) {
            masd = angles[i].first;
        }
    }
    return masd;
}
