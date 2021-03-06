#include "CLogin.h"
#include "Request.h"
#include "Poco/RegularExpression.h"
#include "StiBel/Util.h"
#include "StiBel/TEncode.h"
#include "StiBel/Data/MySQL/MySQL.h"
#include "StiBel/Data/Redis/CMyRedis.h"

using Poco::RegularExpression;

using StiBel::TEncode;
using StiBel::FileUtil;
using StiBel::Data::Redis::CMyRedis;

using namespace StiBel::Log;
using namespace StiBel::Data::MySQL;

Login::LoginInfoMap Login::_loginInfoMap;

CREATE_STATIC_LOGGERMANAGER(Login);

Login::Login()
{
	LogInfo("Login::Login()");
}

Login::~Login()
{
	LogInfo("Login::~Login()");
}

Response Login::login(Request req)
{
	 LogDebug("Login::login() method : %s,  url: %s,  params: %s, token: %s",
        req.getMethod().c_str(),
        req.getUrl().c_str(),
        req.getParams().c_str(),
        req.getToken().c_str());

	Response res;
	string data;
	string method = req.getMethod();

	//存放user,passwd,token
	std::map<string, string> paramsMap=req.getParamsMap();

	if ("GET" == method)
	{
		//"127.0.0.1",int nPort=6379,const string& passwd=""
		//GET请求时候,带user和token进行校验 /login?user=KKK&token=klc123
		//设置了登陆时间失效机制成功,每次GET请求，刷新超期时间
		if (req.getToken() == (CMyRedis::getInstance("127.0.0.1",6379,"klczxas741789"))->Get(paramsMap["user"]))
		{
			(CMyRedis::getInstance())->Setex(paramsMap["user"],expire_time,req.getToken());
			data = FileUtil::loadFile(HTML_PATH + "index.html");
		}
		else
		{
			//校验失败
			data = "token is not match";
		}
		
		res.setResData(data);
	}
	else if ("POST" == method)
	{
		
		MySQL::ptr mysql= MySQL::getInstance();
		if (!mysql->connect())
		{
			LogError("mysql connect fail");
			res.setResData("mysql connect fail");
			return res;
		}
		
		string m_user = paramsMap["user"];
		string m_passwd = paramsMap["passwd"];
	
		m_passwd = TEncode::base64Encode(m_passwd);//加密后与数据库比对
		
		string sql_insert("");
		sql_insert += "select * from user where user='";
		sql_insert += m_user;
		sql_insert += "' AND passwd='";
		sql_insert += m_passwd;
		sql_insert += "';";

		mysql->query(sql_insert);
		
        LogDebug("sql_insert=%s",sql_insert.c_str());
		LogError("mysql->getAffectedRows(): %d ,mysql->getErrStr(): %s",mysql->getAffectedRows(),mysql->getErrStr().c_str());
		
		//查询数据库是否存在对应记录，如果存在，校验用户成功
		if (mysql->getAffectedRows() != 0)
		{
			_loginInfoMap.insert(pair<string, string>(m_user, req.getToken()));
			for(LoginInfoMap::iterator iter=_loginInfoMap.begin();iter!=_loginInfoMap.end();iter++)
            {
                LogInfo("_loginInfoMap:user = %s,token = %s",(iter->first).c_str(),(iter->second).c_str());
            }
			data = FileUtil::loadFile(HTML_PATH + "index.html");
			res.setSetToken(req.getToken());
		}
		//登陆失败
		else
		{
			data = FileUtil::loadFile(HTML_PATH + "loginError.html");			
		}

		res.setResData(data);
	}
	LogDebug("login end");
	return res;
}

Response Login::logout(Request req)
{
	LogDebug("logout begin");
    Response res;
	string data;
	string method = req.getMethod();
    LogDebug("Method is %s",method.c_str());
	if ("GET" == method)
	{
		 data = FileUtil::loadFile(HTML_PATH + "login.html");			
		 res.setResData(data);
	}
	else if ("POST" == method)
	{			
	    data = FileUtil::loadFile(HTML_PATH + "judge.html");			
		res.setResData(data);
	}
	LogDebug("logout end");
	return res;
}

