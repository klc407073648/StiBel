#include "RequestData.h"
#include "MyUtil.h"
#include "StiBel/Util.h"
#include "Epoll.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <queue>
#include <cstdlib>
#include <iostream>
#include <map>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include "StiBel/Data/MySQL/MySQL.h"

using namespace std;
using StiBel::ShellUtil;
using namespace StiBel::Data::MySQL;

pthread_once_t MimeType::once_control = PTHREAD_ONCE_INIT;
std::unordered_map<std::string, std::string> MimeType::mime;
MySQL::ptr RequestData::_mySql=NULL;

void RequestData::connMySql()
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

void MimeType::init()
{
    mime[".html"] = "text/html";
    mime[".avi"] = "video/x-msvideo";
    mime[".bmp"] = "image/bmp";
    mime[".c"] = "text/plain";
    mime[".doc"] = "application/msword";
    mime[".gif"] = "image/gif";
    mime[".gz"] = "application/x-gzip";
    mime[".htm"] = "text/html";
    mime[".ico"] = "image/x-icon";
    mime[".jpg"] = "image/jpeg";
    mime[".png"] = "image/png";
    mime[".txt"] = "text/plain";
    mime[".mp3"] = "audio/mp3";
    mime["default"] = "text/html";
}

std::string MimeType::getMime(const std::string &suffix)
{
    pthread_once(&once_control, MimeType::init);
    if (mime.find(suffix) == mime.end())
        return mime["default"];
    else
        return mime[suffix];
}

RequestData::RequestData() : now_read_pos(0),
                             state(STATE_PARSE_URI),
                             h_state(H_START),
                             keep_alive(false),
                             isAbleRead(true),
                             isAbleWrite(false),
                             events(0),
                             error(false)
{
    cout << "RequestData()" << endl;
    connMySql();
}

RequestData::RequestData(int _epollfd, int _fd, std::string _path) : now_read_pos(0),
                                                                     state(STATE_PARSE_URI),
                                                                     h_state(H_START),
                                                                     keep_alive(false),
                                                                     path(_path),
                                                                     fd(_fd),
                                                                     epollfd(_epollfd),
                                                                     isAbleRead(true),
                                                                     isAbleWrite(false),
                                                                     events(0),
                                                                     error(false)
{
    cout << "RequestData()" << endl;
    connMySql();
}

RequestData::~RequestData()
{
    cout << "~RequestData()" << endl;
    close(fd);
}

void RequestData::reset()
{
    inBuffer.clear();
    file_name.clear();
    path.clear();
    realFile.clear();
    now_read_pos = 0;
    state = STATE_PARSE_URI;
    h_state = H_START;
    headers.clear();
    //keep_alive = false;
    if (timer.lock())
    {
        shared_ptr<TimerNode> my_timer(timer.lock());
        my_timer->clearReq();
        timer.reset();
    }
}

void RequestData::seperateTimer()
{
    cout << "seperateTimer" << endl;
    if (timer.lock())
    {
        shared_ptr<TimerNode> my_timer(timer.lock());
        my_timer->clearReq();
        timer.reset();
    }
}

