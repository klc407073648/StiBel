#include "Event.h"
#include "memory.h"
#include <unistd.h>
#include <fcntl.h>

//#define THREAD_COUNT 20
//static int counts[THREAD_COUNT];
CREATE_STATIC_LOGGERMANAGER(Event);

Event::Event(int threadNum):
    m_iThreadNum(threadNum)
{
    
}

Event::~Event()
{

}

void Event::addEvent(string requestEvent, handlerFunc func)
{
    m_EventMap.insert(EventMap::value_type(requestEvent, func));
    LogInfo("add %s to m_EventMap",requestEvent.c_str());
}

void Event::exec()
{
    if(m_iThreadNum > 1)
    {
        //多线程模式
        LogInfo("multiple thread ,num=%d",m_iThreadNum);
        pthread_t id[m_iThreadNum];
        FCGX_Init();
        for (int i = 1; i < m_iThreadNum; i++)
        {
            threadStruct p;
            p.pthis = this;
            p.threadID = i;
            pthread_create(&id[i], NULL, pthreadTask, (void*)&p);
        }
        processMessage(0);
    }
    else
    {
        //单线程模式
        while(FCGI_Accept() >= 0)
        {
            LogInfo("Single thread enter",m_iThreadNum);
             //"GET" or "POST"
            string requestMethod =  string(getenv("REQUEST_METHOD"));
            //"/login?key=value&name=pwd"
            string requestUri = string(getenv("REQUEST_URI"));
            size_t pos = requestUri.find_first_of("?");
            string requestEvent = requestUri.substr(0,pos);
			
            string requestParam;
            if("GET" == requestMethod)
            {
                if(-1 != pos)
                {
                    requestParam = requestUri.substr(pos+1);
                }
            }
            else if("POST" == requestMethod)
            {
                int len = atoi(getenv("CONTENT_LENGTH")) + 1;
                char *bufpost = (char* )malloc(len);
                memset(bufpost, 0, len);
                FCGI_fread(bufpost, len, 1, FCGI_stdin);
				requestParam = string(bufpost);
                free(bufpost);
            }
		
            EventMap::iterator iterMap;
            iterMap = m_EventMap.find(requestEvent);
            if(iterMap == m_EventMap.end())
            {
                LogError("not find Event: %s",requestEvent.c_str());
            }
            else
            {
                LogInfo("find Event: %s",requestEvent.c_str());
                handlerFunc func = iterMap->second;
                Request req;
                req.setMethod(requestMethod);
                req.setUrl(requestEvent);
                req.setParams(requestParam);
                req.setToken("");
                 
                Response res = func(req);
                FCGI_printf(res.Out().c_str()); 
            }
        }
    }
}

void Event::processMessage(int threadID)
{
//    pid_t pid = getpid();
    LogInfo("threadID: %d is running", threadID);
    int rc;
    FCGX_Request request;
    FCGX_InitRequest(&request, 0, 0);
    for(;;)
    {
        static pthread_mutex_t accept_mutex = PTHREAD_MUTEX_INITIALIZER;

        pthread_mutex_lock(&accept_mutex);
        rc = FCGX_Accept_r(&request);
        pthread_mutex_unlock(&accept_mutex);
        if (rc < 0)
            break;

        std::string httpCookie;
        if(FCGX_GetParam("HTTP_COOKIE", request.envp))
            httpCookie = std::string(FCGX_GetParam("HTTP_COOKIE", request.envp)); 

        //"GET" or "POST"
        std::string requestMethod;
        if(FCGX_GetParam("REQUEST_METHOD", request.envp))
            requestMethod = std::string(FCGX_GetParam("REQUEST_METHOD", request.envp));

        //"/login?key=value&name=pwd"
        std::string requestUri;
        if(FCGX_GetParam("REQUEST_URI", request.envp))
            requestUri = std::string(FCGX_GetParam("REQUEST_URI", request.envp));

		LogInfo("httpCookie: %s,requestMethod: %s,requestUri: %s",httpCookie.c_str(),requestMethod.c_str(),requestUri.c_str());

        size_t pos = requestUri.find_first_of("?");
        string requestEvent = requestUri.substr(0,pos);
        string requestParam;
        if("GET" == requestMethod)
        {
            if(-1 != pos)
            {
                requestParam = requestUri.substr(pos+1);
            }
        }
        else if("POST" == requestMethod)
        {
            if(FCGX_GetParam("CONTENT_LENGTH", request.envp))
            {
                int ilen = atoi(FCGX_GetParam("CONTENT_LENGTH", request.envp)) + 1;
                char *bufpost = (char* )malloc(ilen);
                memset(bufpost, 0, ilen);
                FCGX_GetStr(bufpost,ilen,request.in);
                requestParam = string(bufpost);
                free(bufpost);
            }
        }

        EventMap::iterator iterMap;
        iterMap = m_EventMap.find(requestEvent);
        if(iterMap == m_EventMap.end())
        {
             LogError("not find Event: %s",requestEvent.c_str());
        }
        else
        {
            LogInfo("find Event: %s",requestEvent.c_str());
            handlerFunc func = iterMap->second;
            Request req;
            req.setMethod(requestMethod);
            req.setUrl(requestEvent);
            req.setParams(requestParam);
            req.setToken("");
            Response res = func(req);

            FCGX_PutS(res.Out().c_str(), request.out);

            //LogInfo("res.Out: %s", res.Out().c_str());
        }
//        sleep(2);
//        pthread_mutex_lock(&counts_mutex);
//        DBG(L_DEBUG, "counts: %5d , PID: %d, threadID: %d" , ++counts[threadID], pid, threadID);
//        pthread_mutex_unlock(&counts_mutex);
        
        FCGX_Finish_r(&request);
    }
}

