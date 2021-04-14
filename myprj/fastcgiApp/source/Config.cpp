#include "Config.h"
#include "json/json.h"
#include <iostream>  
#include <fstream> 
#include "Common.h"

using namespace StiBel::Log;

CREATE_STATIC_LOGGERMANAGER(Config);

Config::Config(const std::string& name,const std::string& description):
m_name(name),m_description(description)
{
	Priority mPriority= Priority::DEBUG;
	mlogger->setRootPriority(mPriority);
	/*
	if(FileUtil::isFile(name))
	{
		m_type = name.substr( find_last_of(".")+1);

		if(m_type="json")
		{
			readConfFromJson();
		}
	}
	
	*/
    readConfFromJson(name);
	LogDebug("Config()");
}

Config::~Config()
{

}

std::string Config::getParams(const std::string &name)
{
    if ( name != "" && m_mapParams.find(name) != m_mapParams.end() )
        return m_mapParams[name];
    else
        return std::string("");
}

std::string Config::toString()
{
    std::string retStr;
    ParamsList::iterator iterMap = m_mapParams.begin();
    for(; iterMap != m_mapParams.end(); iterMap++)
    {
        retStr.append(iterMap->first);
        retStr.append("=");
        retStr.append(iterMap->second);
        retStr.append("&");
    }
    return retStr.substr(0,retStr.length() - 1);
}

void Config::readConfFromJson(const std::string& json_file)
{
	Json::Reader reader;
	Json::Value root;
 
	//从文件中读取，保证当前文件有demo.json文件  
	ifstream in(json_file, ios::binary);
 
	if (!in.is_open())
	{
		LogError("Error opening file");
		return;
	}
 
	if (reader.parse(in, root))
	{
	   //TODO 循环解析json文件里的array
       string mysql_host = root["mysql"]["host"].asString();
       string mysql_user = root["mysql"]["user"].asString();
       string mysql_passwd = root["mysql"]["passwd"].asString();
       string mysql_dbname = root["mysql"]["dbname"].asString();

	   string redis_ip = root["redis"]["ip"].asString();
       string redis_port = root["redis"]["port"].asString();
       string redis_passwd = root["redis"]["passwd"].asString();
       string redis_expire_time = root["redis"]["expire_time"].asString();

	   string log4cpp_conf = root["log4cpp"]["conf"].asString();

	   string html_path = root["html_path"].asString();

	   string upload_path = root["upload_path"].asString();
	   
       string json_name = root["name"].asString();
       string json_description = root["description"].asString();
       string json_type = root["type"].asString();
       
	   m_mapParams.insert(map<string, string>::value_type("mysql_host",mysql_host));
	   m_mapParams.insert(map<string, string>::value_type("mysql_user",mysql_user));
	   m_mapParams.insert(map<string, string>::value_type("mysql_passwd",mysql_passwd));
	   m_mapParams.insert(map<string, string>::value_type("mysql_dbname",mysql_dbname));

	   m_mapParams.insert(map<string, string>::value_type("redis_ip",redis_ip));
	   m_mapParams.insert(map<string, string>::value_type("redis_port",redis_port));
	   m_mapParams.insert(map<string, string>::value_type("redis_passwd",redis_passwd));
	   m_mapParams.insert(map<string, string>::value_type("redis_expire_time",redis_expire_time));

	   m_mapParams.insert(map<string, string>::value_type("log4cpp_conf",log4cpp_conf));

	   m_mapParams.insert(map<string, string>::value_type("html_path",html_path));

	   m_mapParams.insert(map<string, string>::value_type("upload_path",upload_path));

	   m_mapParams.insert(map<string, string>::value_type("json_name",json_name));
	   m_mapParams.insert(map<string, string>::value_type("json_description",json_description));
	   m_mapParams.insert(map<string, string>::value_type("json_type",json_type));
	   

	   map<string, string>::iterator iter;

	   LogInfo("m_mapParams begin:");
	   for(iter = m_mapParams.begin(); iter != m_mapParams.end(); iter++){
			LogInfo("%s:%s",(iter->first).c_str(),(iter->second).c_str());
		}
	   LogInfo("m_mapParams end");

	   LogInfo("Reading Complete!");
	}
	else
	{
        LogError("parse error!");
	}
 
	in.close();
}
