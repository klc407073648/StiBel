#include <iostream>
#include <sstream>
#include <map>
#include "StiBel/Data/Redis/Redis.h"

using namespace StiBel::Data::Redis;
using namespace std;

void fun1()
{

    std::map<std::string, std::string> params;
    //数据库的参数
    params["host"] = "127.0.0.1:6379";
    params["passwd"] = "klczxas741789";
    params["timeout"] = "100";
    params["name"] = "redis_1";
    
    Redis *m_redis = new Redis(params);
    if (!m_redis->connect())
    {
        std::cout << "connect fail" << std::endl;
        return;
    }

    cout << "查询studentInfo表的结果集" << endl;

    m_redis->cmd("SET %s %s", "name", "Jason");
    m_redis->cmd("SET %s %s", "num", "123456");
    m_redis->cmd("SET %s %s", "address", "NJ");

    ReplyPtr m_ptr = m_redis->cmd("GET %s ", "address");
    cout << m_ptr->str << endl;
    cout << "——————————————————————————————" << endl;
}

void fun2()
{
    //
    std::map<std::string, std::map<std::string, std::string> > m_configMap;
    std::map<std::string, std::string> m_map1,m_map2;
    //数据库的参数
    m_map1["host"]="127.0.0.1:6379";
    m_map1["type"]="redis";
	m_map1["passwd"]="klczxas741789";
	m_map1["timeout"]="100";
	m_map1["name"]="redis_1";
    m_map1["pool"]="2";
    m_configMap["redis_1"]=m_map1;

    m_map2["host"]="127.0.0.1:6380";
    m_map2["type"]="redis";
	//m_map2["passwd"]="klczxas741789";
	m_map2["timeout"]="100";
	m_map2["name"]="redis_2";
    m_map2["pool"]="2";
    m_configMap["redis_2"]=m_map2;


    //redis_1操作
    RedisManager *m_RedisMgr=new RedisManager(m_configMap);
	
	//redis_2操作
    IRedis::ptr m=m_RedisMgr->get("redis_1");

    m->cmd("SET %s %s", "time", "20210423");
	m->cmd("SET %s %s", "home", "NJ");
	m->cmd("SET %s %s", "book", "IRedis");
	m->cmd("SET %s %s", "music", "Love");
    ReplyPtr m_ptr = m->cmd("GET %s ", "time");
    cout << m_ptr->str << endl;
	
	//
	IRedis::ptr m2=m_RedisMgr->get("redis_2");

    ReplyPtr m_ptr2 = m2->cmd("GET %s ", "book");
    cout << m_ptr2->str << endl;
	m2->cmd("SET %s %s", "book","ISQL");
	m_ptr2 = m2->cmd("GET %s ", "book");
    cout << m_ptr2->str << endl;
	m_ptr2 = m2->cmd("GET %s ", "music");
    cout << m_ptr2->str << endl;
	std::stringstream ss;
	m_RedisMgr->dump(ss);
	
	cout << ss.str() << endl;
}

int main(int argc, char **argv)
{
    fun2();
    return 0;
}

/*
[root@VM-0-10-centos build]# ../deploy/ToolClass/redis/redisTest
m_host:127.0.0.1,m_port:6379,m_passwd:klczxas741789,m_logEnable:1
m_host:127.0.0.1,m_port:6379,m_passwd:klczxas741789,m_logEnable:1
m_host:127.0.0.1,m_port:6380,m_passwd:,m_logEnable:1
m_host:127.0.0.1,m_port:6380,m_passwd:,m_logEnable:1
20210423
IRedis
redisCommand error: (SET %s %s)(127.0.0.1:6380)(redis_2): READONLY You can't write against a read only replica.IRedis
Love
[RedisManager total=2]
    redis_1 :[{host:127.0.0.1:6379}{name:redis_1}{passwd:klczxas741789}{pool:2}{timeout:100}{type:redis}]
    redis_2 :[{host:127.0.0.1:6380}{name:redis_2}{pool:2}{timeout:100}{type:redis}]

*/
