/**
 * @file RequestData.h
 * @brief RequestData类
 * @author klc
 * @date 2021-04-07
 * @copyright Copyright (c) 2021年 klc
 */

#ifndef __STIBEL_REQUESTDATA_H__
#define __STIBEL_REQUESTDATA_H__

#include <string>
#include <cstring>
#include <unordered_map>
#include <memory>
#include <map>
#include "json/json.h"
#include "StiBel/Data/MySQL/MySQL.h"

using namespace StiBel::Data::MySQL;
const int MAX_BUFF = 4096;
const int AGAIN_MAX_TIMES = 200;
const int EPOLL_WAIT_TIME = 500;
const int DEFAULT_EXPIRED_TIME = 2000;             // ms
const int DEFAULT_KEEP_ALIVE_TIME = 5 * 60 * 1000; // ms
const std::string rootPath = "../myprj/webServer/webContent/";
const std::string cgiPath = "./CGISQL.cgi";

enum ProcessState
{
  STATE_PARSE_URI = 1,
  STATE_PARSE_HEADERS,
  STATE_RECV_BODY,
  STATE_ANALYSIS,
  STATE_FINISH
};

enum URIState
{
  PARSE_URI_AGAIN = 1,
  PARSE_URI_ERROR,
  PARSE_URI_SUCCESS,
};

enum HeaderState
{
  PARSE_HEADER_SUCCESS = 1,
  PARSE_HEADER_AGAIN,
  PARSE_HEADER_ERROR
};

enum ParseState
{
  H_START = 0,
  H_KEY,
  H_COLON,
  H_SPACES_AFTER_COLON,
  H_VALUE,
  H_CR,
  H_LF,
  H_END_CR,
  H_END_LF
};

enum AnalysisState
{
  ANALYSIS_SUCCESS = 1,
  ANALYSIS_ERROR
};

enum HttpMethod
{
  METHOD_GET = 1,
  METHOD_POST,
  METHOD_PUT,
  METHOD_DELETE,
  METHOD_HEAD
};

enum HttpVersion
{
  HTTP_10 = 1,
  HTTP_11
};

class MimeType
{
private:
  static void init();
  static std::unordered_map<std::string, std::string> mime;
  MimeType();
  MimeType(const MimeType &m);

public:
  static std::string getMime(const std::string &suffix);

private:
  static pthread_once_t once_control;
};

class TimerNode;

class RequestData : public std::enable_shared_from_this<RequestData>
{

public:
  RequestData();
  RequestData(int _epollfd, int _fd, std::string _path);
  ~RequestData();

  void linkTimer(std::shared_ptr<TimerNode> mtimer) { timer = mtimer; }
  void reset();
  void seperateTimer();

  //第一个const修饰的是返回值，表示返回的是常整型;第二个const该函数为只读函数，不允许修改其中的数据成员的值。
  const int getFd() const { return fd; }
  void setFd(int _fd) { fd = _fd; }
  const std::string &getPath() const { return path; }
  void setPath(const std::string &_path) { path = _path; }

  void handleRead();
  void handleWrite();
  void handleError(int fd, int err_num, std::string short_msg);
  void handleConn();

  void enableRead() { isAbleRead = true; }
  void enableWrite() { isAbleWrite = true; }
  bool canRead() { return isAbleRead; }
  bool canWrite() { return isAbleWrite; }

  void disableReadAndWrite()
  {
    isAbleRead = false;
    isAbleWrite = false;
  }

  void connMySql();

private:
  URIState parseURI();
  HeaderState parseHeaders();
  AnalysisState analysisRequest();

private:
  std::string path;
  int fd;
  int epollfd;

  std::string inBuffer;
  std::string outBuffer;
  __uint32_t events;
  bool error;

  HttpMethod method;
  HttpVersion httpVersion;
  std::string file_name;
  std::string realFile;
  int now_read_pos;
  ProcessState state;
  ParseState h_state;
  bool isfinish;
  bool keep_alive;
  std::unordered_map<std::string, std::string> headers;
  std::weak_ptr<TimerNode> timer;

  bool isAbleRead;
  bool isAbleWrite;
  static MySQL::ptr _mySql;
};

#endif //__STIBEL_REQUESTDATA_H__
