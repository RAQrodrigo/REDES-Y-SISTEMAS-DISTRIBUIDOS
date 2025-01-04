#ifndef TRANSPORTTX
#define TRANSPORTTX

#include <string.h>
#include <omnetpp.h>
#include "FeedbackPkt_m.h"

using namespace omnetpp;

class TransportTx : public cSimpleModule
{
private:
    cQueue buffer;
    bool slowQueueIndicator;
    cMessage *endServiceEvent;
    float DecrementSendRatio;
    int ResetRatio;
    simtime_t serviceTime;
    cOutVector sendRatioVector;

public:
    TransportTx();
    virtual ~TransportTx();

protected:
    int bufferSize;
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportTx);

TransportTx::TransportTx()
{
    endServiceEvent = NULL;
}

TransportTx::~TransportTx()
{
    cancelAndDelete(endServiceEvent);
}

void TransportTx::initialize()
{
    buffer.setName("buffer");
    bufferSize = par("bufferSize").intValue();
    endServiceEvent = new cMessage("endService");
    slowQueueIndicator = false;
    DecrementSendRatio = 1;
    ResetRatio = 0;
    sendRatioVector.setName("SendRatio");
    sendRatioVector.record(DecrementSendRatio);
}

void TransportTx::finish()
{
}

void TransportTx::handleMessage(cMessage *msg) {

    if (msg->getKind() == 2) {
        EV << "============TX: Slow queue indicator RECEIVED =============" << endl;
        slowQueueIndicator = true;//Some module is running out of buffer space
        //We solve this by slowing down the transmission, so it doesn't matter
        //wich module is running out of buffer space specifically
        delete msg;
        return;
    }

    if (msg == endServiceEvent) {
        if (!buffer.isEmpty()) {
            cPacket *pkt = (cPacket *)buffer.pop();
            send(pkt, "toOut$o");
            serviceTime = pkt->getDuration();

            if (slowQueueIndicator) {// If some module is running out of buffer space we slow down the transmission
                //For each msg notifing that the buffer is running out of space we decrease the send ratio
                DecrementSendRatio = DecrementSendRatio + 0.1;
                scheduleAt(simTime() + (serviceTime * DecrementSendRatio), endServiceEvent);
                EV << "============TX: SLOWER QUEUE=============" << endl;
                EV << "SendRatio: " << DecrementSendRatio << endl;
                slowQueueIndicator = false;
                ResetRatio = 0;
            } else {
                //If no module is running out of buffer space we increase the send ratio after sending 50 packets,
                //by waiting this period we give time for the buffers to gain space,
                //and we increase it linearly by the same amount we decreased it until its reseted
                scheduleAt(simTime() + (serviceTime * DecrementSendRatio), endServiceEvent);
                ResetRatio++;
                //Log for simulation
                EV << "SendRatio Slow false: " << DecrementSendRatio << endl;
                if(ResetRatio >= 50 && DecrementSendRatio > 1.1){
                    DecrementSendRatio = DecrementSendRatio - 0.1;
                }
                //By dinamically decreasing the send ratio we avoid the buffer to run out of space
                //and by dinamically increasing when theres no feedback we avoid to lose too much efiency
            }
            sendRatioVector.record(DecrementSendRatio);
        }
    } else {
        buffer.insert(msg);
        if (!endServiceEvent->isScheduled()){
            scheduleAt(simTime(), endServiceEvent);
        }
    }
}

#endif /* TRANSPORTTX */
