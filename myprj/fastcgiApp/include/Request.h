/**
 * @file Request.h
 * @brief 事件请求和事件响应的封装类
 * @author klc
 * @date 2020-08-03
 * @copyright Copyright (c) 2020年 klc
 */

#ifndef __STIBEL_REQUEST_H__
#define __STIBEL_REQUEST_H__


#include <map>
#include <vector>
#include <iostream>
#include <functional>
#include <fcgi_stdio.h>
#include "fcgi_config.h"
#include <string>
#include "Common.h"

using std::string;

class  Request
{
public:
    Request();
    ~Request();

    void setMethod(const string& str);
    void setUrl(const string& str);
    void setParams(const string& str);
    void setToken(const string& str);

    string getMethod() const;
    string getUrl() const;
	string getParams() const;
    string getToken() const;
    const std::map<string, string>& getParamsMap() const;

private:
    void parseParam(std::string paramsStr, std::map<std::string, std::string> &reqParams);
private:
    string m_method;
    string m_url;
    string m_params;
    string m_token;
    int _expireTime;
    std::map<string, string> m_paramsMap;

};


class  Response
{
public:
    Response();
    ~Response();

    void setContentType(const string& str);
    void setSetToken(const string& str);
    void setResData(const string& str);

    string Out();

private:
    string m_contentType;
    string m_setToken;
    string m_resData;

};

#endif //__STIBEL_REQUEST_H__
