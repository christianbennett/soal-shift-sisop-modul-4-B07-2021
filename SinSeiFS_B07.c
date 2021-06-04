#define FUSE_USE_VERSION 28

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

static const char *dirpath = "/home/bennett/Downloads";
static const char *logpath = "/home/bennett/SinSeiFS.log";

void logFile(char *level, char *command, const char *desc[], int descLen)
{
    FILE *f = fopen(logpath, "a");
    time_t t;
    struct tm *tmp;
    char timeString[100];

    time(&t);
    tmp = localtime(&t);
    strftime(timeString, sizeof(timeString), "%d%m%y-%H:%M:%S", tmp);

    fprintf(f, "%s::%s::%s", level, timeString, command);
    for (int i = 0; i < descLen; i++)
    {
        fprintf(f, "::%s", desc[i]);
    }
    fprintf(f, "\n");

    fclose(f);
}

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
    char fpath[1000];

    sprintf(fpath, "%s%s", dirpath, path);

    res = lstat(fpath, stbuf);

    if (res == -1)
        return -errno;

    const char *desc[] = {path};
    logFile("INFO", "GETATTR", desc, 1);

    return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];

    if (strcmp(path, "/") == 0)
    {
        path = dirpath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    int res = 0;

    DIR *dp;
    struct dirent *de;
    (void)offset;
    (void)fi;

    dp = opendir(fpath);

    if (dp == NULL)
        return -errno;

    while ((de = readdir(dp)) != NULL)
    {
        struct stat st;

        memset(&st, 0, sizeof(st));

        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        res = (filler(buf, de->d_name, &st, 0));

        if (res != 0)
            break;
    }

    closedir(dp);

    const char *desc[] = {path};
    logFile("INFO", "READDIR", desc, 1);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    if (strcmp(path, "/") == 0)
    {
        path = dirpath;

        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirpath, path);

    int res = 0;
    int fd = 0;

    (void)fi;

    fd = open(fpath, O_RDONLY);

    if (fd == -1)
        return -errno;

    res = pread(fd, buf, size, offset);

    if (res == -1)
        res = -errno;

    close(fd);

    const char *desc[] = {path};
    logFile("INFO", "READ", desc, 1);

    return res;
}

static int xmp_access(const char *path, int mask)
{
    int res;

    res = access(path, mask);
    if (res == -1)
        return -errno;

    const char *desc[] = {path};
    logFile("INFO", "ACCESS", desc, 1);

    return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
    int res;

    res = readlink(path, buf, size - 1);
    if (res == -1)
        return -errno;

    const char *desc[] = {path};
    logFile("INFO", "READLINK", desc, 1);

    buf[res] = '\0';
    return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
    int res;

    /* On Linux this could just be 'mknod(path, mode, rdev)' but this
	   is more portable */
    if (S_ISREG(mode))
    {
        res = open(path, O_CREAT | O_EXCL | O_WRONLY, mode);
        if (res >= 0)
            res = close(res);
    }
    else if (S_ISFIFO(mode))
        res = mkfifo(path, mode);
    else
        res = mknod(path, mode, rdev);
    if (res == -1)
        return -errno;

    const char *desc[] = {path};
    logFile("INFO", "MKNOD", desc, 1);

    return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    int res;

    res = mkdir(path, mode);
    if (res == -1)
        return -errno;
    const char *desc[] = {path};
    logFile("INFO", "MKDIR", desc, 1);

    return 0;
}

static int xmp_unlink(const char *path)
{
    int res;

    res = unlink(path);
    if (res == -1)
        return -errno;
    const char *desc[] = {path};
    logFile("WARNING", "UNLINK", desc, 1);

    return 0;
}

static int xmp_rmdir(const char *path)
{
    int res;

    res = rmdir(path);
    if (res == -1)
        return -errno;

    const char *desc[] = {path};
    logFile("WARNING", "RMDIR", desc, 1);

    return 0;
}

static int xmp_symlink(const char *from, const char *to)
{
    int res;

    res = symlink(from, to);
    if (res == -1)
        return -errno;

    const char *desc[] = {from, to};
    logFile("INFO", "SYMLINK", desc, 2);

    return 0;
}

