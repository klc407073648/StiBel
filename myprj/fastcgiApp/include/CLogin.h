/**
 * @file CLogin.h
 * @brief 登陆类
 * @author klc
 * @date 2020-08-09
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_CLOGIN_H__
#define __STIBEL_CLOGIN_H__

#include <string>
#include "StiBel/Log/LoggerManager.h"

class Request;
class Response;

class Login
{
public:
        typedef std::map<std::string , std::string> LoginInfoMap;
        Login();
        ~Login();
        Response login(Request req);
        Response logout(Request req);

private:
        DECLARE_STATIC_LOGGERMANAGER();
        static LoginInfoMap _loginInfoMap;
};

#endif //__STIBEL_CLOGIN_H__
