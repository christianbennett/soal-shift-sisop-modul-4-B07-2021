#define FUSE_USE_VERSION 28

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>

//path yang dibutuhkan
static const char *dirPath = "/home/bennett/Downloads";
static const char *logPath = "/home/bennett/SinSeiFS.log";

//array untuk menampung string yang diperlukan
char *key = "SISOP";
char *atoz = "AtoZ_";
char *rx = "RX_";
char *aisa = "A_is_a_";

//mendapatkan lokasi "/""
int slashId(char *path, int last)
{
    for (int i = 0; i < strlen(path); i++)
    {
        if (path[i] == '/')
            return i + 1;
    }
    return last;
}

//mendapatkan lokasi "."
int dotId(char *path)
{
    for (int i = strlen(path) - 1; i >= 0; i--)
    {
        if (path[i] == '.')
            return i;
    }
    return strlen(path);
}

//mendapatkan lokasi extension
int extensionId(char *path)
{
    int flag = 0;
    for (int i = strlen(path) - 1; i >= 0; i--)
    {
        if (path[i] == '.')
        {
            if (flag == 1)
                return i;
            else
                flag = 1;
        }
    }
    return strlen(path);
}

void encryptAtbash(char *path)
{
    if (!strcmp(path, ".") || !strcmp(path, ".."))
        return;

    printf("Atbash Encrypt Path: %s\n", path);

    int endid = extensionId(path);
    if (endid == strlen(path))
        endid = ext_id(path);
    int startid = slashId(path, 0);

    for (int i = startid; i < endid; i++)
    {
        if (path[i] != '/' && isalpha(path[i]))
        {
            char tmp = path[i];
            if (isupper(path[i]))
                tmp -= 'A';
            else
                tmp -= 'a';
            tmp = 25 - tmp; //Atbash cipher
            if (isupper(path[i]))
                tmp += 'A';
            else
                tmp += 'a';
            path[i] = tmp;
        }
    }
}

void decryptAtbash(char *path)
{
    if (!strcmp(path, ".") || !strcmp(path, ".."))
        return;

    printf("Atbash Decrypt Path: %s\n", path);

    int endid = extensionId(path);
    if (endid == strlen(path))
        endid = ext_id(path);
    int startid = slashId(path, endid);

    for (int i = startid; i < endid; i++)
    {
        if (path[i] != '/' && isalpha(path[i]))
        {
            char tmp = path[i];
            if (isupper(path[i]))
                tmp -= 'A';
            else
                tmp -= 'a';
            tmp = 25 - tmp; //Atbash cipher
            if (isupper(path[i]))
                tmp += 'A';
            else
                tmp += 'a';
            path[i] = tmp;
        }
    }
}

void encryptRot13(char *path)
{
    if (!strcmp(path, ".") || !strcmp(path, ".."))
        return;

    printf("ROT14 Encrypt Path: %s\n", path);

    int endid = extensionId(path);
    int startid = slashId(path, 0);

    for (int i = startid; i < endid; i++)
    {
        if (path[i] != '/' && isalpha(path[i]))
        {
            char tmp = path[i];
            if (isupper(path[i]))
                tmp -= 'A';
            else
                tmp -= 'a';
            tmp = (tmp + 13) % 26; //ROT13 cipher
            if (isupper(path[i]))
                tmp += 'A';
            else
                tmp += 'a';
            path[i] = tmp;
        }
    }
}

void decryptRot13(char *path)
{
    if (!strcmp(path, ".") || !strcmp(path, ".."))
        return;

    printf("ROT13 Decrypt Path: %s\n", path);

    int endid = extensionId(path);
    int startid = slashId(path, endid);

    for (int i = startid; i < endid; i++)
    {
        if (path[i] != '/' && isalpha(path[i]))
        {
            char tmp = path[i];
            if (isupper(path[i]))
                tmp -= 'A';
            else
                tmp -= 'a';
            tmp = (tmp + 13) % 26; //ROT13 cipher
            if (isupper(path[i]))
                tmp += 'A';
            else
                tmp += 'a';
            path[i] = tmp;
        }
    }
}

