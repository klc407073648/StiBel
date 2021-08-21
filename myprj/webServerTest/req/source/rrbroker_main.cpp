#include "rrBroker.h"

using StiBel::Log::LoggerManager;

int main(int argc, char *argv[])
{
    LoggerManager::start("../conf/log4cpp_broker.conf");

    rrBroker *m_rrBroker = new rrBroker();
    m_rrBroker->start();
    return 0;
}
