// Define a structure to hold currentNode and hopCount
struct NodeHop {
    int currentNode;
    int hopCount;
    int way;
}

// Define a message packet with additional fields
packet hPacket {
    int source; // Source node
    int destination; // Destination node
    int hopTimes; // Number of hops
    int direction; // Direction of movement

    // List of lists containing currentNode and hopCount
    NodeHop nodeHopLists[]; // List of lists containing currentNode and hopCount
}