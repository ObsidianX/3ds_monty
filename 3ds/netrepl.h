#ifndef _NET_REPL_H_
#define _NET_REPL_H_

#include <unistd.h>

#include <3ds.h>

#define REPL_OK       0
#define REPL_CONTINUE 1
#define REPL_ERROR    2
#define REPL_QUIT     3
#define REPL_WAIT     4

typedef struct PACKED {
    int type;
    bool partial;

    size_t line_len;
    char *line;
} net_repl_packet_t;

bool net_repl_connect(void);

void net_repl_recv(net_repl_packet_t *packet);

void net_repl_send(int type, const char *data, bool partial);

char *strjoin(const char *s1, int sep_char, const char *s2);

#endif // _NET_REPL_H_