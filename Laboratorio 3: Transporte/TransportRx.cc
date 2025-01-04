#ifndef TRANSPORTRX
#define TRANSPORTRX

#include <string.h>
#include <omnetpp.h>
#include "FeedbackPkt_m.h"

using namespace omnetpp;

class TransportRx : public cSimpleModule
{
private:
    bool slowQueueIndicator;//Flag to check if the queue is slowed
    bool fbPktSent;// Flag to check if a feedback packet was sent
    cQueue buffer;
    //Parameters for the statistics about the simulation
    cOutVector bufferSizeVector;
    cOutVector bufferPorcentageVector;
    cOutVector packetDropVector;
    cOutVector feedbackSendVector;
    int feedbackPktSent;
    cMessage *endServiceEvent;
    cMessage *bufferReachingLimitEvent;// Event to notify that the buffer is reaching its limit
    simtime_t serviceTime;

public:
    TransportRx();
    virtual ~TransportRx();

protected:
    int bufferSize;
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(TransportRx);

TransportRx::TransportRx()
{
    endServiceEvent = nullptr;
}

TransportRx::~TransportRx()
{
    cancelAndDelete(endServiceEvent);
}

void TransportRx::initialize()
{
    buffer.setName("buffer");
    bufferSize = par("bufferSize").intValue();
    endServiceEvent = new cMessage("endService");
    slowQueueIndicator = false;
    fbPktSent = false;
    bufferReachingLimitEvent = new cMessage("bufferReachingLimit");
    //Parameters for the statistics about the simulation
    bufferSizeVector.setName("RxBufferSize");
    bufferPorcentageVector.setName("RxRemainingBufferPorcentage");
    packetDropVector.setName("RxPacketDrop");
    feedbackSendVector.setName("RxFeedbackSend");

    bufferSizeVector.record(0);
    bufferPorcentageVector.record(100);
    packetDropVector.record(0);
    feedbackSendVector.record(0);

    feedbackPktSent = 0;
    
    
}

void TransportRx::finish()
{
}

void TransportRx::handleMessage(cMessage *msg)
{
    if (msg->getKind() == 2)
    {//If the message is a feedback we set the slowQueueIndicator to true and
     // use it to send feedback packets to the next module   
        slowQueueIndicator = true;
        EV << "RECEIVED FEEDBACK PACKET" << endl;
    }
    else if (msg->getKind() == 0)
    {
        if (msg == endServiceEvent)
        {
            if (!buffer.isEmpty())
            {
                //Calculate the remaining buffer percentage
                double remainingCapacityPercentage = (double)(this->bufferSize - buffer.getLength()) / this->bufferSize * 100;
                EV << "RX: Remaining capacity percentage: " << remainingCapacityPercentage << endl;
                bufferSizeVector.record(buffer.getLength());
                bufferPorcentageVector.record(remainingCapacityPercentage);

                // Check if the buffer is reaching its limit, 
                //we considerated that the buffer is reaching its limit when it is less than 20%
                if ((remainingCapacityPercentage < 20 || slowQueueIndicator) && !fbPktSent)
                {//In case the buffer is reaching its limit or the previus module is reaching its limit,
                 // we send a feedback packet notifying of this
                    FeedbackPkt *fbPkt = new FeedbackPkt();
                    fbPkt->setKind(2);
                    fbPkt->setByteLength(20);
                    fbPkt->setRemainingBuffer(remainingCapacityPercentage);
                    send(fbPkt, "toOut$o");
                    feedbackPktSent++;
                    feedbackSendVector.record(feedbackPktSent);
                    fbPktSent = true;
                    //Log for simulation
                    EV << "RX: Sending feedback packet" << endl;
                    this->bubble("Buffer is almost full");
                    serviceTime = fbPkt->getDuration();
                    scheduleAt(simTime() + serviceTime, endServiceEvent);
                }
                else
                {
                    cPacket *pkt = (cPacket *)buffer.pop();
                    send(pkt, "toApp");
                    serviceTime = pkt->getDuration();
                    scheduleAt(simTime() + serviceTime, endServiceEvent);
                    //Reset the flags
                    if(fbPktSent){
                        fbPktSent = false;
                    }
                    if(slowQueueIndicator){
                        slowQueueIndicator = false;
                    }
                }
            }
        }
        else
        {
            if (buffer.getLength() >= bufferSize)
            {
                delete msg;
                EV << "RX: packet droped" << endl;
                packetDropVector.record(1);
            }
            else
            {
                buffer.insert(msg);

                if (!endServiceEvent->isScheduled())
                {
                    scheduleAt(simTime(), endServiceEvent);
                }
            }
        }
    }
}

#endif /* TRANSPORTRX */
