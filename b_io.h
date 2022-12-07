/**************************************************************
* Class:  CSC-415
* Name: Alex Shirazi, Duy Nguyen, Vi Dao, Manuel Hernandez
* Student ID: 918794583,917131389,920353977,(unknown)
* TEAM-NAME:A-TEAM
* Project: Basic File System
*
* File: b_io.h
*
* Description:header file for b_io.c
**************************************************************/

#ifndef _B_IO_H
#define _B_IO_H
#include <fcntl.h>

typedef struct 
	{
	char* DirLoc[255];//holds the path from root to loc in a dir
    int FirstDataBlk;//LBA of the first data block
	int buflen;		//holds how many valid bytes are in the buffer
    int FileLen;//set to zero when we make a new fcb,holds total bytes stored of valid data
	} FCB;
typedef struct{
    char data[504];
    int ByteStored;
    int NextLBA;
}dataBlk;

int b_open (char * filename, int flags);//we open and establish a DE and file on our end   
int b_read (int fd, char * buffer, int count);//read from an LBA into buffer, fd is a file on our end we get from b_open
int b_write (int fd, char * buffer, int count);//we read from the char buffer count bytes into our own fd
int b_seek (int fd, off_t offset, int whence);// UNFINISHED 
void b_close (int fd);//close the file on our end
//NOTE:the ints being returned are the lba of the first block of the file 
//which will hold the fcb for that file 
#endif

