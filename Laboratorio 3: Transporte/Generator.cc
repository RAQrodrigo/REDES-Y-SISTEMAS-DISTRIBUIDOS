#ifndef GENERATOR
#define GENERATOR

#include <string.h>
#include <omnetpp.h>

using namespace omnetpp;

/*
// create new packet
cPacket "pkt;
pkt - new cPacket ("packet*);
pkt ->setByteLength (par ("packetByteSize")):
*/

class Generator : public cSimpleModule {
private:
    cPacket *sendPacketEvent;
    cStdDev transmissionStats;
    int packetsAmount;
public:
    Generator();
    virtual ~Generator();
protected:
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};
Define_Module(Generator);

Generator::Generator() {
    sendPacketEvent = NULL;
}

Generator::~Generator() {
    cancelAndDelete(sendPacketEvent);
}

void Generator::initialize() {
    transmissionStats.setName("TotalTransmissions");
    // create the send packet
    sendPacketEvent = new cPacket("sendPacketEvent");
    // schedule the first event at random time
    packetsAmount = 0;
    scheduleAt(par("generationInterval"), sendPacketEvent);
}

void Generator::finish() {
    recordScalar("Packets generated", packetsAmount);
}

void Generator::handleMessage(cMessage *msg) {

    if(msg == sendPacketEvent){
        // create new packet
        cPacket *pkt;
        pkt = new cPacket("pkt");
        // Set packet size based on configuration parameter
        pkt->setByteLength(par("packetByteSize"));
        // send to the output
        send(pkt, "out");
        packetsAmount++;

        

        // compute the new departure time
        simtime_t departureTime = simTime() + (par("generationInterval"));
        // schedule the new packet generation
        scheduleAt(departureTime, sendPacketEvent);
        //packetsPerSecond = par("packetsPerSecond").getIntValue
        //newSpeed = packetsPerSecond * 0.9
        //par("generationInterval").setDoubleValue(newSpeed); // Increase generation interval
    } 
}
/*
else if (msg == highBufferReached){
    packetsSentPerSecond = par("packetsPerSecond").getIntValue
    newSpeed = par("packetsPerSecond") * 1.5
    par("generationInterval").setDoubleValue(newSpeed)
} else if (msg == bufferLimitReached){
    packetsSentPerSecond = par("packetsPerSecond").getIntValue
    newSpeed = par("packetsPerSecond") * 2
    par("generationInterval").setDoubleValue(newSpeed)
}
*/

#endif /* GENERATOR */


/*
ChatGpt
packet FeedbackPkt {
    int feedbackValue;
    double timestamp;
}

Copilot
class FeedbackPacket : public cPacket {
    cpacket 
    msg
};

FeedbackPacket *pkt = new FeedbackPacket("feedback");
send(pkt, "out");
*/