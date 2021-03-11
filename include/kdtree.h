#ifndef KDTREE_H
#define KDTREE_H

#include <map>
#include <vector>

#include "point.h"

static void sortx(std::vector<Point>& points)
{
    std::sort(points.begin(), points.end(),
              [](const Point& point, const Point& other) {
                  return point.m_x < other.m_x;
              });
}

static void sorty(std::vector<Point>& points)
{
    std::sort(points.begin(), points.end(),
              [](const Point& point, const Point& other) {
                  return point.m_y < other.m_y;
              });
}

static void sortz(std::vector<Point>& points)
{
    std::sort(points.begin(), points.end(),
              [](const Point& point, const Point& other) {
                  return point.m_z < other.m_z;
              });
}

struct node {
    Point m_point;
    node* m_left = nullptr;
    node* m_right = nullptr;

    node(Point& point, node* ptr_left, node* ptr_right)
            : m_point(point)
            , m_left(ptr_left)
            , m_right(ptr_right)
    {
    }
};

std::vector<float> nearest(node* root, Point point, int d, std::vector<float> distances)
{
    if (root == nullptr) {
        return distances;
    }
    int axis = d % R;
    node* next = nullptr;
    //node* opposite = nullptr;

    // std::cout << "---------------------------------------------" << std::endl;
    // std::cout << root->m_point << std::endl;

    float distance = point.distance(root->m_point);

    /** sort point list by axis */
    switch (axis) {
        case 0:
            std::cout << "x" << std::endl;
            if (point.m_x < root->m_point.m_x) {
                next = root->m_left;
                //opposite = root->m_right;
                distances.insert(distances.begin(), distance);
            } else {
                next = root->m_right;
                //opposite = root->m_left;
                distances.insert(distances.begin(), distance);
            }
        case 1:
            std::cout << "y" << std::endl;
            if (point.m_y < root->m_point.m_y) {
                next = root->m_left;
                //opposite = root->m_right;
                distances.insert(distances.begin(), distance);
            } else {
                next = root->m_right;
                //opposite = root->m_left;
                distances.insert(distances.begin(), distance);
            }
        case 2:
            std::cout << "z" << std::endl;
            if (point.m_z < root->m_point.m_z) {
                next = root->m_left;
                //opposite = root->m_right;
                distances.insert(distances.begin(), distance);
            } else {
                next = root->m_right;
                //opposite = root->m_left;
                distances.insert(distances.begin(), distance);
            }
        default:
            break;
    }

    return nearest(next, point, d + 1, distances);
}

node* tree(std::vector<Point>& points, int d)
{
    /** base condition for recursive function */
    if (points.empty()) {
        return nullptr;
    }

/** set axis based on depth axis R = 3 [ define in point.cpp ] */
    int axis = d % R;

    /** sort point list by axis */
    switch (axis) {
        case 0:
            sortx(points);
        case 1:
            sorty(points);
        case 2:
            sortz(points);
        default:
            break;
    }

    /** find median */
    int median = (int)points.size() / 2;

    std::vector<Point> lVec;
    std::vector<Point> rVec;

    /** slice at median appropriately */
    if (points.size() >= 3) {
        lVec = std::vector<Point>(points.begin(), points.begin() + median);
        rVec = std::vector<Point>(points.begin() + median + 1, points.end());
    } else if (points.size() == 2) {
        lVec.push_back(points[0]);
        rVec.push_back(points[1]);
    }

    /** recursively create tree nodes */
    return new node(points[median], tree(lVec, d + 1), tree(rVec, d + 1));
}

namespace kdtree {

    const int K = 5;
    std::vector<float> run(std::vector<Point>& points)
    {
        /** construct kdtree */
        node* root = tree(points, 0);

        std::cout << "---------------------------------------------" << std::endl;
        int count = 0;
        for (const auto& point : points){
            std::cout << count << ": " << point << std::endl;
            count ++;
        }
        std::cout << "---------------------------------------------" << std::endl;
        std::cout << "root at tree is: " << root->m_point << std::endl;
        int mean = (int) points.size()/2;
        std::cout <<  "mean in vector is: " << points[mean] << std::endl;
        std::cout << "---------------------------------------------" << std::endl;


        /** container for the four closest distances */
        std::vector<float> nn(K);
        std::vector<float> knn;

        for(auto& point: points){
            std::vector<float> distance = nearest(root, point, 0, nn);
            knn.push_back(distance[4]);
        }
        return knn;
    }
}
#endif /* KDTREE_H */

