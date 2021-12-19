#include "rrServer.h"

CREATE_STATIC_LOGGERMANAGER(rrServer);

MySQL::ptr _mySql = NULL;

rrServer::rrServer()
{
  LogDebug("rrServer::rrServer()");
  init();
}

rrServer::~rrServer()
{
  LogDebug("rrServer::~rrServer()");
  delete context;
  delete responder;
}

void rrServer::init()
{
  LogDebug("rrServer::init() bgein");
  context = new zmq::context_t(1);
  responder = new zmq::socket_t(*context, ZMQ_REP);

  responder->connect("ipc://databaseresponse.ipc");

  connMySql();

  LogDebug("rrServer::init() end");
}

std::string execShellPipe(std::string cmd)
{
	char res[1024]={0},*p;
	 
	FILE *fp = popen(cmd.c_str(),"r");
	
	if( fp != NULL)
	{
		fgets( res, sizeof(res), fp ); //遇到\n终止复制
		if((p = strchr(res,'\n')) != NULL)
			*p = '\0';
		//fread( res, sizeof(char), sizeof(res), fp );
		pclose(fp);
	}
	
	return res;
}

void rrServer::connMySql()
{
  if (_mySql != NULL)
    return;

  std::map<std::string, std::string> params;
  //数据库的参数

  //需要通过配置文件读取
  string curPath = execShellPipe("pwd");
  string fullPath = curPath + "/../conf/" + "mysql.yml";
  LogDebug("[connMySql] fullPath:[%s]", fullPath.c_str());
  YAML::Node config = YAML::LoadFile(fullPath);
  
  cout << "name:" << config["name"].as<string>() << endl;
  cout << "sex:" << config["sex"].as<string>() << endl;
  
  params["host"] = config["mysql"]["host"].as<string>();
  params["user"] = config["mysql"]["user"].as<string>();
  params["port"] = config["mysql"]["port"].as<string>();
  params["passwd"] = config["mysql"]["passwd"].as<string>();
  params["dbname"] = config["mysql"]["dbname"].as<string>();

  for(std::map<std::string, std::string>::iterator iter = params.begin(); iter != params.end(); iter++) {
      LogDebug("[connMySql] params:[%s:%s]", (iter->first).c_str(),(iter->second).c_str());
  }

  MySQL::ptr mysql(new MySQL(params));
  if (!mysql->connect())
  {
    LogError("connect fail");
    return;
  }
  _mySql = mysql;
}

void rrServer::start()
{
  LogDebug("rrServer::start() start");

  while (1)
  {
    //  Wait for next request from client
    std::string string = s_recv(*responder);

    LogDebug("Received request: :[%s]", string.c_str());
    LogDebug("Received request: method:[%s]", (JsonUtil::getStr(string, "method")).c_str());
    LogDebug("Received request: url:[%s]", (JsonUtil::getStr(string, "url")).c_str());

    std::vector<std::string> pairslist = StringUtil::mySplit(JsonUtil::getStr(string, "url"), "/");

    std::string sql1 = "select * from ";
    std::string retStr = "";
    if (pairslist.size() == 2)
    {
      LogDebug("pairslist: %s , %s", pairslist[0].c_str(), pairslist[1].c_str());

      ISQLData::ptr m_ptr = _mySql->query("show tables");

      std::vector<std::string> tableList;
      bool isExist = false;

      while (m_ptr->next())
      {
        for (int j = 0; j < m_ptr->getColumnCount(); ++j)
        {
          tableList.push_back(m_ptr->getString(j));
        }
      }

      std::string tablelistStr = "";
      for (int i = 0; i < tableList.size(); i++)
      {
        tablelistStr = tablelistStr + tableList[i] + " ";
      }

      LogDebug("tablelistStr:[%s]", tablelistStr.c_str());

      std::vector<std::string>::iterator s = find(tableList.begin(), tableList.end(), pairslist[1]);

      if (s != tableList.end()) //找到
      {
        std::cout << *s << std::endl;
        isExist = true;
      }
      else
      {
        retStr = "not find table";
      }

      if (pairslist[0] == "database" && isExist)
      {
        sql1 = sql1 + pairslist[1];
        ISQLData::ptr m_ptr1 = _mySql->query(sql1);

        for (int i = 0; i < m_ptr1->getColumnCount(); i++)
        {
          retStr = retStr + m_ptr1->getColumnName(i) + " ";
        }
        retStr = retStr + "\n";

        while (m_ptr1->next())
        {
          for (int j = 0; j < m_ptr1->getColumnCount(); ++j)
          {
            retStr = retStr + m_ptr1->getString(j) + " ";
          }
          retStr = retStr + "\n";
        }
      }
    }

    // Do some 'work'

    LogDebug("retStr:[%s]", retStr.c_str());
    //  Send reply back to client
    s_send(*responder, retStr);
  }

  LogDebug("rrServer::start() end");
}