void RequestData::handleRead()
{
    cout << "handleRead begin" << endl;

    do
    {
        //从fd获取信息存储到inBuffer
        int read_num = readn(fd, inBuffer);
        printf("read_num: %d\n , inBuffer: \n [%s]", read_num, inBuffer.c_str());

        //判断读取的字符数量
        if (read_num < 0)
        {
            perror("1");
            error = true;
            handleError(fd, 400, "Bad Request");
            break;
        }
        else if (read_num == 0)
        {
            // 有请求出现但是读不到数据，可能是Request Aborted，或者来自网络的数据没有达到等原因
            // 最可能是对端已经关闭了，统一按照对端已经关闭处理
            error = true;
            break;
        }

        printf("begin to parseURI\n");
        if (state == STATE_PARSE_URI)
        {
            URIState flag = this->parseURI();
            if (flag == PARSE_URI_AGAIN)
                break;
            else if (flag == PARSE_URI_ERROR)
            {
                perror("2");
                error = true;
                handleError(fd, 400, "Bad Request");
                break;
            }
            else
                state = STATE_PARSE_HEADERS;
        }

        printf("begin to parseHeaders\n");
        if (state == STATE_PARSE_HEADERS)
        {
            HeaderState flag = this->parseHeaders();
            if (flag == PARSE_HEADER_AGAIN)
                break;
            else if (flag == PARSE_HEADER_ERROR)
            {
                perror("3");
                error = true;
                handleError(fd, 400, "Bad Request");
                break;
            }

            //仅POST方法需要传递body内容
            //if (method == METHOD_POST || method == METHOD_PUT || method == METHOD_DELETE)
            if (method == METHOD_POST)
            {
                state = STATE_RECV_BODY;
            }
            else
            {
                state = STATE_ANALYSIS;
            }
        }

        printf("begin to ensure if STATE_RECV_BODY\n");
        if (state == STATE_RECV_BODY)
        {
            int content_length = -1;
            if (headers.find("Content-Length") != headers.end())
            {
                content_length = stoi(headers["Content-Length"]);
            }
            else
            {
                error = true;
                handleError(fd, 400, "Bad Request: Lack of argument (Content-Length)");
                break;
            }

            if (inBuffer.size() < content_length)
                break;

            state = STATE_ANALYSIS;
        }

        printf("begin to analysisRequest\n");
        if (state == STATE_ANALYSIS)
        {
            AnalysisState flag = this->analysisRequest();
            if (flag == ANALYSIS_SUCCESS)
            {
                state = STATE_FINISH;
                break;
            }
            else
            {
                error = true;
                break;
            }
        }
    } while (false);

    if (!error)
    {
        if (outBuffer.size() > 0)
            events |= EPOLLOUT;

        if (state == STATE_FINISH)
        {
            cout << "keep_alive=" << keep_alive << endl;
            if (keep_alive)
            {
                this->reset();
                events |= EPOLLIN;
            }
            else
                return;
        }
        else
            events |= EPOLLIN;
    }
}

void RequestData::handleWrite()
{
    if (!error)
    {
        if (writen(fd, outBuffer) < 0)
        {
            perror("writen");
            events = 0;
            error = true;
        }
        else if (outBuffer.size() > 0)
            events |= EPOLLOUT;
    }
}

void RequestData::handleConn()
{
    if (!error)
    {
        if (events != 0)
        {
            // 一定要先加时间信息，否则可能会出现刚加进去，下个in触发来了，然后分离失败后，又加入队列，最后超时被删，然后正在线程中进行的任务出错，double free错误。
            // 新增时间信息
            int timeout = DEFAULT_EXPIRED_TIME;
            if (keep_alive)
                timeout = DEFAULT_KEEP_ALIVE_TIME;
            isAbleRead = false;
            isAbleWrite = false;
            Epoll::add_timer(shared_from_this(), timeout);

            if ((events & EPOLLIN) && (events & EPOLLOUT))
            {
                events = __uint32_t(0);
                events |= EPOLLOUT;
            }
            events |= (EPOLLET | EPOLLONESHOT);
            __uint32_t _events = events;
            events = 0;
            if (Epoll::epoll_mod(fd, shared_from_this(), _events) < 0)
            {
                printf("Epoll::epoll_mod error\n");
            }
        }
        else if (keep_alive)
        {
            events |= (EPOLLIN | EPOLLET | EPOLLONESHOT);
            int timeout = DEFAULT_KEEP_ALIVE_TIME;
            isAbleRead = false;
            isAbleWrite = false;
            Epoll::add_timer(shared_from_this(), timeout);
            __uint32_t _events = events;
            events = 0;
            if (Epoll::epoll_mod(fd, shared_from_this(), _events) < 0)
            {
                printf("Epoll::epoll_mod error\n");
            }
        }
    }
}

