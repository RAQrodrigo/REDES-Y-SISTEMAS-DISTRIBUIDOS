#ifndef NET
#define NET

#include <string.h>
#include <omnetpp.h>
#include <packet_m.h>
#include <hpacket_m.h>

#include "node_route_list.h"
#include "node_route.h"

using namespace omnetpp;

// right and left constants
#define RIGHT_DIR 0
#define LEFT_DIR 1

class Net : public cSimpleModule
{
private:
    NodeRouteList routeList;

public:
    Net();
    virtual ~Net();

protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Net);

#endif /* NET */

Net::Net()
{
}

Net::~Net()
{
}

void Net::initialize()
{
    // Hello packet if its node 0

    NodeRoute node = {this->getParentModule()->getIndex(), 0, RIGHT_DIR};
    routeList.append(node);

    hPacket *rightHelloPkt = new hPacket();
    rightHelloPkt->setByteLength(par("packetByteSize"));
    rightHelloPkt->setSource(this->getParentModule()->getIndex());
    rightHelloPkt->setDestination((this->getParentModule()->getIndex()));
    rightHelloPkt->setHopTimes(0);
    rightHelloPkt->setKind(2);
    rightHelloPkt->setDirection(RIGHT_DIR);
    send(rightHelloPkt, "toLnk$o", RIGHT_DIR);

    hPacket *leftHelloPkt = new hPacket();
    leftHelloPkt->setByteLength(par("packetByteSize"));
    leftHelloPkt->setSource(this->getParentModule()->getIndex());
    leftHelloPkt->setDestination((this->getParentModule()->getIndex()));
    leftHelloPkt->setDirection(LEFT_DIR);
    leftHelloPkt->setHopTimes(0);
    leftHelloPkt->setKind(2);
    send(leftHelloPkt, "toLnk$o", LEFT_DIR);
}

void Net::finish()
{
}

void Net::handleMessage(cMessage *msg)
{

    // All msg (events) on net are packets
    Packet *pkt = (Packet *)msg;

    // if its a hello packet
    if (pkt->getKind() == 2)
    {
        // if its my own hello packet, return
        if (pkt->getDestination() == this->getParentModule()->getIndex())
        {
            hPacket *helloPkt = (hPacket *)pkt;

            for (int i = 0; i < helloPkt->getNodeHopListsArraySize(); i++)
            {
                // append each node in routeList
                NodeRoute node = {helloPkt->getNodeHopLists(i).currentNode, helloPkt->getNodeHopLists(i).hopCount, helloPkt->getDirection()};
                // if it exist, append, else check hop count, if its lower, replace
                routeList.replace(node);
            }
            delete helloPkt;
            return;
        }

        // cast to hPacket

        hPacket *helloPkt = (hPacket *)pkt;

        helloPkt->setHopTimes(helloPkt->getHopTimes() + 1);

        NodeHop *node = new NodeHop();
        node->currentNode = this->getParentModule()->getIndex();
        node->hopCount = helloPkt->getHopTimes();
        node->way = helloPkt->getDirection();

        helloPkt->insertNodeHopLists(helloPkt->getNodeHopListsArraySize(), *node);

        // print node list ev
        delete node;

        send(helloPkt, "toLnk$o", helloPkt->getDirection());
        return;
    }
    else if (pkt->getKind() == 0)
    {
        // If this node is the final destination, send to App
        if (pkt->getDestination() == this->getParentModule()->getIndex())
        {
            send(msg, "toApp$o");
        }
        // If not, forward the packet to some else... to who?
        else
        {
            int direction;
            NodeRoute node;
            node = routeList.getNodeByID(pkt->getDestination());
            direction = node.way;
            send(msg, "toLnk$o", direction);

        }
    }
}
