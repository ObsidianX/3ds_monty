#include <sys/iosupport.h>

static int zip_open(struct _reent *r, void *fileStruct, const char *path, int flags, int mode);
static int zip_close(struct _reent *r, int fd);
static size_t zip_read(struct _reent *r, int fd, char *ptr, size_t len);
static off_t zip_seek(struct _reent *r, int fd, off_t pos, int dir);
static int zip_fstat(struct _reent *r, int fd, struct stat *st);
static int zip_stat(struct _reent *r, const char *file, struct stat *st);
static int zip_chdir(struct _reent *r, const char *name);
static DIR_ITER *zip_diropen(struct _reent *r, DIR_ITER *dirState, const char *path);
static int zip_dirreset(struct _reent *r, DIR_ITER *dirState);
static int zip_dirnext(struct _reent *r, DIR_ITER *dirState, char *filename, struct stat *filestat);
static int zip_dirclose(struct _reent *r, DIR_ITER *dirState);
static int zip_statvfs(struct _reent *r, const char *path, struct statvfs *buf);


static devoptab_t zip_devoptab = {
        .name = "zip",
        .structSize = sizeof(zip_file_t),
        .open_r = zip_open,
        .close_r = zip_close,
        .read_r = zip_read,
        .seek_r = zip_seek,
        .fstat_r = zip_fstat,
        .stat_r = zip_stat,
        .chdir_r = zip_chdir,
        .dirStateSize = sizeof(zip_dir_t),
        .diropen_r = zip_diropen,
        .dirreset_r = zip_dirreset,
        .dirnext_r = zip_dirnext,
        .dirclose_r = zip_dirclose,
        .statvfs_r = zip_statvfs,
        .deviceData = NULL,
};

int zipInit(const char *filename) {
    int dev = AddDevice(&zip_devoptab);
    if(dev != -1) {
        setDefaultDevice(dev);
    }
}

void zipExit() {
    RemoveDevice("zip:");
}

static int zip_open(struct _reent *r, void *fileStruct, const char *path, int flags, int mode) {

}

static int zip_close(struct _reent *r, int fd) {

}

static size_t zip_read(struct _reent *r, int fd, char *ptr, size_t len) {

}

static off_t zip_seek(struct _reent *r, int fd, off_t pos, int dir) {

}

static int zip_fstat(struct _reent *r, int fd, struct stat *st) {

}

static int zip_stat(struct _reent *r, const char *file, struct stat *st) {

}

static int zip_chdir(struct _reent *r, const char *name) {

}

static DIR_ITER *zip_diropen(struct _reent *r, DIR_ITER *dirState, const char *path) {

}

static int zip_dirreset(struct _reent *r, DIR_ITER *dirState) {

}

static int zip_dirnext(struct _reent *r, DIR_ITER *dirState, char *filename, struct stat *filestat) {

}

static int zip_dirclose(struct _reent *r, DIR_ITER *dirState) {

}

static int zip_statvfs(struct _reent *r, const char *path, struct statvfs *buf) {

}