#ifndef KDTREE_H
#define KDTREE_H

#include <map>
#include <vector>

#include "axis.h"
#include "node.h"
#include "point.h"

node* query(Point point, node* ptr_nodeA, node* ptr_nodeB)
{
    /** base case: return whichever is not null */
    if (ptr_nodeA == nullptr) {
        return ptr_nodeB;
    }
    if (ptr_nodeB == nullptr) {
        return ptr_nodeA;
    }
    /** find the node that corresponds to the shortest distance from the given
     * point */
    float d1 = point.distance(ptr_nodeA->m_point);
    float d2 = point.distance(ptr_nodeB->m_point);
    if (d1 < d2) {
        return ptr_nodeA;
    }
    return ptr_nodeB;
}

node* nearest(node* ptr_root, Point point, int depth)
{
    /** base case */
    if (ptr_root == nullptr) {
        return nullptr;
    }
    node* ptr_next = nullptr;
    node* ptr_opposite = nullptr;
    node* ptr_candidate = nullptr;

    /** for a point, use the depth and root node
     *  to find the nearest neighbours ... axis-wise search */
    int axis = depth % R;
    switch (axis) {
    case 0:
        if (point.m_x < ptr_root->m_point.m_x) {
            ptr_next = ptr_root->m_left;
            ptr_opposite = ptr_root->m_right;
        } else {
            ptr_next = ptr_root->m_right;
            ptr_opposite = ptr_root->m_left;
        }
        ptr_candidate
            = query(point, nearest(ptr_next, point, depth + 1), ptr_root);

        if (point.distance(ptr_candidate->m_point)
            > abs(point.m_x) - ptr_root->m_point.m_x) {
            ptr_candidate = query(
                point, nearest(ptr_opposite, point, depth + 1), ptr_candidate);
        }

    case 1:
        if (point.m_y < ptr_root->m_point.m_y) {
            ptr_next = ptr_root->m_left;
            ptr_opposite = ptr_root->m_right;
        } else {
            ptr_next = ptr_root->m_right;
            ptr_opposite = ptr_root->m_left;
        }
        ptr_candidate
            = query(point, nearest(ptr_next, point, depth + 1), ptr_root);

        if (point.distance(ptr_candidate->m_point)
            > abs(point.m_y) - ptr_root->m_point.m_y) {
            ptr_candidate = query(
                point, nearest(ptr_opposite, point, depth + 1), ptr_candidate);
        }

    case 2:
        if (point.m_z < ptr_root->m_point.m_z) {
            ptr_next = ptr_root->m_left;
            ptr_opposite = ptr_root->m_right;
        } else {
            ptr_next = ptr_root->m_right;
            ptr_opposite = ptr_root->m_left;
        }
        ptr_candidate
            = query(point, nearest(ptr_next, point, depth + 1), ptr_root);

        if (point.distance(ptr_candidate->m_point)
            > abs(point.m_z) - ptr_root->m_point.m_z) {
            ptr_candidate = query(
                point, nearest(ptr_opposite, point, depth + 1), ptr_candidate);
        }
    default:
        break;
    }
    /** return  nearest neighbour */
    return ptr_candidate;
}

void print(node* ptr_root)
{
    /** in-order (lnr) traversal */
    if (ptr_root != nullptr) {
        print(ptr_root->m_left);
        std::cout << ptr_root->m_point << std::endl;
        print(ptr_root->m_right);
    }
}

node* build(std::vector<Point>& points, int depth)
{
    /** base condition */
    if (points.empty()) {
        return nullptr;
    }

    /** sort axis based on depth and modular arithmetic [ R = 3 ] */
    int axis = depth % R;
    switch (axis) {
    case 0:
        axis::sortXAxis(points);
    case 1:
        axis::sortYAxis(points);
    case 2:
        axis::sortZAxis(points);
    default:
        break;
    }

    /** slice sorted points at corresponding median */
    std::vector<Point> leftPoints;
    std::vector<Point> rightPoints;
    int median = (int)points.size() / 2;
    leftPoints = std::vector<Point>(points.begin(), points.begin() + median);
    rightPoints = std::vector<Point>(points.begin() + median + 1, points.end());

    /** recursively build tree using pre-oder traversal (nlr) */
    return new node(points[median], build(leftPoints, depth + 1),
        build(rightPoints, depth + 1));
}

namespace kdtree {

std::vector<float> run(std::vector<Point>& points)
{
    /** initial depth */
    const int DEPTH = 0;

    /** build kdtree, [ pre-order traversal ]
     * n.b., root node holds entire tree */
    node* ptr_root = build(points, DEPTH);

    /** print tree [ in-order traversal ]*/
    // show(ptr_root);

    /** find nearest neighbour */
    node* ptr_nn = nearest(ptr_root, points[0], DEPTH);

    /** return the 4th nearest neighbour */
    std::vector<float> knn;

    return knn;
}
}
#endif /* KDTREE_H */
