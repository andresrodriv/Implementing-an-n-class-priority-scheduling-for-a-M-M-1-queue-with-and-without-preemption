#include <omnetpp.h>

using namespace omnetpp;


class Queue : public cSimpleModule
{
  protected:
    cMessage *msgServiced;//Message
    cMessage *endServiceMsg;//End of the Service time

    cQueue queue;
    //Default
    simsignal_t qlenSignal;
    simsignal_t busySignal;
    //Generic
    simsignal_t queueingTimeSignal;
    simsignal_t responseTimeSignal;
    //Per Class
    simsignal_t queueingTimeclass1Signal;
    simsignal_t responseTimeclass1Signal;
    simsignal_t extendedServiceTimeclass1Signal;

    simsignal_t queueingTimeclass2Signal;
    simsignal_t responseTimeclass2Signal;
    simsignal_t extendedServiceTimeclass2Signal;

    simsignal_t queueingTimeclass3Signal;
    simsignal_t responseTimeclass3Signal;
    simsignal_t extendedServiceTimeclass3Signal;

    simsignal_t queueingTimeclass4Signal;
    simsignal_t responseTimeclass4Signal;
    simsignal_t extendedServiceTimeclass4Signal;

    simsignal_t queueingTimeclass5Signal;
    simsignal_t responseTimeclass5Signal;
    simsignal_t extendedServiceTimeclass5Signal;

  public:
    Queue();
    virtual ~Queue();

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void Priority_Queue_Insertion(cMessage *msg);
    virtual void Priority_Server_Insertion(cMessage *msg);
    virtual void Zero_Time();
    virtual simtime_t Start_Service();
    virtual simtime_t Service_time();
};

Define_Module(Queue);


Queue::Queue()//Constructor
{
    msgServiced = endServiceMsg = nullptr;
}

Queue::~Queue()//Destructor
{
    delete msgServiced;
    cancelAndDelete(endServiceMsg);
}

void Queue::initialize()
{
    endServiceMsg = new cMessage("end-service");//Create the end of service message
    queue.setName("queue");//Set the name of the Queue

    //Register the signals//
    //Default
    qlenSignal = registerSignal("qlen");
    busySignal = registerSignal("busy");
    //Generic
    queueingTimeSignal = registerSignal("queueingTime");
    responseTimeSignal = registerSignal("responseTime");
    //Per Class
    queueingTimeclass1Signal = registerSignal("queueingTimeclass1");
    responseTimeclass1Signal = registerSignal("responseTimeclass1");
    extendedServiceTimeclass1Signal = registerSignal("extendedServiceTimeclass1");

    queueingTimeclass2Signal = registerSignal("queueingTimeclass2");
    responseTimeclass2Signal = registerSignal("responseTimeclass2");
    extendedServiceTimeclass2Signal = registerSignal("extendedServiceTimeclass2");

    queueingTimeclass3Signal = registerSignal("queueingTimeclass3");
    responseTimeclass3Signal = registerSignal("responseTimeclass3");
    extendedServiceTimeclass3Signal = registerSignal("extendedServiceTimeclass3");

    queueingTimeclass4Signal = registerSignal("queueingTimeclass4");
    responseTimeclass4Signal = registerSignal("responseTimeclass4");
    extendedServiceTimeclass4Signal = registerSignal("extendedServiceTimeclass4");

    queueingTimeclass5Signal = registerSignal("queueingTimeclass5");
    responseTimeclass5Signal = registerSignal("responseTimeclass5");
    extendedServiceTimeclass5Signal = registerSignal("extendedServiceTimeclass5");

    emit(qlenSignal, queue.getLength());//Emit the first values
    emit(busySignal, false);//Status of the server
}