void encryptVigenere(char *path)
{
    if (!strcmp(path, ".") || !strcmp(path, ".."))
        return;

    printf("Vigenere Encrypt Path: %s\n", path);

    int endid = extensionId(path);
    int startid = slashId(path, 0);

    for (int i = startid; i < endid; i++)
    {
        if (path[i] != '/' && isalpha(path[i]))
        {
            char tmp1 = path[i];
            char tmp2 = key[(i - startid) % strlen(key)];
            if (isupper(path[i]))
            {
                tmp1 -= 'A';
                tmp2 -= 'A';
                tmp1 = (tmp1 + tmp2) % 26; //Vigenere cipher
                tmp1 += 'A';
            }
            else
            {
                tmp1 -= 'a';
                tmp2 = tolower(tmp2) - 'a';
                tmp1 = (tmp1 + tmp2) % 26; //Vigenere cipher
                tmp1 += 'a';
            }
            path[i] = tmp1;
        }
    }
}

void decryptVigenere(char *path)
{
    if (!strcmp(path, ".") || !strcmp(path, ".."))
        return;

    printf("Vigenere Decrypt Path: %s\n", path);

    int endid = extensionId(path);
    int startid = slashId(path, endid);

    for (int i = startid; i < endid; i++)
    {
        if (path[i] != '/' && isalpha(path[i]))
        {
            char tmp1 = path[i];
            char tmp2 = key[(i - startid) % strlen(key)];
            if (isupper(path[i]))
            {
                tmp1 -= 'A';
                tmp2 -= 'A';
                tmp1 = (tmp1 - tmp2 + 26) % 26; //Vigenere cipher
                tmp1 += 'A';
            }
            else
            {
                tmp1 -= 'a';
                tmp2 = tolower(tmp2) - 'a';
                tmp1 = (tmp1 - tmp2 + 26) % 26; //Vigenere cipher
                tmp1 += 'a';
            }
            path[i] = tmp1;
        }
    }
}

void encrypt2(char *fpath)
{
    chdir(fpath);
    DIR *dp;
    struct dirent *dir;

    dp = opendir(".");
    if (dp == NULL)
        return;
    struct stat st;
    char dirPath[1000];
    char filePath[1000];

    while ((dir = readdir(dp)) != NULL)
    {
        printf("dirname %s\n", dir->d_name);
        printf("%s/%s\n", fpath, dir->d_name);
        if (stat(dir->d_name, &st) < 0)
            ;
        else if (S_ISDIR(st.st_mode))
        {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;
            sprintf(dirPath, "%s/%s", fpath, dir->d_name);
            encrypt2(dirPath);
            printf("dirPath %s\n", dirPath);
        }
        else
        {
            sprintf(filePath, "%s/%s", fpath, dir->d_name);
            FILE *input = fopen(filePath, "r");
            if (input == NULL)
                return;
            int bytes_read, count = 0;
            void *buffer = malloc(1024);
            while ((bytes_read = fread(buffer, 1, 1024, input)) > 0)
            {
                char newFilePath[1000];
                sprintf(newFilePath, "%s.%04d", filePath, count);
                count++;
                FILE *output = fopen(newFilePath, "w+");
                if (output == NULL)
                    return;
                fwrite(buffer, 1, bytes_read, output);
                fclose(output);
                memset(buffer, 0, 1024);
            }
            fclose(input);
            printf("filepath %s\n", filePath);
            remove(filePath);
        }
    }
    closedir(dp);
}

void decrypt2(char *dir)
{
    chdir(dir);
    DIR *dp;
    struct dirent *de;
    struct stat st;
    dp = opendir(".");
    if (dp == NULL)
        return;

    char dirPath[1000];
    char filePath[1000];

    while ((de = readdir(dp)) != NULL)
    {
        if (stat(de->d_name, &st) < 0)
            ;
        else if (S_ISDIR(st.st_mode))
        {
            if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
                continue;
            sprintf(dirPath, "%s/%s", dir, de->d_name);
            decrypt2(dirPath);
        }
        else
        {
            sprintf(filePath, "%s/%s", dir, de->d_name);
            filePath[strlen(filePath) - 5] = '\0';
            FILE *check = fopen(filePath, "r");
            if (check != NULL)
                return;
            FILE *file = fopen(filePath, "w");
            int count = 0;
            char topath[1000];
            sprintf(topath, "%s.%04d", filePath, count);
            void *buffer = malloc(1024);
            while (1)
            {
                FILE *op = fopen(topath, "rb");
                if (op == NULL)
                    break;
                size_t readSize = fread(buffer, 1, 1024, op);
                fwrite(buffer, 1, readSize, file);
                fclose(op);
                remove(topath);
                count++;
                sprintf(topath, "%s.%04d", filePath, count);
            }
            free(buffer);
            fclose(file);
        }
    }
    closedir(dp);
}