static int xmp_rename(const char *from, const char *to)
{
    int res;

    res = rename(from, to);
    if (res == -1)
        return -errno;

    const char *desc[] = {from, to};
    logFile("INFO", "RENAME", desc, 2);

    return 0;
}

static int xmp_link(const char *from, const char *to)
{
    int res;

    res = link(from, to);
    if (res == -1)
        return -errno;

    const char *desc[] = {from, to};
    logFile("INFO", "LINK", desc, 2);

    return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
    int res;

    res = chmod(path, mode);
    if (res == -1)
        return -errno;

    const char *desc[] = {path, modeBuff};
    logFile("INFO", "CHMOD", desc, 2;

    return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
    char uidString[100];
    char gidString[100];
    sprintf(uidString, "%d", uid);
    sprintf(gidString, "%d", gid);

    int res;

    res = lchown(path, uid, gid);
    if (res == -1)
        return -errno;

    const char *desc[] = {path, uidString, gidString};
    logFile("INFO", "CHOWN", desc, 3);

    return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
    int res;

    res = truncate(path, size);
    if (res == -1)
        return -errno;

    const char *desc[] = {path};
    logFile("INFO", "TRUNCATE", desc, 1);

    return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
    int res;
    struct timeval tv[2];

    tv[0].tv_sec = ts[0].tv_sec;
    tv[0].tv_usec = ts[0].tv_nsec / 1000;
    tv[1].tv_sec = ts[1].tv_sec;
    tv[1].tv_usec = ts[1].tv_nsec / 1000;

    res = utimes(path, tv);
    if (res == -1)
        return -errno;

    const char *desc[] = {path};
    logFile("INFO", "UTIMENSAT", desc, 1);

    return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
    int res;

    res = open(path, fi->flags);
    if (res == -1)
        return -errno;

    const char *desc[] = {path};
    logFile("INFO", "OPEN", desc, 1);

    close(res);
    return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
                     off_t offset, struct fuse_file_info *fi)
{
    int fd;
    int res;

    (void)fi;
    fd = open(path, O_WRONLY);
    if (fd == -1)
        return -errno;

    res = pwrite(fd, buf, size, offset);
    if (res == -1)
        res = -errno;

    close(fd);

    const char *desc[] = {path};
    logFile("INFO", "WRITE", desc, 1);

    return res;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
    int res;

    res = statvfs(path, stbuf);
    if (res == -1)
        return -errno;

    const char *desc[] = {path};
    logFile("INFO", "STATFS", desc, 1);

    return 0;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{

    (void)fi;

    int res;
    res = creat(path, mode);
    if (res == -1)
        return -errno;

    const char *desc[] = {path};
    logFile("INFO", "CREAT", desc, 1;

    close(res);

    return 0;
}

static int xmp_release(const char *path, struct fuse_file_info *fi)
{
    /* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

    (void)path;
    (void)fi;

    const char *desc[] = {path};
    logFile("INFO", "RELEASE", desc, 1);

    return 0;
}

static int xmp_fsync(const char *path, int isdatasync,
                     struct fuse_file_info *fi)
{
    /* Just a stub.	 This method is optional and can safely be left
	   unimplemented */

    (void)path;
    (void)isdatasync;
    (void)fi;

    onst char *desc[] = {path};
    logFile("INFO", "FSYNC", desc, 1);

    return 0;
}

static struct fuse_operations xmp_oper = {
    .getattr = xmp_getattr,
    .access = xmp_access,
    .readlink = xmp_readlink,
    .readdir = xmp_readdir,
    .mknod = xmp_mknod,
    .mkdir = xmp_mkdir,
    .symlink = xmp_symlink,
    .unlink = xmp_unlink,
    .rmdir = xmp_rmdir,
    .rename = xmp_rename,
    .link = xmp_link,
    .chmod = xmp_chmod,
    .chown = xmp_chown,
    .truncate = xmp_truncate,
    .utimens = xmp_utimens,
    .open = xmp_open,
    .read = xmp_read,
    .write = xmp_write,
    .statfs = xmp_statfs,
    .create = xmp_create,
    .release = xmp_release,
    .fsync = xmp_fsync,
};

int main(int argc, char *argv[])
{
    umask(0);

    return fuse_main(argc, argv, &xmp_oper, NULL);
}
