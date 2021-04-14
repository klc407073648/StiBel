/**
 * @file MyUtil.h
 * @brief MyUtil类
 * @author klc
 * @date 2021-04-07
 * @copyright Copyright (c) 2021年 klc
 */

#ifndef __STIBEL_MYUTIL_H__
#define __STIBEL_MYUTIL_H__

#include <cstdlib>
#include <string>

ssize_t readn(int fd, void *buff, size_t n);
ssize_t readn(int fd, std::string &inBuffer);
ssize_t writen(int fd, void *buff, size_t n);
ssize_t writen(int fd, std::string &sbuff);
void handle_for_sigpipe();
int setSocketNonBlocking(int fd);
int socket_bind_listen(int port);

#endif //__STIBEL_MYUTIL_H__