void Queue::handleMessage(cMessage *msg)
{
    if (msg == endServiceMsg) { // Self-message arrived

        EV << "Completed service of " << msgServiced->getName() << endl;
        //Response time: time from msg arrival timestamp to time msg ends service (now)
        //Generic
        emit(responseTimeSignal, simTime() - msgServiced->getTimestamp());
        //Per Class
        switch (msgServiced->getArrivalGateId()){
            case 0:{//Class 1
                emit(responseTimeclass1Signal, simTime() - msgServiced->getTimestamp());
                emit(extendedServiceTimeclass1Signal, simTime() - (simtime_t)msgServiced->par("service_stamp2"));
                break;
            }
            case 2:{//Class 2
                emit(responseTimeclass2Signal, simTime() - msgServiced->getTimestamp());
                emit(extendedServiceTimeclass2Signal, simTime() - (simtime_t)msgServiced->par("service_stamp2"));
                break;
            }
            case 4:{//Class 3
                emit(responseTimeclass3Signal, simTime() - msgServiced->getTimestamp());
                emit(extendedServiceTimeclass3Signal, simTime() - (simtime_t)msgServiced->par("service_stamp2"));
                break;
            }
            case 6:{//Class 4
                emit(responseTimeclass4Signal, simTime() - msgServiced->getTimestamp());
                emit(extendedServiceTimeclass4Signal, simTime() - (simtime_t)msgServiced->par("service_stamp2"));
                break;
            }
            case 8:{//Class 5
                emit(responseTimeclass5Signal, simTime() - msgServiced->getTimestamp());
                emit(extendedServiceTimeclass5Signal, simTime() - (simtime_t)msgServiced->par("service_stamp2"));
                break;
            }
            default: break;
        }
        send(msgServiced, "out");

        if (queue.isEmpty()) { // Empty queue, server goes in IDLE

            EV << "Empty queue, server goes IDLE" <<endl;
            msgServiced = nullptr;
            emit(busySignal, false);

        }
        else { // Queue contains users

            msgServiced = (cMessage *)queue.pop();
            emit(qlenSignal, queue.getLength()); //Queue length changed, emit new length!

            //Waiting time: time from msg arrival to time msg enters the server (now)
            //Generic
            emit(queueingTimeSignal, simTime() - msgServiced->getTimestamp());
            //Per Class
            switch (msgServiced->getArrivalGateId()){
                case 0:{//Class 1
                    emit(queueingTimeclass1Signal, simTime() - msgServiced->getTimestamp());
                    break;
                }
                case 2:{//Class 2
                    emit(queueingTimeclass2Signal, simTime() - msgServiced->getTimestamp());
                    break;
                }
                case 4:{//Class 3
                    emit(queueingTimeclass3Signal, simTime() - msgServiced->getTimestamp());
                    break;
                }
                case 6:{//Class 4
                    emit(queueingTimeclass4Signal, simTime() - msgServiced->getTimestamp());
                    break;
                }
                case 8:{//Class 5
                    emit(queueingTimeclass5Signal, simTime() - msgServiced->getTimestamp());
                    break;
                }
                default: break;
            }
            //Setting a time stamp for the moment in which the message start the service
            simtime_t x = Start_Service();
            msgServiced->par("service_stamp") = x.dbl();
            if ((simtime_t)msgServiced->par("service_stamp2") == 0)
                msgServiced->par("service_stamp2") = x.dbl();
        }

    }
    else { // Data msg has arrived

        //Setting arrival timestamp as msg field
        msg->setTimestamp();
        //Setting a time stamp for the moment in which the message start the service
        msg->addPar("service_stamp")=0;
        msg->addPar("service_stamp2")=0;
        msg->addPar("service_diff")=0;
        msg->addPar("service_time")=0;

        if (!msgServiced) { //No message in service (server IDLE) ==> No queue ==> Direct service

            ASSERT(queue.getLength() == 0);

            msgServiced = msg;
            Zero_Time();
            //Setting a time stamp for the moment in which the message start the service
            simtime_t x = Start_Service();
            msgServiced->par("service_stamp") = x.dbl();
            msgServiced->par("service_stamp2") = x.dbl();

            emit(busySignal, true);
        }else {  //Message in service (server BUSY) ==> Queuing
            switch ((int)par("policy")){
                case 0:{//Non-preemptive policy
                    Priority_Queue_Insertion(msg);
                    break;
                }
                case 1:{//Preemptive-Restart Policy
                    if (msgServiced->getArrivalGateId() > msg->getArrivalGateId()){
                        EV << "Starting service of " << msg->getName() << endl << msgServiced->getName() << " returns to the queue" <<endl;
                        Priority_Server_Insertion(msg);
                        simtime_t serviceTime = Service_time();
                        //Reschedule the endServiceMsg this time for the higher priority message arrived
                        cancelEvent(endServiceMsg);
                        scheduleAt(simTime()+serviceTime, endServiceMsg);
                        //Here we assign the time stamp in the moment in which the message starts the service
                        simtime_t y = simTime();
                        msgServiced->par("service_stamp") = y.dbl();
                        msgServiced->par("service_stamp2") = y.dbl();
                    }else//In case there is a message with higher priority in service
                        Priority_Queue_Insertion(msg); //No need of {} is just one instruction (is based on c++)
                    break;
                }
                case 2:{//Preemptive-Resume Policy
                    if (msgServiced->getArrivalGateId() > msg->getArrivalGateId()){
                        EV << "Starting service of " << msg->getName() << endl << msgServiced->getName() << " returns to the queue" <<endl;

                        simtime_t service_stamp = (simtime_t)msgServiced->par("service_stamp");//Cast the parameter to its simtime_t type.
                        ASSERT(service_stamp != 0);//The parameter cannot be null since the message has already been in the server.

                        //Here we assign the time difference in the moment in which the message returns to the queue.
                        simtime_t x = simTime() - service_stamp;
                        msgServiced->par("service_diff") = x.dbl();

                        Priority_Server_Insertion(msg);
                        //During this function, msgServiced is changed to the new higher priority messaged arrived.

                        simtime_t serviceTime = Service_time();
                        msgServiced->par("service_time")=serviceTime.dbl();
                        //Reschedule the endServiceMsg this time for the higher priority message arrived
                        cancelEvent(endServiceMsg);
                        //Since the message is just arrived, the endServiceMessage is scheduled with the full service time
                        scheduleAt(simTime()+serviceTime, endServiceMsg);
                        //Here we assign the time stamp in the moment in which the message starts the service
                        simtime_t y = simTime();
                        msgServiced->par("service_stamp") = y.dbl();
                        msgServiced->par("service_stamp2") = y.dbl();

                    }else//In case there is a message with higher priority in service
                        Priority_Queue_Insertion(msg); //No need of {} is just one instruction (is based on c++)
                    break;
                }
                default: break;
            }
            emit(qlenSignal, queue.getLength()); //Queue length changed, emit new length!
       }
    }
}//end of handleMessage

