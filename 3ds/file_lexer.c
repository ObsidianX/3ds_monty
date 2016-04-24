#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#include "py/mpconfig.h"
#include "py/lexer.h"

typedef struct _mp_lexer_file_buf_t {
    int fd;
    byte buf[1024];
    uint16_t len;
    uint16_t pos;
} mp_lexer_file_buf_t;

STATIC mp_uint_t file_buf_next_byte(mp_lexer_file_buf_t *fb) {
    if (fb->pos >= fb->len) {
        if (fb->len < sizeof(fb->buf)) {
            return MP_LEXER_EOF;
        } else {
            unsigned int n = read(fb->fd, fb->buf, sizeof(fb->buf));
            if (n == 0) {
                return MP_LEXER_EOF;
            }
            fb->len = n;
            fb->pos = 0;
        }
    }
    return fb->buf[fb->pos++];
}

STATIC void file_buf_close(mp_lexer_file_buf_t *fb) {
    close(fb->fd);
    m_del_obj(mp_lexer_file_buf_t, fb);
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    mp_lexer_file_buf_t *fb = m_new_obj_maybe(mp_lexer_file_buf_t);
    if (fb == NULL) {
        return NULL;
    }
    fb->fd = open(filename, O_RDONLY);
    if (fb->fd < 0) {
        m_del_obj(mp_lexer_file_buf_t, fb);
        return NULL;
    }
    unsigned int n = read(fb->fd, fb->buf, sizeof(fb->buf));
    fb->len = n;
    fb->pos = 0;
    return mp_lexer_new(qstr_from_str(filename), fb, (mp_lexer_stream_next_byte_t)file_buf_next_byte, (mp_lexer_stream_close_t)file_buf_close);
}

