#include "node_route_list.h"
#include <algorithm>
#include <stdexcept> // for std::out_of_range

void NodeRouteList::append(const NodeRoute& node) {
    nodeRouteList.push_back(node);
}

void NodeRouteList::replace(const NodeRoute& node) {
    auto it = std::find_if(nodeRouteList.begin(), nodeRouteList.end(), [&node](const NodeRoute& existingNode) {
        return existingNode.currentNode == node.currentNode;
    });

    if (it != nodeRouteList.end()) {
        // If the node exists, replace it if hop count is lower
        if (node.hopCount < it->hopCount)
            *it = node;
    } else {
        // If the node does not exist, append it
        nodeRouteList.push_back(node);
    }
}

void NodeRouteList::remove(int currentNode) {
    nodeRouteList.remove_if([currentNode](const NodeRoute& node) {
        return node.currentNode == currentNode;
    });
}

int NodeRouteList::getSize() const {
    return nodeRouteList.size();
}

NodeRoute NodeRouteList::getNode(int index) const {
    if (index < 0 || index >= getSize()) {
        throw std::out_of_range("Index out of range");
    }
    auto it = nodeRouteList.begin();
    std::advance(it, index);
    return *it;
}

int NodeRouteList::getWay(int index) const {
    return getNode(index).way;
}

int NodeRouteList::getHopCount(int index) const {
    return getNode(index).hopCount;
}

int NodeRouteList::getCurrentNode(int index) const {
    return getNode(index).currentNode;
}

NodeRoute NodeRouteList::getNodeByID(int currentNode) const {
    auto it = std::find_if(nodeRouteList.begin(), nodeRouteList.end(), [currentNode](const NodeRoute& node) {
        return node.currentNode == currentNode;
    });

    if (it != nodeRouteList.end()) {
        return *it;
    } else {
        throw std::out_of_range("Node with given currentNode not found");
    }
}