#define FUSE_USE_VERSION 28

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <wait.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>

//path yang dibutuhkan
static const char *dirPath = "/home/bennett/Downloads";
static const char *logPath = "/home/bennett/SinSeiFS.log";

//array untuk menampung string yang diperlukan
char *key = "SISOP";
char *atoz = "AtoZ_";
char *rx = "RX_";
char *aisa = "A_is_a_";

int x = 0;

//mendapatkan lokasi extension
int extensionId(char *path)
{
	int ada = 0;
	for (int i = strlen(path) - 1; i >= 0; i--)
	{
		if (path[i] == '.')
		{
			if (ada == 1)
				return i;
			else
				ada = 1;
		}
	}
	return strlen(path);
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

//mendapat letak /
int slashId(char *path, int end)
{
	for (int i = 0; i < strlen(path); i++)
	{
		if (path[i] == '/')
			return i + 1;
	}
	return end;
}

void encryptAtbash(char *path)
{
	if (!strcmp(path, ".") || !strcmp(path, ".."))
		return;

	printf("encrypt path Atbash: %s\n", path);

	int endId = extensionId(path);
	if (endId == strlen(path))
		endId = dotId(path);
	int startId = slashId(path, 0);

	for (int i = startId; i < endId; i++)
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

	printf("decrypt path Atbash: %s\n", path);

	int endId = extensionId(path);
	if (endId == strlen(path))
		endId = dotId(path);
	int startId = slashId(path, endId);

	for (int i = startId; i < endId; i++)
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

//no2
void encryptRot13(char *path)
{
	if (!strcmp(path, ".") || !strcmp(path, ".."))
		return;

	printf("encrypt path ROT13: %s\n", path);

	int endId = extensionId(path);
	int startId = slashId(path, 0);

	for (int i = startId; i < endId; i++)
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

	printf("decrypt path ROT13: %s\n", path);

	int endId = extensionId(path);
	int startId = slashId(path, endId);

	for (int i = startId; i < endId; i++)
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

	printf("encrypt path Vigenere: %s\n", path);

	int endId = extensionId(path);
	int startId = slashId(path, 0);

	for (int i = startId; i < endId; i++)
	{
		if (path[i] != '/' && isalpha(path[i]))
		{
			char tmp1 = path[i];
			char tmp2 = key[(i - startId) % strlen(key)];
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

	printf("decrypt path Vigenere: %s\n", path);

	int endId = extensionId(path);
	int startId = slashId(path, endId);

	for (int i = startId; i < endId; i++)
	{
		if (path[i] != '/' && isalpha(path[i]))
		{
			char tmp1 = path[i];
			char tmp2 = key[(i - startId) % strlen(key)];
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

//no2
void enkripsi2(char *fpath)
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
			enkripsi2(dirPath);
			printf("dirpath %s\n", dirPath);
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

void dekripsi2(char *dir)
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
			dekripsi2(dirPath);
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
//no3
void getBinary(char *fname, char *bin, char *lowercase)
{
	int endId = dotId(fname);
	int startId = slashId(fname, 0);
	int i;

	for (i = startId; i < endId; i++)
	{
		if (isupper(fname[i]))
		{
			bin[i] = '1';
			lowercase[i] = fname[i] + 32;
		}
		else
		{
			bin[i] = '0';
			lowercase[i] = fname[i];
		}
	}
	bin[endId] = '\0';

	for (; i < strlen(fname); i++)
	{
		lowercase[i] = fname[i];
	}
	lowercase[i] = '\0';
}

int binToDec(char *bin)
{
	int tmp = 1, res = 0;
	for (int i = strlen(bin) - 1; i >= 0; i--)
	{
		if (bin[i] == '1')
			res += tmp;
		tmp *= 2;
	}
	return res;
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
	char filePathBinary[1000];

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
			char bin[1000], lowercase[1000];
			getBinary(dir->d_name, bin, lowercase);
			int dec = binToDec(bin);
			sprintf(filePathBinary, "%s/%s.%d", fpath, lowercase, dec);
			rename(filePath, filePathBinary);
		}
	}
	closedir(dp);
}

int convertDec(char *ext)
{
	int dec = 0, pengali = 1;
	for (int i = strlen(ext) - 1; i >= 0; i--)
	{
		dec += (ext[i] - '0') * pengali;
		pengali *= 10;
	}
	return dec;
}

void decToBin(int dec, char *bin, int len)
{
	int idx = 0;
	while (dec)
	{
		if (dec & 1)
			bin[idx] = '1';
		else
			bin[idx] = '0';
		idx++;
		dec /= 2;
	}
	while (idx < len)
	{
		bin[idx] = '0';
		idx++;
	}
	bin[idx] = '\0';

	for (int i = 0; i < idx / 2; i++)
	{
		char tmp = bin[i];
		bin[i] = bin[idx - 1 - i];
		bin[idx - 1 - i] = tmp;
	}
}

void getDecimal(char *fname, char *bin, char *normalcase)
{
	int endId = dotId(fname);
	int startId = slashId(fname, 0);
	int i;

	for (i = startId; i < endId; i++)
	{
		if (bin[i - startId] == '1')
			normalcase[i - startId] = fname[i] - 32;
		else
			normalcase[i - startId] = fname[i];
	}

	for (; i < strlen(fname); i++)
	{
		normalcase[i - startId] = fname[i];
	}
	normalcase[i - startId] = '\0';
}

void decryptBinary(char *fpath)
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
	char filePathDecimal[1000];

	while ((dir = readdir(dp)) != NULL)
	{
		if (stat(dir->d_name, &st) < 0)
			;
		else if (S_ISDIR(st.st_mode))
		{
			if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
				continue;
			sprintf(dirPath, "%s/%s", fpath, dir->d_name);
			decryptBinary(dirPath);
		}
		else
		{
			sprintf(filePath, "%s/%s", fpath, dir->d_name);
			char fname[1000], bin[1000], normalcase[1000], clearPath[1000];

			strcpy(fname, dir->d_name);
			char *ext = strrchr(fname, '.');
			int dec = convertDec(ext + 1);
			for (int i = 0; i < strlen(fname) - strlen(ext); i++)
				clearPath[i] = fname[i];

			char *ext2 = strrchr(clearPath, '.');
			decToBin(dec, bin, strlen(clearPath) - strlen(ext2));
			getDecimal(clearPath, bin, normalcase);
			sprintf(filePathDecimal, "%s/%s", fpath, normalcase);
			rename(filePath, filePathDecimal);
		}
	}
	closedir(dp);
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

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	if (x != 24)
		x++;
	else
	{
		const char *desc[] = {path};
		logFile("INFO", "READDIR", desc, 1);
	}

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

		if (a != NULL)
			encryptAtbash(de->d_name);
		if (b != NULL)
		{
			encryptAtbash(de->d_name);
			encryptRot13(de->d_name);
		}
		res = (filler(buf, de->d_name, &st, 0));
		if (res != 0)
			break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	int res = 0;
	int fd = 0;

	(void)fi;

	const char *desc[] = {fpath};
	logFile("INFO", "READ", desc, 1);

	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	const char *desc[] = {fpath};
	logFile("INFO", "MKDIR", desc, 1);

	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_rmdir(const char *path)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	const char *desc[] = {fpath};
	logFile("WARNING", "RMDIR", desc, 1);

	if (res == -1)
		return -errno;
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
		enkripsi2(topath);
	}

	if (b != NULL && c == NULL)
	{
		dekripsi2(topath);
	}

	if (strstr(to, aisa) != NULL)
	{
		encryptBinary(topath);
	}

	if (strstr(from, aisa) != NULL && strstr(to, aisa) == NULL)
	{
		decryptBinary(topath);
	}

	return 0;
}

static int xmp_truncate(const char *path, off_t size)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	res = truncate(fpath, size);

	const char *desc[] = {path};
	logFile("INFO", "TRUNCATE", desc, 1);

	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size, off_t offset, struct fuse_file_info *fi)
{
	int fd;
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	(void)fi;

	const char *desc[] = {path};
	logFile("INFO", "WRITE", desc, 1);

	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	(void)fi;

	const char *desc[] = {path};
	logFile("INFO", "CREATE", desc, 1);

	res = creat(fpath, mode);
	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);

	const char *desc[] = {path};
	logFile("INFO", "UTIMENSAT", desc, 1);

	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_access(const char *path, int mask)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	res = access(fpath, mask);

	const char *desc[] = {path};
	logFile("INFO", "ACCESS", desc, 1);

	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	res = open(fpath, fi->flags);

	const char *desc[] = {path};
	logFile("INFO", "OPEN", desc, 1);

	if (res == -1)
		return -errno;

	close(res);
	return 0;
}

static int xmp_unlink(const char *path)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	res = unlink(fpath);

	const char *desc[] = {path};
	logFile("WARNING", "UNLINK", desc, 1);

	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_readlink(const char *path, char *buf, size_t size)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	res = readlink(fpath, buf, size - 1);

	const char *desc[] = {path};
	logFile("INFO", "READLINK", desc, 1);

	if (res == -1)
		return -errno;

	buf[res] = '\0';
	return 0;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	const char *desc[] = {path};
	logFile("INFO", "MKNOD", desc, 1);

	/* On Linux this could just be 'mknod(path, mode, rdev)' but this is more portable */
	if (S_ISREG(mode))
	{
		res = open(fpath, O_CREAT | O_EXCL | O_WRONLY, mode);
		if (res >= 0)
			res = close(res);
	}
	else if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);

	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_symlink(const char *from, const char *to)
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

	res = symlink(frompath, topath);

	const char *desc[] = {from, to};
	logFile("INFO", "SYMLINK", desc, 2);

	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_link(const char *from, const char *to)
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

	res = link(frompath, topath);

	const char *desc[] = {from, to};
	logFile("INFO", "LINK", desc, 2);

	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
	char fpath[1000];
	char modeString[100];
	sprintf(modeString, "%d", mode);

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	res = chmod(fpath, mode);

	const char *desc[] = {path, modeString};
	logFile("INFO", "CHMOD", desc, 2);

	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_chown(const char *path, uid_t uid, gid_t gid)
{
	int res;
	char fpath[1000];
	char uidString[100];
	char gidString[100];
	sprintf(uidString, "%d", uid);
	sprintf(gidString, "%d", gid);

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	res = lchown(fpath, uid, gid);

	const char *desc[] = {path, uidString, gidString};
	logFile("INFO", "CHOWN", desc, 3);

	if (res == -1)
		return -errno;
	return 0;
}

static int xmp_statfs(const char *path, struct statvfs *stbuf)
{
	int res;
	char fpath[1000];

	char *a = strstr(path, atoz);
	if (a != NULL)
		decryptAtbash(a);

	char *b = strstr(path, rx);
	if (b != NULL)
	{
		decryptRot13(b);
		decryptAtbash(b);
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

	res = statvfs(fpath, stbuf);

	const char *desc[] = {path};
	logFile("INFO", "STATFS", desc, 1);

	if (res == -1)
		return -errno;
	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr = xmp_getattr,
	.readdir = xmp_readdir,
	.read = xmp_read,
	.mkdir = xmp_mkdir,
	.rmdir = xmp_rmdir,
	.rename = xmp_rename,
	.truncate = xmp_truncate,
	.write = xmp_write,
	.create = xmp_create,
	.utimens = xmp_utimens,
	.access = xmp_access,
	.open = xmp_open,
	.unlink = xmp_unlink,
	.readlink = xmp_readlink,
	.mknod = xmp_mknod,
	.symlink = xmp_symlink,
	.link = xmp_link,
	.chmod = xmp_chmod,
	.chown = xmp_chown,
	.statfs = xmp_statfs,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
