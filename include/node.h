#ifndef NODE_H
#define NODE_H

#include "point.h"

class node {

public:
    Point m_point;
    node* m_left = nullptr;
    node* m_right = nullptr;

    node();

    node(Point& point, node* ptr_left, node* ptr_right)
        : m_point(point)
        , m_left(ptr_left)
        , m_right(ptr_right)
    {
    }
};
#endif /* NODE_H */
