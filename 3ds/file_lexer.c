#include <stdio.h>

#include "py/mpconfig.h"
#include "py/lexer.h"

typedef struct _mp_lexer_file_buf_t {
    FILE *fd;
    byte buf[1];
    uint16_t len;
    uint16_t pos;
} mp_lexer_file_buf_t;

STATIC mp_uint_t file_buf_next_byte(mp_lexer_file_buf_t *fb) {
    if (fb->pos >= fb->len) {
        if (fb->len < sizeof(fb->buf)) {
            return MP_LEXER_EOF;
        } else {
            unsigned int n = fread(fb->buf, 1, sizeof(fb->buf), fb->fd);
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
    fclose(fb->fd);
    m_del_obj(mp_lexer_file_buf_t, fb);
}

mp_lexer_t *mp_lexer_new_from_file(const char *filename) {
    mp_lexer_file_buf_t *fb = m_new_obj_maybe(mp_lexer_file_buf_t);
    if (fb == NULL) {
        return NULL;
    }
    fb->fd = fopen(filename, "r");
    if (!fb->fd) {
        m_del_obj(mp_lexer_file_buf_t, fb);
        return NULL;
    }
    unsigned int n = fread(fb->buf, 1, sizeof(fb->buf), fb->fd);
    fb->len = n;
    fb->pos = 0;
    return mp_lexer_new(qstr_from_str(filename), fb, (mp_lexer_stream_next_byte_t)file_buf_next_byte, (mp_lexer_stream_close_t)file_buf_close);
}

