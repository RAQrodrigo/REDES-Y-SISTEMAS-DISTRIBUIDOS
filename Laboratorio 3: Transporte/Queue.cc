#ifndef QUEUE
#define QUEUE

#include <string.h>
#include <omnetpp.h>
#include "FeedbackPkt_m.h"

using namespace omnetpp;


class Queue : public cSimpleModule
{
private:
    //Parameters for the statistics about the simulation
    cOutVector bufferSizeVector;
    cOutVector bufferPorcentageVector;
    cOutVector packetDropVector;
    cOutVector feedbackSendVector;

    int feedbackPktSent;// Number of feedback packets sent
    bool fbPktSent;// Flag to check if a feedback packet was sent
    cQueue buffer;
    cMessage *endServiceEvent;
    cMessage *bufferReachingLimitEvent;// Event to notify that the buffer is reaching its limit
    simtime_t serviceTime;

public:
    Queue();
    virtual ~Queue();

protected:
    int bufferSize;
    virtual void initialize();
    virtual void finish();
    virtual void handleMessage(cMessage *msg);
};

Define_Module(Queue);

Queue::Queue()
{
    endServiceEvent = NULL;
}

Queue::~Queue()
{
    cancelAndDelete(endServiceEvent);
}

void Queue::initialize()
{
    buffer.setName("buffer");
    bufferSize = par("bufferSize").intValue();
    endServiceEvent = new cMessage("endService");
    fbPktSent = false;
    bufferReachingLimitEvent = new cMessage("bufferReachingLimit");
    //Parameters for the statistics about the simulation
    bufferSizeVector.setName("bufferSizeVector");
    bufferPorcentageVector.setName("QueueRemainingBufferPorcentageVector");
    packetDropVector.setName("QueuePacketDropVector");
    feedbackSendVector.setName("feedbackSendVector");
    bufferSizeVector.record(0);
    bufferPorcentageVector.record(100);
    packetDropVector.record(0);
    feedbackSendVector.record(0);
    feedbackPktSent = 0;
}

void Queue::finish()
{
}

void Queue::handleMessage(cMessage *msg)
{
    if (msg->getKind() == 2)
    {
        delete msg;
        //Calculate and send the remaining buffer percentage
        EV << "QUEUE: Feedback packet received" << endl;
        FeedbackPkt *fbPkt = new FeedbackPkt();
        fbPkt->setByteLength(20);
        fbPkt->setKind(2);
        double remainingCapacityPercentage = (double)(this->bufferSize - buffer.getLength()) / this->bufferSize * 100;
        fbPkt->setRemainingBuffer(remainingCapacityPercentage);
        send(fbPkt, "out");
    }
    else if (msg->getKind() == 0)
    {
        if (msg == endServiceEvent)
        {
            if (!buffer.isEmpty())
            {   //Calculate the remaining buffer percentage
                double remainingCapacityPercentage = (double)(this->bufferSize - buffer.getLength()) / this->bufferSize * 100;
                //Log for simulation
                EV << "QUEUE: Remaining capacity percentage: " << remainingCapacityPercentage << endl;
                bufferSizeVector.record(buffer.getLength());
                bufferPorcentageVector.record(remainingCapacityPercentage);

                // Check if the buffer is reaching its limit, 
                //we considerated that the buffer is reaching its limit when it is less than 20%
                if ((remainingCapacityPercentage < 20) && !fbPktSent)
                {//In case the buffer is reaching its limit, we send a feedback packet notifying of this
                    FeedbackPkt *fbPkt = new FeedbackPkt();
                    fbPkt->setByteLength(20);
                    fbPkt->setKind(2);
                    fbPkt->setRemainingBuffer(remainingCapacityPercentage);
                    send(fbPkt, "out");
                    feedbackPktSent++;//Vector for statistics
                    feedbackSendVector.record(feedbackPktSent);
                    fbPktSent = true;// Set the flag to true
                    serviceTime = fbPkt->getDuration();
                    //Log for simulation
                    EV << "QUEUE: Sending feedback packet" << endl;
                    scheduleAt(simTime() + serviceTime, endServiceEvent);
                }
                else
                {   //If the buffer is not reaching its limit, we send the packet normally
                    cPacket *pkt = (cPacket *)buffer.pop();
                    send(pkt, "out");
                    serviceTime = pkt->getDuration();
                    scheduleAt(simTime() + serviceTime, endServiceEvent);
                    if(fbPktSent){//If we had sent a feedback packet, we set the flag to false
                        fbPktSent = false;
                    }
                }
            }
        }
        else
        {
            if (buffer.getLength() >= bufferSize)
            {
                delete msg;
                //Log for simulation
                EV << "QUEUE: packet droped" << endl;
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

#endif 
