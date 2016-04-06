#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <3ds.h>

#include "basic_console.h"
#include "netload.h"
#include "netcommon.h"

static int recvwait(int sockfd, void *buf, size_t len, int flags) {
    errno = 0;

    int ret = 0;
    size_t read = 0;
    while (((ret = (int) recv(sockfd, buf + read, len - read, flags)) >= 0 && (read += ret) < len) || errno == EAGAIN) {
        errno = 0;
    }

    return ret < 0 ? ret : (int) read;
}

static ssize_t sendwait(int sockfd, void *buf, size_t len, int flags) {
    errno = 0;

    ssize_t ret = 0;
    size_t written = 0;
    while (((ret = send(sockfd, buf + written, len - written, flags)) >= 0 && (written += ret) < len) || errno == EAGAIN) {
        errno = 0;
    }

    return ret < 0 ? ret : (int) read;
}

int net_load() {
    int sock = net_server_init(5000);

    int result = 0;

    init_console();
    consoleClear();

    struct in_addr addr = {(in_addr_t) gethostid()};
    printf("Waiting for script on port:\n");
    printf("%s:5000\n", inet_ntoa(addr));

    printf("\nPress any key to cancel\n");

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

    bool waiting = true;
    int cl_sock = -1;
    while (aptMainLoop() && waiting) {
        cl_sock = accept(sock, (struct sockaddr *) &client, &client_len);
        if (cl_sock >= 0) {
            result = 1;
            break;
        }

        hidScanInput();
        if (hidKeysDown()) {
            waiting = false;
        }

        gspWaitForVBlank();
    }

    if (cl_sock >= 0) {
        int buffer_size = NET_BUFFER_SIZE;
        setsockopt(cl_sock, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size));

        fcntl(cl_sock, F_SETFL, fcntl(cl_sock, F_GETFL, 0) | O_NONBLOCK);

        printf("\nClient: %s\n", inet_ntoa(client.sin_addr));

        unsigned int total_bytes = 0;
        if (recvwait(cl_sock, &total_bytes, sizeof(total_bytes), 0) < 0 || total_bytes == 0) {
            close(cl_sock);
            net_server_exit(sock);
            return net_error("Couldn't read file size");
        }

        total_bytes = ntohl(total_bytes);

        printf("\nReceiving file... (%u bytes)\n", total_bytes);
        printf("Press B to exit\n");
        gfxFlushBuffers();

        unsigned int received_bytes = 0;
        int ret = 0;
        unsigned char *buffer = malloc(512);
        FILE *file = fopen("/monty_netload.py", "w");

        while (aptMainLoop() && received_bytes < total_bytes) {
            ret = (int) recv(cl_sock, buffer, 512, 0);
            if (ret >= 0) {
                received_bytes += ret;
                printf("\x1b[20;0H%50c", ' ');
                printf("\x1b[20;0H%u / %u", received_bytes, total_bytes);
                gfxFlushBuffers();

                fwrite(buffer, (size_t) ret, 1, file);
            } else if (ret == EOF) {
                result = 0;
                break;
            }

            hidScanInput();
            if (hidKeysDown() & KEY_B) {
                result = 0;
                break;
            }

            gspWaitForVBlank();
        }

        u8 ack = 0;
        sendwait(cl_sock, &ack, sizeof(ack), 0);

        fflush(file);
        fclose(file);

        free(buffer);

        close(cl_sock);
    }

    net_server_exit(sock);

    gfxExit();

    return result;
}