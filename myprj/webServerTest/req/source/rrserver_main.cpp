#include "rrServer.h"

using StiBel::Log::LoggerManager;

int main(int argc, char *argv[])
{
	LoggerManager::start("../conf/log4cpp.conf");

	rrServer *m_rrServer = new rrServer();
	m_rrServer->start();
	return 0;
}
