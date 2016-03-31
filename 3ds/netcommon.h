#ifndef _NET_COMMON_H_
#define _NET_COMMON_H_

#define NET_BUFFER_SIZE 32768

int net_error(const char *error);

int net_server_init(unsigned short port);

void net_server_exit(int socket);

#endif // _NET_COMMON_H_