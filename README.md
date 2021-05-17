#### Example use for this tiny knn lib based on [nanoflann](https://github.com/jlblancoc/nanoflann)

Massive thanks to [Jose](https://github.com/jlblancoc) for sharing this adaptation of [flann](https://github.com/mariusmuja/flann) 👏🍻🍻

In this example we find the 5th nearest neighbour of the first five to every other point. Typically, the Nth nearest neighbour of each point to every other point needs to be computed:
an expensive operation  even with flann's impressive optimizations.

```
int main()
{
    int k = 5;                                // K value
    std::vector<float> knnQuery;              // L2 distances to Kth neighbours
    std::vector<Point> points = readPoints(); // get points
    const int indexes = 5;                    // arbitrary val for illustration

    for (int i = 0; i < indexes; i++) {
        int indexOfQueryPoint = i;

        std::vector<std::pair<Point, float>> nn
            = knn::compute(points, k, indexOfQueryPoint);

        knnQuery.push_back(
            std::sqrt(nn[i].second)); // nanoflann uses squared L2 vals

        std::cout << "#" << i << ",\t"
                  << "dist: " << std::sqrt(nn[i].second) << ",\t"
                  << "point: (" << nn[i].first.m_xyz[0] << ", "
                  << nn[i].first.m_xyz[1] << ", " << nn[i].first.m_xyz[2]
                  << ") " << std::endl;
    }

    /** for plotting:
     * sort the distance in descending order and output results */
    std::sort(knnQuery.begin(), knnQuery.end(), std::greater<>());
    const std::string file = pwd() + "/knn.csv";
    std::cout << file << std::endl;
    writePoints (knnQuery, file);
    return 0;
}

```

The `Point` Class used in this example given below:
```
struct Point {
    std::array<float, 3> m_xyz {};
};
```

The `readPoints` and `writePoints` functions are adaptable implementations and therefore left out.