URIState RequestData::parseURI()
{
    string &str = inBuffer;
    // 读到完整的请求行再开始解析请求
    int pos = str.find('\r', now_read_pos);

    if (pos < 0)
    {
        return PARSE_URI_AGAIN;
    }

    // 去掉请求行所占的空间，节省空间
    string request_line = str.substr(0, pos);
    if (str.size() > pos + 1)
        str = str.substr(pos + 1);
    else
        str.clear();

    // Method
    vector<std::string> vec = {"null", "GET", "POST", "PUT", "DELETE", "HEAD"};
    bool isFind = false;
    for (int i = 1; i < vec.size(); i++)
    {
        int posTemp = request_line.find(vec[i]);

        if (posTemp >= 0)
        {
            pos = posTemp;

            switch (i)
            {
            case 1:
                method = METHOD_GET;
                break;
            case 2:
                method = METHOD_POST;
                break;
            case 3:
                method = METHOD_PUT;
                break;
            case 4:
                method = METHOD_DELETE;
                break;
            case 5:
                method = METHOD_HEAD;
                break;
            }
            isFind = true;
            cout << "method: " << vec[i] << " method value: " << method << endl;
            break;
        }
    }

    if (!isFind)
    {
        return PARSE_URI_ERROR;
    }

    //POST /login HTTP/1.1
    pos = request_line.find("/", pos);
    if (pos < 0)
        return PARSE_URI_ERROR;
    else
    {
        int _pos = request_line.find(' ', pos);
        if (_pos < 0)
            return PARSE_URI_ERROR;
        else
        {
            if (_pos - pos > 1)
            {
                file_name = request_line.substr(pos + 1, _pos - pos - 1);
                int __pos = file_name.find('?');
                if (__pos >= 0)
                {
                    file_name = file_name.substr(0, __pos);
                }
            }

            else
                file_name = "judge.html";
        }
        pos = _pos;
    }

    cout << "file_name: " << file_name << endl;

    // HTTP 版本号
    pos = request_line.find("/", pos);
    if (pos < 0)
        return PARSE_URI_ERROR;
    else
    {
        if (request_line.size() - pos <= 3)
            return PARSE_URI_ERROR;
        else
        {
            string ver = request_line.substr(pos + 1, 3);
            if (ver == "1.0")
                httpVersion = HTTP_10;
            else if (ver == "1.1")
                httpVersion = HTTP_11;
            else
                return PARSE_URI_ERROR;
        }
    }

    return PARSE_URI_SUCCESS;
}

//解析请求报头，按照:保存为map结构
HeaderState RequestData::parseHeaders()
{
    string &str = inBuffer;
    int key_start = -1, key_end = -1, value_start = -1, value_end = -1;
    int now_read_line_begin = 0;
    bool notFinish = true;
    size_t i = 0;
    for (; i < str.size() && notFinish; ++i)
    {
        switch (h_state)
        {
        case H_START:
        {
            if (str[i] == '\n' || str[i] == '\r')
                break;
            h_state = H_KEY;
            key_start = i;
            now_read_line_begin = i;
            break;
        }
        case H_KEY:
        {
            if (str[i] == ':')
            {
                key_end = i;
                if (key_end - key_start <= 0)
                    return PARSE_HEADER_ERROR;
                h_state = H_COLON;
            }
            else if (str[i] == '\n' || str[i] == '\r')
                return PARSE_HEADER_ERROR;
            break;
        }
        case H_COLON:
        {
            if (str[i] == ' ')
            {
                h_state = H_SPACES_AFTER_COLON;
            }
            else
                return PARSE_HEADER_ERROR;
            break;
        }
        case H_SPACES_AFTER_COLON:
        {
            h_state = H_VALUE;
            value_start = i;
            break;
        }
        case H_VALUE:
        {
            if (str[i] == '\r')
            {
                h_state = H_CR;
                value_end = i;
                if (value_end - value_start <= 0)
                    return PARSE_HEADER_ERROR;
            }
            else if (i - value_start > 255)
                return PARSE_HEADER_ERROR;
            break;
        }
        case H_CR:
        {
            if (str[i] == '\n')
            {
                h_state = H_LF;
                string key(str.begin() + key_start, str.begin() + key_end);
                string value(str.begin() + value_start, str.begin() + value_end);
                headers[key] = value;
                now_read_line_begin = i;
            }
            else
                return PARSE_HEADER_ERROR;
            break;
        }
        case H_LF:
        {
            if (str[i] == '\r')
            {
                h_state = H_END_CR;
            }
            else
            {
                key_start = i;
                h_state = H_KEY;
            }
            break;
        }
        case H_END_CR:
        {
            if (str[i] == '\n')
            {
                h_state = H_END_LF;
            }
            else
                return PARSE_HEADER_ERROR;
            break;
        }
        case H_END_LF:
        {
            notFinish = false;
            key_start = i;
            now_read_line_begin = i;
            break;
        }
        }
    }
    if (h_state == H_END_LF)
    {
        str = str.substr(now_read_line_begin);
        return PARSE_HEADER_SUCCESS;
    }
    str = str.substr(now_read_line_begin);
    return PARSE_HEADER_AGAIN;
}

