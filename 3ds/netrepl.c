#include <arpa/inet.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "basic_console.h"
#include "netcommon.h"
#include "netrepl.h"

static int server_socket = -1;
static int client_socket = -1;
static bool repl_thread_running = false;
static Thread repl_read_thread_handle;
static Thread repl_send_thread_handle;

static net_repl_packet_t repl_packet;
static bool repl_packet_ready = false;

static net_repl_packet_t repl_send_packet;
static bool repl_send_packet_ready = false;

void repl_read_thread(void *arg) {
    while (repl_thread_running) {
        if (!repl_packet_ready) {
            ssize_t count = recv(client_socket, &repl_packet, sizeof(net_repl_packet_t), 0);
            if (count == sizeof(net_repl_packet_t)) {
                repl_packet.line = malloc(repl_packet.line_len);
                recv(client_socket, &repl_packet.line, repl_packet.line_len, 0);

                repl_packet_ready = true;
            }
        }
    }
}

void repl_send_thread(void *arg) {
    while (repl_thread_running) {
        if (repl_send_packet_ready) {
            ssize_t sent = send(client_socket, &repl_send_packet, sizeof(net_repl_packet_t), 0);
            if (sent == sizeof(net_repl_packet_t)) {
                send(client_socket, &repl_send_packet.line, repl_send_packet.line_len, 0);

                repl_send_packet_ready = false;
            }
        }
    }
}

bool net_repl_connect(void) {
    server_socket = net_server_init(5555);

    init_console();
    consoleClear();

    struct in_addr host_addr = {(in_addr_t) gethostid()};
    printf("\x1b[14;10HWaiting for REPL connection on");
    printf("\x1b[15;15H%*s:5555", 15, inet_ntoa(host_addr));

    printf("\x1b[17;13HPress any key to cancel");

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

    bool result = true;
    bool waiting = true;
    int cl_sock = -1;
    while (waiting) {
        cl_sock = accept(server_socket, (struct sockaddr *) &client, &client_len);
        if (cl_sock >= 0) {
            break;
        }

        hidScanInput();
        if (hidKeysDown()) {
            waiting = false;
            result = false;
        }

        gspWaitForVBlank();
    }

    client_socket = cl_sock;

    int buffer_size = NET_BUFFER_SIZE;
    setsockopt(client_socket, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size));

    s32 main_prio;
    svcGetThreadPriority(&main_prio, CUR_THREAD_HANDLE);

    repl_read_thread_handle = threadCreate(repl_read_thread, NULL, 1024, main_prio - 1, -2, true);
    repl_send_thread_handle = threadCreate(repl_send_thread, NULL, 1024, main_prio - 1, -2, true);

    return result;
}

void net_repl_close(void) {
    if (client_socket >= 0) {
        close(client_socket);
    }
    net_server_exit(server_socket);
}

void net_repl_recv(net_repl_packet_t *packet) {
    if (!repl_packet_ready) {
        packet->type = REPL_WAIT;
        return;
    }

    *packet = repl_packet;
}

void net_repl_send(int type, const char *text, bool partial) {
    memset(&repl_send_packet, 0, sizeof(net_repl_packet_t));

    repl_send_packet.type = type;
    repl_send_packet.line = strdup(text);
    repl_send_packet.line_len = strlen(text);
    repl_send_packet.partial = partial;
}

char *strjoin(const char *s1, int sep_char, const char *s2) {
    int l1 = strlen(s1);
    int l2 = strlen(s2);
    char *s = malloc(l1 + l2 + 2);
    memcpy(s, s1, l1);
    if (sep_char != 0) {
        s[l1] = sep_char;
        l1 += 1;
    }
    memcpy(s + l1, s2, l2);
    s[l1 + l2] = 0;
    return s;
}