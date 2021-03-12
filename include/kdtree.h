#ifndef KDTREE_H
#define KDTREE_H

#include <map>
#include <vector>

#include "axis.h"
#include "node.h"
#include "point.h"

node* query(Point point, node* ptr_currentNearest, node* ptr_nextCandidate)
{
    /** return node closest to the pivot */
    if (ptr_currentNearest == nullptr) {
        return ptr_nextCandidate;
    }
    if (ptr_nextCandidate == nullptr) {
        return ptr_currentNearest;
    }

    float d1 = point.distance(ptr_currentNearest->m_point);
    float d2 = point.distance(ptr_nextCandidate->m_point);

    if (d1 < d2) {
        return ptr_currentNearest;
    }
    return ptr_nextCandidate;
}

node* nearest(node* ptr_root, Point point, int depth)
{
    if (ptr_root == nullptr) {
        return nullptr;
    }
    /** Iff our best nearest neighbour candidate is a nullptr or,
     *  Iff presiding node is a better nearest neighbour candidate:
     *  update ptr_nextBest */
    /** node with next best result */
    node* ptr_next = nullptr;
    node* ptr_opposite = nullptr;

    node* ptr_candidate = nullptr;
    node* ptr_best = nullptr;

    /** set axis based on depth axis R = 3 [ define in point.cpp ] */
    int axis = depth % R;

    /** pick subsequent left or right search node based on depth */
    switch (axis) {
    case 0:
        if (point.m_x < ptr_root->m_point.m_x) {
            ptr_next = ptr_root->m_left;
            ptr_opposite = ptr_root->m_right;
        } else {
            ptr_next = ptr_root->m_right;
            ptr_opposite = ptr_root->m_left;
        }

        /** nearest (node*, Point, int) return a best candidate nearest
         * neighbour query (Point, node*, node*) determines which node, i.e.,
         * between the current best candidate and the presiding root node. */
        ptr_candidate
            = query(point, nearest(ptr_next, point, depth + 1), ptr_root);

        if (point.distance(ptr_candidate->m_point)
            > abs(point.m_x) - ptr_root->m_point.m_x) {
            ptr_candidate = query(
                point, nearest(ptr_opposite, point, depth + 1), ptr_candidate);
        }
        ptr_best = ptr_candidate;

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
        ptr_best = ptr_candidate;

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
        ptr_best = ptr_candidate;
    default:
        break;
    }
    return ptr_best;
}

void show(node* ptr_root)
{
    /** in-order (lnr) traversal */
    if (ptr_root != nullptr) {
        show(ptr_root->m_left);
        std::cout << ptr_root->m_point << std::endl;
        show(ptr_root->m_right);
    }
}

node* tree(std::vector<Point>& points, int depth)
{
    /** base condition for recursive function */
    if (points.empty()) {
        return nullptr;
    }

    /** set axis based on depth axis R = 3 [ define in point.cpp ] */
    int axis = depth % R;

    /** sort point list by axis X, Y, Z */
    switch (axis) {
    case 0:
        axis::sortX(points);
    case 1:
        axis::sortY(points);
    case 2:
        axis::sortZ(points);
    default:
        break;
    }
    /** slice at axis medians */
    int median = (int)points.size() / 2;
    std::vector<Point> leftPoints;
    std::vector<Point> rightPoints;

    leftPoints = std::vector<Point>(points.begin(), points.begin() + median);
    rightPoints = std::vector<Point>(points.begin() + median + 1, points.end());

    /** recursively create tree nodes, viz, pre-oder (nlr) traversal */
    return new node(points[median], tree(leftPoints, depth + 1),
        tree(rightPoints, depth + 1));
}

namespace kdtree {

const int DEPTH = 0;

std::vector<float> run(std::vector<Point>& points)
{
    /** construct kdtree: n.b. root node recurse synonymous with kdtree */
    node* ptr_root = tree(points, DEPTH);
    // show(ptr_root);
    // -------------------------tested

    /** pointer to node with nearest neighbour */
    for (int i = 0; i < 5; i++) {
        node* ptr_nn = nearest(ptr_root, points[i], DEPTH);
        std::cout << ptr_nn->m_point.m_id << ": " << ptr_nn->m_point
                  << std::endl;
    }

    /** container for the four knn */
    std::vector<float> knn;

    return knn;
}
}
#endif /* KDTREE_H */
