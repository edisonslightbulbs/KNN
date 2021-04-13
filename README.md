#### Example implementation of tiny knn interface using [nanoflann](https://github.com/jlblancoc/nanoflann)

Massive thanks to [Jose](https://github.com/jlblancoc) for sharing this adaptation of [flann](https://github.com/mariusmuja/flann) 👏🍻🍻

## Quick Start

###### CMakeLists.txt

```txt
cmake_minimum_required(VERSION 3.11)
project(knn)

# optimization for dev environments
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# project version
set(MAJOR 0)
set(MINOR 1)
set(REMAP_VERSION ${MAJOR}.${MINOR})

# CXX and C versions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_FLAGS "-pthread")

# cmake modules path spec
list(APPEND CMAKE_MODULE_PATH "$ENV{CMAKE_MODULE_PATH}")
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

# cmake binary path spec
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# optimization
option(OPTIMIZE_CODEBASE "Clang-tidy checks during build" OFF)
if(OPTIMIZE_CODEBASE)
        set(CMAKE_CXX_CLANG_TIDY
            clang-tidy -p pwd
            --checks=-*,readability-*,clang-analyzer-*,performance-*)

    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-g")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3")
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# project paths
set(PROJECT_DIR ${CMAKE_CURRENT_LIST_DIR})
set(SRC_DIR ${PROJECT_DIR}/src)

# *.hpp & *.h paths
# find header directories and collect
# them in the $INCLUDE_DIRS variable
set (INCLUDE_DIRS "")
file(GLOB_RECURSE HEADERS
    ${PROJECT_DIR}/include/*.h
    ${PROJECT_DIR}/nanoflann/*.h
    ${PROJECT_DIR}/nanoflann/*.hpp
    )
foreach (HEADER ${HEADERS})
    get_filename_component(DIR ${HEADER} PATH)
    list (APPEND INCLUDE_DIRS ${DIR})
endforeach()
list(REMOVE_DUPLICATES INCLUDE_DIRS)

# *.cpp paths
file(GLOB_RECURSE LIBS_SRC ${SRC_DIR}/*.cpp)

# add executables
add_executable(knn
    ${LIBS_SRC}
    ${SRC_DIR}/main.cpp
    )

# include directories
target_include_directories(knn PRIVATE
    ${INCLUDE_DIRS}
    )

# auto run target
option(RUN "execute target" ON) # (default ON)
if(RUN)
    add_custom_command(TARGET knn
        POST_BUILD
        COMMAND ./build/bin/knn
        WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR})
endif()
```


###### main.cpp

```cpp
#include <vector>
#include <iostream>
#include <cmath>
#include <unistd.h>
#include <fstream>
#include <sstream>

#include "knn.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-result"
std::string pwd()
{
    char buff[FILENAME_MAX];
    getcwd(buff, FILENAME_MAX);
    std::string workingDir(buff);
    return workingDir;
}
#pragma GCC diagnostic pop

std::vector<knn::t_point> read(std::vector<knn::t_point> points, const std::string& file)
{
    /** create input stream and string for parsing file data */
    std::ifstream data(file);
    std::string line;

    /** assign each parsed point and id starting from 1 */
    int id = 1;

    /** while un-parsed lines exist ... */
    while (std::getline(data, line)) {

        /** ... get next line in file */
        std::stringstream ss(line);
        std::string cell;
        std::vector<std::string> row;

        /** ... parse each line based on delimiter */
        while (std::getline(ss, cell, ' ')) {
            row.push_back(cell);
        }

        /** create Point type definitions */
        knn::t_point point {};
        point.x = (float)std::stof(row[0]);
        point.y = (float)std::stof(row[1]);
        point.z = (float)std::stof(row[2]);
        points.push_back(point);
        id++;
    }
    return points;
}

std::vector<knn::t_point> readPlyFile()
{
    std::vector<knn::t_point> points;
    const std::string file = pwd() + "/context.ply";
    points = read(points, file);
    return points;
}

/** un-used and only place here to indicate how
 *  nanoflann evaluates distances using the
 *  squared result of evaluated L2 norms */
float distance(knn::t_point point, knn::t_point other)
{
    float x = point.x - other.x;
    float y = point.y - other.y;
    float z = point.z - other.z;
    float solution = (float)(std::sqrt((x * x) + (y * y) + (z * z)))*(std::sqrt((x * x) + (y * y) + (z * z)));
    return solution;
}

int main()
{
    /** read-in and parse point cloud (example) */
    std::vector<knn::t_point> points = readPlyFile();

    /** use nanoflann to query and collect the
     *  Nth nearest neighbours of every point. */
    int k = 5;
    std::vector<std::pair<knn::t_point, float>> bucket5thNn;
    int indexOfQueryPoint = 5;
    std::vector<std::pair<knn::t_point, float>> nn = knn::compute(points, k, indexOfQueryPoint, bucket5thNn);

    for(int i = 0; i < nn.size(); i++){
        std::cout << "#" << i << ",\t"
                  << "dist: " << std::sqrt(nn[i].second) << ",\t"
                  << "point: ("
                  << nn[i].first.x << ", "
                  << nn[i].first.y << ", "
                << nn[i].first.z << ") "<< std::endl;
    }
    return 0;
}
```

to collect the Nth nearest neighbours of all points.

```cpp
    /** query and collect Kth nearest neighbours of all points. */
    std::vector<float> kthNn;

    const int sample = points.size();
    for(int i = 0; i < sample; i++){
        int indexOfQueryPoint = i;
        std::vector<std::pair<knn::t_point, float>> nn
                = knn::compute(points, k, indexOfQueryPoint, bucket2NthNn);
        kthNn.push_back(std::sqrt(nn[i].second));
    }

```

##### PLY file with 3D points is delimited by spaces  (not provided)

Simply create your own data points making sure the `x, y, z,` values are space
delimited as shown below.
    
```ply
12 -219 1387
 9 -219 1387
 9 -216 1387
.   .    .
.   .    .
.   .    .
```