//This function inserts the arriving message in the queue according to its priority.
void Queue::Priority_Queue_Insertion(cMessage *msg){
    int i, n = (int) queue.length();
    cMessage *msg_in_queue = nullptr;
    for (i = 0; i < n; i++){
        msg_in_queue = (cMessage *)queue.get(i);
        if (msg_in_queue->getArrivalGateId() > msg->getArrivalGateId()){
            EV << msg->getName() << " enters the queue, inserted before " << msg_in_queue->getName() << endl;
            queue.insertBefore(msg_in_queue, msg);
            break;
        }
    }
    if (!msg_in_queue || i==n){
        EV << msg->getName() << " enters queue, at the end"<< endl;
        queue.insert(msg);
    }
    msg_in_queue = nullptr;
    delete msg_in_queue;
}// end of the priority insertion

//This function inserts the higher message directly in the server and reinserts the previous lower priority message that were in service.
void Queue::Priority_Server_Insertion(cMessage *msg){
    if(queue.front())//Send the message that was being served to the front of the queue
        queue.insertBefore(queue.front(), msgServiced);
    else//In case the queue is empty
        queue.insert(msgServiced);
    //In case there is a user with lower priority in the server, the arrived user waits zero in the queue
    msgServiced = msg;// msgServiced is a global variable for Queue.
    Zero_Time();
}//end Server_Insertion

//This function emits a waiting time equal to zero, for the general and the specific class signal
void Queue::Zero_Time(){
    //Generic
    emit(queueingTimeSignal, SIMTIME_ZERO);
    //Per Class
    switch (msgServiced->getArrivalGateId()){
        case 0:{//Class 1
            emit(queueingTimeclass1Signal, SIMTIME_ZERO);
            break;
        }
        case 2:{//Class 2
            emit(queueingTimeclass2Signal, SIMTIME_ZERO);
            break;
        }
        case 4:{//Class 3
            emit(queueingTimeclass3Signal, SIMTIME_ZERO);
            break;
        }
        case 6:{//Class 4
            emit(queueingTimeclass4Signal, SIMTIME_ZERO);
            break;
        }
        case 8:{//Class 5
            emit(queueingTimeclass5Signal, SIMTIME_ZERO);
            break;
        }
        default: break;
    }
}//end Zero_Time

//This function start the service of the message.
simtime_t Queue::Start_Service(){
    EV << "Starting service of " << msgServiced->getName() <<endl;
    simtime_t service_diff =(simtime_t)msgServiced->par("service_diff");//Is necessary to cast the void* (returned by getContext) to its original type.
    //We set the service time according to the time of the context pointer in case of resume
    if (service_diff != 0 && (int)par("policy") == 2){//If the message has already entered the service and if the policy is preemptive resume
        /* *service_diff, gets the value of the time in which the message was in service before it gets out.
        The value of service_diff is assigned in the Preemtive-resume Policy*/
        scheduleAt(simTime() + (simtime_t)msgServiced->par("service_time") - service_diff, endServiceMsg);
        //Here we assign the time stamp in the moment in which the message starts the service
        simtime_t x = simTime();
        return x;
    }else{
        simtime_t serviceTime = Service_time();
        msgServiced->par("service_time")=serviceTime.dbl();
        scheduleAt(simTime()+serviceTime, endServiceMsg);
        //Here we assign the time stamp in the moment in which the message starts the service
        simtime_t x = simTime();//que tipo de variable me retorna simtime????
        return x;
    }
}//end Start_Service

//This function returns the correspondent service time for the correspondent class.
simtime_t Queue::Service_time(){
    switch (msgServiced->getArrivalGateId()){
        case 0:return par("serviceTimeclass1");break;
        case 2:return par("serviceTimeclass2");break;
        case 4:return par("serviceTimeclass3");break;
        case 6:return par("serviceTimeclass4");break;
        case 8:return par("serviceTimeclass5");break;
        default: break;
    }
}//end Service_time