//untuk mendapatkan strlen tanpa extension
int len(char str[])
{
    int count = 0;
    for (int i = 0; i < strlen(str); i++)
    {
        count++;
        if (str[i] == '.')
        {
            count--;
            break;
        }
    }
    return count;
}

//compare 2 string jika beda
void compareString(char str1[], char str2[], char arr[])
{
    for (int i = 0; i < len(str1); i++)
    {
        if (str1[i] != str2[i])
        {
            arr[i] = '1';
        }
        else
        {
            arr[i] = '0';
        }
    }
}

//convert decimal to binary
int convert(long long int n)
{
    int dec = 0, i = 0, rem;
    while (n != 0)
    {
        rem = n % 10;
        n /= 10;
        dec += rem * pow(2, i);
        ++i;
    }
    return dec;
}

//concat pada string
void appendString(char str1[])
{
    char str2[strlen(str1)];

    for (int i = 0; i < strlen(str1); i++)
    {
        str2[i] = tolower(str1[i]);
    }

    int size = len(str1);

    char arr[size];

    compareString(str1, str2, arr);

    long long int binary = atoll(arr);

    // printf("Binary number = %lld\n", binary);

    int decimal = convert(binary);

    // printf("Decimal number = %d\n", decimal);

    char decimalString[size];

    sprintf(decimalString, "%d", decimal);

    // printf("Decimal number (String) = %s\n", decimalString);
    // printf("str1 = %s\n", str1);
    // printf("decimalString = %s\n", decimalString);

    for (int i = 0; str1[i]; i++)
    {
        str1[i] = tolower(str1[i]);
    }

    strcat(str1, ".");
    strcat(str1, decimalString);

    // printf("New String = %s", str1);
}

void encryptBinary(char *fpath)
{
    chdir(fpath);

    DIR *dp;
    struct dirent *dir;
    struct stat st;

    dp = opendir(".");
    if (dp == NULL)
        return;

    char dirPath[1000];
    char filePath[1000];
    char binaryFilePath[1000];
    char temp[1000];

    while ((dir = readdir(dp)) != NULL)
    {
        if (stat(dir->d_name, &st) < 0)
            ;
        else if (S_ISDIR(st.st_mode))
        {
            if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
                continue;
            sprintf(dirPath, "%s/%s", fpath, dir->d_name);
            encryptBinary(dirPath);
        }
        else
        {
            sprintf(filePath, "%s/%s", fpath, dir->d_name);
            sprintf(temp, "%s", dir->d_name);
            appendString(temp);
            sprintf(binaryFilePath, "%s/%s", fpath, temp);
            rename(filePath, binaryFilePath);
        }
    }
    closedir(dp);
}

void decryptBinary(char *fpath)
{
    // chdir(fpath);

    // DIR *dp;
    // struct dirent *dir;
    // struct stat st;

    // dp = opendir(".");
    // if (dp == NULL)
    //     return;

    // char dirPath[1000];
    // char filePath[1000];
    // char decimalFilePath[1000];
    // char fname[1000];
    // char bin[1000];
    // char normalcase[1000];
    // char clearPath[1000];

    // while ((dir = readdir(dp)) != NULL)
    // {
    //     if (stat(dir->d_name, &st) < 0)
    //         ;
    //     else if (S_ISDIR(st.st_mode))
    //     {
    //         if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
    //             continue;
    //         sprintf(dirPath, "%s/%s", fpath, dir->d_name);
    //         decryptBinary(dirPath);
    //     }
    //     else
    //     {
    //         sprintf(filePath, "%s/%s", fpath, dir->d_name);

    //         strcpy(fname, dir->d_name);
    //         char *ext = strrchr(fname, '.');
    //         int dec = convertDec(ext + 1);
    //         for (int i = 0; i < strlen(fname) - strlen(ext); i++)
    //             clearPath[i] = fname[i];

    //         char *ext2 = strrchr(clearPath, '.');
    //         dec_to_bin(dec, bin, strlen(clearPath) - strlen(ext2));
    //         getDecimal(clearPath, bin, normalcase);
    //         sprintf(decimalFilePath, "%s/%s", fpath, normalcase);
    //         rename(filePath, decimalFilePath);
    //     }
    // }
    // closedir(dp);
}

