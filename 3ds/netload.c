#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <arpa/inet.h>

#include <3ds.h>
#include <malloc.h>

#define NET_BUFFER_SIZE 32768

#define SOC_BUFFER_ALIGNMENT 0x1000
#define SOC_BUFFER_SIZE 0x100000

static void *soc_buffer;

static void initConsole() {
    gfxInitDefault();
    consoleInit(GFX_TOP, NULL);
}

static int recvwait(int sockfd, void *buf, size_t len, int flags) {
    errno = 0;

    int ret = 0;
    size_t read = 0;
    while (((ret = (int) recv(sockfd, buf + read, len - read, flags)) >= 0 && (read += ret) < len) || errno == EAGAIN) {
        errno = 0;
    }

    return ret < 0 ? ret : (int) read;
}

static int sendwait(int sockfd, void *buf, size_t len, int flags) {
    errno = 0;

    int ret = 0;
    size_t written = 0;
    while (((ret = send(sockfd, buf + written, len - written, flags)) >= 0 && (written += ret) < len) || errno == EAGAIN) {
        errno = 0;
    }

    return ret < 0 ? ret : (int) read;
}

static int show_net_error(const char *error) {
    free(soc_buffer);
    socExit();

    initConsole();

    printf(error);
    printf("\nerrno: %d\n\n", errno);
    printf("Press Start to exit\n");
    printf("Press Select to retry\n");

    int ret = 0;

    while (aptMainLoop()) {
        hidScanInput();

        int down = hidKeysDown();
        if (down) {
            if(down & KEY_SELECT) {
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

int net_load() {
    soc_buffer = memalign(SOC_BUFFER_ALIGNMENT, SOC_BUFFER_SIZE);
    if (soc_buffer == NULL) {
        errno = ENOMEM;
        return 0;
    }
    socInit(soc_buffer, SOC_BUFFER_SIZE);

    int result = 1;

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_IP);
    if (sock < 0) {
        return show_net_error("Failed to init socket");
    }

    fcntl(sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(5000);
    server.sin_addr.s_addr = (in_addr_t) gethostid();

    if (bind(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
        close(sock);
        return show_net_error("Couldn't bind socket");
    }

    if (listen(sock, 5) < 0) {
        close(sock);
        return show_net_error("Couldn't listen on socket");
    }

    initConsole();
    consoleClear();

    printf("Waiting for script on port:\n");
    struct in_addr addr = {(in_addr_t) gethostid()};
    printf("%s:5000\n", inet_ntoa(addr));

    printf("\nPress any key to cancel\n");

    struct sockaddr_in client;
    socklen_t client_len = sizeof(client);

    bool waiting = true;
    int cl_sock = -1;
    while (waiting) {
        cl_sock = accept(sock, (struct sockaddr *) &client, &client_len);
        if (cl_sock >= 0) {
            break;
        }

        hidScanInput();
        if (hidKeysDown()) {
            waiting = false;
            result = 0;
        }

        gspWaitForVBlank();
    }

    if (cl_sock >= 0) {
        int buffer_size = NET_BUFFER_SIZE;
        setsockopt(cl_sock, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size));

        fcntl(cl_sock, F_SETFL, fcntl(sock, F_GETFL, 0) | O_NONBLOCK);

        printf("\nClient: %s\n", inet_ntoa(client.sin_addr));

        unsigned int total_bytes = 0;
        if (recvwait(cl_sock, &total_bytes, sizeof(total_bytes), 0) < 0 || total_bytes == 0) {
            close(cl_sock);
            close(sock);
            return show_net_error("Couldn't read file size");
        }

        total_bytes = ntohl(total_bytes);

        printf("\nReceiving file... (%u bytes)\n", total_bytes);
        printf("Press B to exit\n");
        gfxFlushBuffers();

        unsigned int received_bytes = 0;
        int ret = 0;
        unsigned char *buffer = malloc(512);
        FILE *file = fopen("/tmp.py", "w");

        while (received_bytes < total_bytes) {
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

    close(sock);

    free(soc_buffer);
    socExit();

    gfxExit();

    return result;
}