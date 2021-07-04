//
//   Request-reply service in C++
//   Connects REP socket to tcp://localhost:5560
//   Expects "Hello" from client, replies with "World"
//
// Olivier Chamoux <olivier.chamoux@fr.thalesgroup.com>


#include "zhelpers.hpp"
#include "StiBel/Data/MySQL/MySQL.h"
#include "StiBel/Util.h"
#include "StiBel/JSON/JsonUtil.h"
#include <vector>

using namespace StiBel::Data::MySQL;
using namespace StiBel::JSON;
using StiBel::StringUtil;

MySQL::ptr _mySql=NULL;

void connMySql()
{
	if(_mySql!=NULL)
		return;
    std::map<std::string, std::string> params;
    //数据库的参数
    params["host"] = "1.15.109.169";
    params["user"] = "root";
    params["port"] = "3307";
    params["passwd"] = "123456";
    params["dbname"] = "student";

    MySQL::ptr mysql(new MySQL(params));
    if(!mysql->connect()) {
        std::cout << "connect fail" << std::endl;
        return;
    }
    _mySql=mysql;
}
 
int main (int argc, char *argv[])
{
    zmq::context_t context(1);

	zmq::socket_t responder(context, ZMQ_REP);
	//responder.connect("tcp://localhost:5560");
	responder.connect("ipc://databaseresponse.ipc");
	
	connMySql();
	
	while(1)
	{
		//  Wait for next request from client
		std::string string = s_recv (responder);
		
		std::cout << "Received request: " << string << std::endl;
		std::cout << "Received request: " << JsonUtil::getStr(string,"method") << std::endl;
		std::cout << "Received request: " << JsonUtil::getStr(string,"url") << std::endl;
		
		std::vector<std::string> pairslist = StringUtil::mySplit(JsonUtil::getStr(string,"url"), "/");
		
		std::string sql1 = "select * from ";
		std::string retStr="";
		if(pairslist.size()==2)
		{
			std::cout << "pairslist: " << pairslist[0]<<" "<<pairslist[1] << std::endl;
			
			ISQLData::ptr m_ptr=_mySql->query("show tables");
			
			std::vector<std::string> tableList;
			bool isExist=false;
			
			while(m_ptr->next())
			{
				for(int j=0; j < m_ptr->getColumnCount(); ++j)
				{
					tableList.push_back(m_ptr->getString(j));
				}
			}
			
			for(int i=0;i<tableList.size();i++)
			{
				std::cout << "tableList: " << tableList[i]<<std::endl;
			}
			
			std::cout << "tableList test" <<std::endl;
			
			std::vector<std::string>::iterator s=find(tableList.begin(),tableList.end(),pairslist[1]);
			
			if( s !=tableList.end())//找到
			{
				std::cout<<*s<<std::endl;
				isExist=true;
			}
			else
			{
				retStr="not find table";
			}
			
			
			if(pairslist[0]=="database"&&isExist)
			{
				sql1=sql1+pairslist[1];
				ISQLData::ptr m_ptr1=_mySql->query(sql1);
				
				for(int i=0;i<m_ptr1->getColumnCount();i++)
				{
					retStr=retStr+m_ptr1->getColumnName(i)+" ";
				}
				retStr=retStr+"\n";
				
				while(m_ptr1->next())
				{
					for(int j=0; j < m_ptr1->getColumnCount(); ++j)
					{
						retStr=retStr+m_ptr1->getString(j)+" ";
					}
					retStr=retStr+"\n";
				}
			}
		}
		
		// Do some 'work'
        
		std::cout<< "retStr"<<retStr<<std::endl;
        //  Send reply back to client
		s_send (responder, retStr);
		
	}
}