//fungsi log no.4
void logFile(char *level, char *command, const char *desc[], int descLen)
{
    FILE *f = fopen(logPath, "a");
    time_t t;
    struct tm *tmp;
    char timeString[100];

    time(&t);
    tmp = localtime(&t);
    strftime(timeString, sizeof(timeString), "%d%m%y-%H:%M:%S", tmp);

    fprintf(f, "%s::%s:%s", level, timeString, command);
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

    // sprintf(fpath, "%s%s", dirPath, path);

    if (strstr(path, atoz) != NULL)
    {
        decryptAtbash(strstr(path, atoz));
    }

    if (strstr(path, rx) != NULL)
    {
        decryptRot13(strstr(path, rx));
        decryptAtbash(strstr(path, rx));
    }

    if (strcmp(path, "/") == 0)
    {
        path = dirPath;
        sprintf(fpath, "%s", path);
    }
    else
    {
        sprintf(fpath, "%s%s", dirPath, path);
    }

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

    if (strstr(path, atoz) != NULL)
    {
        decryptAtbash(strstr(path, atoz));
    }

    if (strstr(path, rx) != NULL)
    {
        decryptRot13(strstr(path, rx));
        decryptAtbash(strstr(path, rx));
    }

    if (strcmp(path, "/") == 0)
    {
        path = dirPath;
        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirPath, path);

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
        if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
            continue;

        struct stat st;
        memset(&st, 0, sizeof(st));
        st.st_ino = de->d_ino;
        st.st_mode = de->d_type << 12;

        if (strstr(path, atoz) != NULL)
            encryptAtbash(de->d_name);
        if (strstr(path, rx) != NULL)
        {
            encryptAtbash(de->d_name);
            encryptRot13(de->d_name);
        }
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

    if (strstr(path, atoz) != NULL)
    {
        decryptAtbash(strstr(path, atoz));
    }

    if (strstr(path, rx) != NULL)
    {
        decryptRot13(strstr(path, rx));
        decryptAtbash(strstr(path, rx));
    }

    if (strcmp(path, "/") == 0)
    {
        path = dirPath;

        sprintf(fpath, "%s", path);
    }
    else
        sprintf(fpath, "%s%s", dirPath, path);

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

    const char *desc[] = {fpath};
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
    char fpath[1000];

    if (strstr(path, atoz) != NULL)
    {
        decryptAtbash(strstr(path, atoz));
    }

    if (strstr(path, rx) != NULL)
    {
        decryptRot13(strstr(path, rx));
        decryptAtbash(strstr(path, rx));
    }

    if (strcmp(path, "/") == 0)
    {
        path = dirPath;
        sprintf(fpath, "%s", path);
    }

    else
    {
        sprintf(fpath, "%s%s", dirPath, path);
    }

    res = mkdir(fpath, mode);
    if (res == -1)
        return -errno;
    const char *desc[] = {fpath};
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
    char fpath[1000];

    if (strstr(path, atoz) != NULL)
    {
        decryptAtbash(strstr(path, atoz));
    }

    if (strstr(path, rx) != NULL)
    {
        decryptRot13(strstr(path, rx));
        decryptAtbash(strstr(path, rx));
    }

    if (strcmp(path, "/") == 0)
    {
        path = dirPath;
        sprintf(fpath, "%s", path);
    }

    else
    {
        sprintf(fpath, "%s%s", dirPath, path);
    }

    res = rmdir(fpath);
    if (res == -1)
        return -errno;

    const char *desc[] = {fpath};
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
    char frompath[1000], topath[1000];

    char *a = strstr(to, atoz);
    if (a != NULL)
        decryptAtbash(a);

    char *b = strstr(from, rx);
    if (b != NULL)
    {
        decryptRot13(b);
        decryptAtbash(b);
    }

    char *c = strstr(to, rx);
    if (c != NULL)
    {
        decryptRot13(c);
        decryptAtbash(c);
    }

    sprintf(frompath, "%s%s", dirPath, from);
    sprintf(topath, "%s%s", dirPath, to);

    res = rename(frompath, topath);
    if (res == -1)
        return -errno;

    const char *desc[] = {frompath, topath};
    logFile("INFO", "RENAME", desc, 2);

    if (c != NULL)
    {
        encrypt2(topath);
    }

    if (b != NULL && c == NULL)
    {
        decrypt2(topath);
    }

    if (strstr(to, aisa) != NULL)
    {
        encryptBinary(topath);
    }

    if (strstr(from, aisa) != NULL && strstr(to, aisa) == NULL)
    {
        decryptBinary(topath);
    }
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
    char modeString[100];
    sprintf(modeString, "%d", mode);

    int res;

    res = chmod(path, mode);
    if (res == -1)
        return -errno;

    const char *desc[] = {path, modeString};
    logFile("INFO", "CHMOD", desc, 2);

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
    logFile("INFO", "CREATE", desc, 1);

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

    const char *desc[] = {path};
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
