#include "StiBel/Config.h"
#include <iostream>

using namespace std;
using StiBel::Config;

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
    Config m_Config("my_config_test",m_configMap);
	
	
    cout << "m_Config->toString():"<<m_Config.toString() << endl;
	string str="redis_2";
	cout << "m_Config[redis_2][host]:"<<(m_Config.getParams(str))["host"] << endl;
	std::map<std::string, std::string> m_map3=m_Config[str];
	cout << "m_Config[redis_2][timeout]:"<<m_map3["timeout"] << endl;
	
	/*
	Config *m_Config=new Config("my_config_test",m_configMap);
	
	
    cout << "m_Config->toString():"<<m_Config->toString() << endl;
	string str="redis_2";
	cout << "m_Config[redis_2][host]:"<<(m_Config->getParams(str))["host"] << endl;
	std::map<std::string, std::string> m_map3=m_Config->[str];//指针就无法使用了
	cout << "m_Config[redis_2][timeout]:"<<m_map3["timeout"] << endl;
	*/
}
/*
[root@VM-0-10-centos new]# ./ConfigTest
m_Config->toString():{key: redis_1 , values: [host:127.0.0.1:6379,name:redis_1,passwd:klczxas741789,pool:2,timeout:100,type:redis]},{key: redis_2 , values: [host:127.0.0.1:6380,name:redis_2,pool:2,timeout:100,type:redis]}
m_Config[redis_2][host]:127.0.0.1:6380
m_Config[redis_2][timeout]:100

*/

int main(int argc, char **argv)
{
    fun2();
    return 0;
}
