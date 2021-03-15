#ifndef NODE_H
#define NODE_H

#include "point.h"

class Node {

public:
    Point m_point;
    Node* m_left = nullptr;
    Node* m_right = nullptr;

    Node();

    Node(Point& point, Node* ptr_left, Node* ptr_right)
        : m_point(point)
        , m_left(ptr_left)
        , m_right(ptr_right)
    {
    }
};
#endif /* NODE_H */
