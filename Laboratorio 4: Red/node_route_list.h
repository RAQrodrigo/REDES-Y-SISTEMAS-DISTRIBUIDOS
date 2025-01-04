#ifndef NODE_ROUTE_LIST_H
#define NODE_ROUTE_LIST_H

#include <iostream>
#include <list>
#include "node_route.h"

class NodeRouteList {
public:
    void append(const NodeRoute& node);
    void replace(const NodeRoute& node);
    void remove(int currentNode);
    int getSize() const; // Getter to get the size of the list
    NodeRoute getNode(int index) const; // Getter to retrieve the node at a specified index
    NodeRoute getNodeByID(int currentNode) const; // Function to get a node matching a given currentNode
    int getWay(int index) const; // Getter to get the way of the node at a specified index
    int getHopCount(int index) const; // Getter to get the hop count of the node at a specified index
    int getCurrentNode(int index) const; // Getter to get the current node of the node at a specified index

private:
    std::list<NodeRoute> nodeRouteList;
};

#endif // NODE_ROUTE_LIST_H
