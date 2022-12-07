/**************************************************************
* Class:  CSC-415
* Name: Professor Bierman
* Student ID: N/A
* Project: Basic File System
*
* File: mfs.h 
* Description: header file for mfs.c
**************************************************************/
#ifndef _MFS_H
#define _MFS_H
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include "b_io.h"
#include <dirent.h>
#include <string.h>
#include "FSAPI.h"
#include <stdio.h>

#define FT_REGFILE	DT_REG
#define FT_DIRECTORY DT_DIR
#define FT_LINK	DT_LNK

#ifndef uint64_t
typedef u_int64_t uint64_t;
#endif
#ifndef uint32_t
typedef u_int32_t uint32_t;
#endif

#define MAX_DIR_NAME 20

#define FILENAME_SIZE 20
#define bsIZE 1024

//test if cd works on a multi block dir for the cd searching implementation
struct fs_diriteminfo
	{
    unsigned short d_reclen;    /* length of this record */
    unsigned char fileType;    
    char d_name[256]; 			/* filename max filename is 255 characters */
	

	};//I think this is only in memory to iterate through the directories and maybe files

typedef struct//48 bytes bytes 
	{
	/*****TO DO:  Fill in this structure with what your open/read directory needs  *****/
	unsigned short  d_reclen;		/*length of this record */
	unsigned short	dirEntryPosition;	/*which directory entry position, like file pos.*/
	//Right now this field is not utilized but potential use is to hold the LBA block that holds inodes or blocks containing fs_stat sctructs
	uint64_t	directoryStartLocation;		/*Starting LBA of directory */
	//both
	char name[FILENAME_SIZE];
	int type;//0-free 1-file 2-dir 
	
	//file
	int numBytes;//size of the file, though may use this for dir also need to think about it
	
	//dir

	} fdDir;//im thinking this is the info for each directory entry (which may be a file or directory) and this struct is what 
	// we LBA WRITE either in B_io/write ( for file creation) or in mkDir(for directory creation)
	//need one for every dirEntry
	//will need one for DE 0 AND 1 where names are . and ..
 char dirNames[20][MAX_DIR_NAME];
 char* cdBuf;
	//NOTE
	// probably need a few custom functions for logic used in multiple functions 
 //char* defaultDirName;//= "empty DE";
					//Do i need to specify size? wtf
 char* CurrentDir;//= malloc(bsIZE);
int fs_mkdir(const char *pathname, mode_t mode);//STATE: FINISHED TESTING: FINISHED
int fs_rmdir(const char *pathname);//STATE: FINISHED TESTING: FINISHED(?)
fdDir * fs_opendir(const char *name);//STATE: UNFINISHED TESTING: UNFINISHED
struct fs_diriteminfo *fs_readdir(fdDir *dirp);//STATE: UNFINISHED TESTING: UNFINISHED
int fs_closedir(fdDir *dirp);//STATE: UNFINISHED TESTING: UNFINISHED

char * fs_getcwd(char *buf, size_t size);//STATE: FINISHED TESTING: UNFINISHED
int fs_setcwd(char *buf);   //linux chdir	//STATE: FINISHED TESTING: UNFINISHED
int fs_isFile(char * path);	//return 1 if file, 0 otherwise		//STATE: UNFINISHED TESTING: UNFINISHED
int fs_isDir(char * path);		//return 1 if directory, 0 otherwise	//STATE: UNFINISHED TESTING: UNFINISHED
int fs_delete(char* filename);	//removes a file	//STATE: UNFINISHED TESTING: UNFINISHED



struct fs_stat
	{
	off_t     st_size;    		/* total size, in bytes */
	blksize_t st_blksize; 		/* blocksize for file system I/O */
	blkcnt_t  st_blocks;  		/* number of 512B blocks allocated */
	time_t    st_accesstime;   	/* time of last access */
	time_t    st_modtime;   	/* time of last modification */
	time_t    st_createtime;   	/* time of last status change */
	
	/* add additional attributes here for your file system */
	};

int fs_stat(const char *path, struct fs_stat *buf);
mv_dir(char* filename, char* path);
int fs_rmFile(const char *pathname);
#endif

