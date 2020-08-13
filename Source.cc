#include <omnetpp.h>

using namespace omnetpp;


class Source : public cSimpleModule
{
  private:
    cMessage *sendMessageEvent;
    int nbGenMessages;

  public:
    Source();
    virtual ~Source();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

Define_Module(Source);

Source::Source()
{
    sendMessageEvent = nullptr;
}

Source::~Source()
{
    cancelAndDelete(sendMessageEvent);
}

void Source::initialize()
{
    nbGenMessages = 0;
    sendMessageEvent = new cMessage("sendMessageEvent");//The pinter is equal to a new message
    scheduleAt(simTime(), sendMessageEvent);//To deliver the message to the module
}

void Source::handleMessage(cMessage *msg)
{
    ASSERT(msg == sendMessageEvent);//confirmation that the recived message is the sended message
    char msgname[40];
    sprintf(msgname,"message-%d class-%d",++nbGenMessages,(int)msg->getId()+1);
    cMessage *message = new cMessage(msgname);
    send(message, "out");//the current message is sended out

    scheduleAt(simTime()+par("interArrivalTime").doubleValue(), sendMessageEvent);//It generates a new message according to the interarrival time set at omntepp.ini
}