AnalysisState RequestData::analysisRequest()
{
    if (method == METHOD_POST)
    {
        int length = stoi(headers["Content-Length"]);
        vector<char> data(inBuffer.begin(), inBuffer.begin() + length);

        string postInfo = inBuffer.substr(0, length);
        cout << "METHOD_POST inBuffer " << postInfo << endl;

        outBuffer = outBuffer + "\{ \"test\": \"post\" }";
        return ANALYSIS_SUCCESS;
    }
    else if (method == METHOD_GET)
    {
		std::string sql1 = "select * from student";
		ISQLData::ptr m_ptr1=_mySql->query(sql1);
		m_ptr1->showAllRes(); 
		cout<<"——————————————————————————————"<<endl;
		
        //待优化成json信息返回，各类Content-type返回，或者转给其他进程处理zeromq
        cout << "headers BEGIN" << endl;
        for (std::unordered_map<std::string, std::string>::iterator it = headers.begin(); it!=headers.end(); it++)
        {
            cout << it->first << ":" << it->second << endl;
        }
        cout << "headers END" << endl;

        string res = "\{ \"method\": \"get\",";

        res = res + "\"file_name\":" + file_name + "}";

        string header;
        header += "HTTP/1.1 200 OK\r\n";
        if (headers.find("Connection") != headers.end() && (headers["Connection"] == "Keep-Alive" || headers["Connection"] == "keep-alive"))
        {
            keep_alive = true;
            header += string("Connection: Keep-Alive\r\n") + "Keep-Alive: timeout=" + to_string(5 * 60 * 1000) + "\r\n";
        }

        header += "Access-Control-Allow-Origin: *\r\n";
        header += "Content-type: application/json\r\n";
        header += "Content-Length: " + to_string(res.size()) + "\r\n";

        header += "\r\n";
        outBuffer += header;
        outBuffer += res;

        return ANALYSIS_SUCCESS;
    }
    else
    {
        handleError(fd, 400, "method NOT GET OR POST");
        return ANALYSIS_ERROR;
    }
}

void RequestData::handleError(int fd, int err_num, string short_msg)
{
    short_msg = " " + short_msg;
    char send_buff[MAX_BUFF];
    string body_buff, header_buff;
    body_buff += "<html><title>哎~出错了</title>";
    body_buff += "<body bgcolor=\"ffffff\">";
    body_buff += to_string(err_num) + short_msg;
    body_buff += "<hr><em> STLBE's Web Server</em>\n</body></html>";

    header_buff += "HTTP/1.1 " + to_string(err_num) + short_msg + "\r\n";
    header_buff += "Content-type: application/json\r\n";
    header_buff += "Connection: close\r\n";
    header_buff += "Content-Length: " + to_string(body_buff.size()) + "\r\n";
    header_buff += "\r\n";
    // 错误处理不考虑writen不完的情况
    sprintf(send_buff, "%s", header_buff.c_str());
    writen(fd, send_buff, strlen(send_buff));
    sprintf(send_buff, "%s", body_buff.c_str());
    writen(fd, send_buff, strlen(send_buff));
}