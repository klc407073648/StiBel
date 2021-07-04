#include "rrBroker.h"

CREATE_STATIC_LOGGERMANAGER(rrBroker);

rrBroker::rrBroker()
{
    LogDebug("rrBroker::rrBroker()");
    init();
}

rrBroker::~rrBroker()
{
    LogDebug("rrBroker::~rrBroker()");
    delete context;
    delete frontend;
    delete backend;
}

void rrBroker::init()
{
    LogDebug("rrBroker::init() bgein");
    context = new zmq::context_t(1);
    frontend = new zmq::socket_t(*context, ZMQ_ROUTER);
    backend = new zmq::socket_t(*context, ZMQ_DEALER);

    //frontend.bind("tcp://*:5559");
    //backend.bind("tcp://*:5560");

    frontend->bind("ipc://databaserequest.ipc");
    backend->bind("ipc://databaseresponse.ipc");

    //front2back.insert(pair<std::string, std::string>("ipc://databaserequest.ipc","ipc://databaseresponse.ipc"));
    front2back.push_back("ipc://databaserequest.ipc");
    front2back.push_back("ipc://databaseresponse.ipc");
    LogDebug("rrBroker::init() end");
}

void rrBroker::start()
{
    LogDebug("rrBroker::start() start");

    zmq::pollitem_t items[] = {
        {*frontend, 0, ZMQ_POLLIN, 0},
        {*backend, 0, ZMQ_POLLIN, 0}};

    //  Switch messages between sockets
    while (1)
    {
        zmq::message_t message;
        int64_t more; //  Multipart detection

        zmq::poll(&items[0], 2, -1);

        if (items[0].revents & ZMQ_POLLIN)
        {
            while (1)
            {
                //  Process all parts of the message
                // 通过判断more是否为0,来判断是否为最后一帧消息
                frontend->recv(&message);
                size_t more_size = sizeof(more);
                frontend->getsockopt(ZMQ_RCVMORE, &more, &more_size);

                LogDebug("rrBroker::start() frontend:[%s] recv message[%s],send to backend:[%s]", (front2back[0]).c_str(), (message.to_string()).c_str(), (front2back[1]).c_str());

                backend->send(message, more ? ZMQ_SNDMORE : 0);

                if (!more)
                    break; //  Last message part
            }
        }
        if (items[1].revents & ZMQ_POLLIN)
        {
            while (1)
            {
                //  Process all parts of the message
                backend->recv(&message);
                size_t more_size = sizeof(more);
                backend->getsockopt(ZMQ_RCVMORE, &more, &more_size);

                LogDebug("rrBroker::start() backend:[%s] recv message[%s],send to frontend:[%s]", (front2back[1]).c_str(), (message.to_string()).c_str(), (front2back[0]).c_str());

                frontend->send(message, more ? ZMQ_SNDMORE : 0);
                if (!more)
                    break; //  Last message part
            }
        }
    }

    LogDebug("rrBroker::start() end");
}
