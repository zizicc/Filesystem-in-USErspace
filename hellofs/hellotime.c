/*
  FUSE: Filesystem in Userspace
  Copyright (C) 2001-2007  Miklos Szeredi <miklos@szeredi.hu>

  This program can be distributed under the terms of the GNU GPL.
  See the file COPYING.

  gcc -Wall hello.c `pkg-config fuse --cflags --libs` -o hello
*/

#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <time.h>

//static char *hello_time = "\n";
static const char *hello_str = "Hello World!\n";
static const char *hello_path = "/hello";
static const char *time_path = "/time";
static const char *now_path = "/time/now.txt";

const int TIMESTAMP_LEN = 20;
/**
* Write the current local time to the given character buffer,
* which must be at least TIMESTAMP_LEN+1 bytes long.
* The timestamp itself (excluding null terminator) will be
* exactly TIMESTAMP_LEN bytes long.
*/
void timestamp(char *output) {
	// Get the current time
	time_t raw_time;
	struct tm *time_info;
	time(&raw_time);
	time_info = localtime(&raw_time);
	// Format the timestamp
	strftime(output, TIMESTAMP_LEN+1, "%Y-%m-%d %H:%M:%S\n", time_info);
}

static int hello_getattr(const char *path, struct stat *stbuf)
{
	int res = 0;

	memset(stbuf, 0, sizeof(struct stat));
	if (strcmp(path, "/") == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	} else if (strcmp(path, hello_path) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_size = strlen(hello_str);
	} else if (strcmp(path, time_path) == 0) {
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;	
	}
	else if (strcmp(path, now_path) == 0) {
		stbuf->st_mode = S_IFREG | 0444;
		stbuf->st_nlink = 1;
		stbuf->st_uid = fuse_get_context()->uid;
        //stbuf->st_gid = fuse_get_context()->gid;
		stbuf->st_size = TIMESTAMP_LEN + 1;
		stbuf->st_mtime = time(NULL);
	} else
		res = -ENOENT;

	return res;
}

static int hello_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
			 off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;

	if (strcmp(path, "/") == 0) {
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, hello_path + 1, NULL, 0);
		filler(buf, time_path + 1, NULL, 0);
	} else if (strcmp(path, time_path) == 0) {
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL, 0);
		filler(buf, "now.txt", NULL, 0);		
	} else {
		return -ENOENT;
	}
		
	return 0;
}

static int hello_open(const char *path, struct fuse_file_info *fi)
{
	if (strcmp(path, hello_path) != 0 && strcmp(path, now_path) != 0)
		return -ENOENT;

	if ((fi->flags & 3) != O_RDONLY)
		return -EACCES;

	return 0;
}

static int hello_read(const char *path, char *buf, size_t size, off_t offset,
		      struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;
	if(strcmp(path, hello_path) == 0) {
			len = strlen(hello_str);
		if (offset < len) {
			if (offset + size > len)
				size = len - offset;
			memcpy(buf, hello_str + offset, size);
		} else
			size = 0;
	} else if (strcmp(path, now_path) == 0) {
		char time_str[TIMESTAMP_LEN + 1];
        timestamp(time_str);
        len = strlen(time_str);
        if (offset < len) {
            if (offset + size > len)
                size = len - offset;
            memcpy(buf, time_str + offset, size);
        } else {
            size = 0;
        }
	} else 
		return -ENOENT;

	return size;
}

static struct fuse_operations hello_oper = {
	.getattr	= hello_getattr,
	.readdir	= hello_readdir,
	.open		= hello_open,
	.read		= hello_read,
};

int main(int argc, char *argv[])
{
	return fuse_main(argc, argv, &hello_oper, NULL);
}
