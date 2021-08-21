#include "Server.h"
#include <iostream>
#include <unistd.h>
#include <string>

using StiBel::Log::LoggerManager;
using namespace std;

int main(int argc, char *argv[])
{
    LoggerManager::start("../conf/log4cpp_webServer.conf");

    int threadNum = 4;
    int port = 9950;
    int queueSize = 65535;

    int opt;
    const char *str = "t:p:q:";
    while ((opt = getopt(argc, argv, str)) != -1)
    {
        switch (opt)
        {
        case 't':
        {
            threadNum = atoi(optarg);
            break;
        }
        case 'p':
        {
            port = atoi(optarg);
            break;
        }
        case 'q':
        {
            queueSize = atoi(optarg);
            break;
        }
        default:
            break;
        }
    }

    cout << "threadNum: " << threadNum << ", port: " << port << ", queueSize: " << queueSize << endl;

    Server myHTTPServer(threadNum, port, queueSize);
    myHTTPServer.start();
    return 0;
}