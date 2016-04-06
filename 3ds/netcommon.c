#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <malloc.h>
#include <stdio.h>
#include <unistd.h>

#include <3ds.h>

#include "basic_console.h"

#define SOC_BUFFER_ALIGNMENT 0x1000
#define SOC_BUFFER_SIZE 0x100000

static void *soc_buffer;

int net_error(const char *error) {
    free(soc_buffer);
    socExit();

    init_console();

    printf(error);
    printf("\nerrno: %d\n\n", errno);
    printf("Press Start to exit\n");
    printf("Press Select to retry\n");

    int ret = 0;

    while (aptMainLoop()) {
        hidScanInput();

        int down = hidKeysDown();
        if (down) {
            if (down & KEY_SELECT) {
                ret = 2;
            }
            break;
        }

        gfxFlushBuffers();
        gfxSwapBuffers();

        gspWaitForVBlank();
    }

    gfxExit();

    return ret;
}

int net_server_init(short port) {
    soc_buffer = memalign(SOC_BUFFER_ALIGNMENT, SOC_BUFFER_SIZE);
    if (soc_buffer == NULL) {
        errno = ENOMEM;
        return 0;
    }
    socInit(soc_buffer, SOC_BUFFER_SIZE);

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
        return net_error("Failed to init socket");
    }

    fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = (in_addr_t) gethostid();

    if (bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        close(sock);
        net_error("Couldn't bind socket");
        return -1;
    }

    if (listen(sock, 5) < 0) {
        close(sock);
        net_error("Couldn't listen on socket");
        return -1;
    }

    return sock;
}

void net_server_exit(int fd) {
    if(fd >= 0) {
        close(fd);
    }

    free(soc_buffer);

    socExit();
}