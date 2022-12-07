/**************************************************************
* Class:  CSC-415
* Name: Alex Shirazi, Duy Nguyen, Vi Dao, Manuel Hernandez
* Student ID: 918794583,917131389,920353977,(unknown)
* TEAM-NAME:A-TEAM
* Project: Basic File System
*
* File: b_io.c
*
* Description: input out put file that reads files in and out of the file system
*
**************************************************************/
#include "b_io.h"
#include "mfs.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>			// for malloc
#include <string.h>			// for memcpy
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "FSAPI.h"
#define blockSize 512


int b_open (char * filename, int flags){// we open and establish a DE and file on our end
    //Establishing ptrs
    fdDir* DE;//Directory Entries to iteratate through
    FCB* fcb;//file control block
    fcb = malloc(sizeof(*fcb));
    int size= sizeof(*DE);
    DE = malloc(size);
    char *buf = malloc(blockSize);
    int pos,LBA;
    LBA=absolutePathChild(CurrentDir);
    memset(buf,0,blockSize);//sets every byte in buf to 0 makeing sure there is no garabage data
    LBAread(buf,1,LBA);
    
    //checks to see if dir already exists
    //loop through entries before adding directory to make sure theres no dupliates
    for(int i=2;i<(blockSize/size);i++){
        memmove(DE,buf+(size*i),size);
        if(strcmp(DE->name,filename)==0){
            printf("\nfile already exists please choose another name!\n");
            return 1;
        }
        else if (strcmp("CAP",DE->name)==0){//we've reached the end on a block of a directory that spans multiple blocks
            i=0;
            LBAread(buf,1,DE->directoryStartLocation);
        }
    }
    for(int i = 0;i<(blockSize/size);i++){//debug and trace this block traversal
        memmove(DE,buf+(size*i),size);
        if(DE->type==0){
            DE->type=1;
            pos=i;
            i=10;
            memset(DE->name,0,20);//clears out garbage data IMPORTANT!
            strcpy(DE->name,filename); 
            DE->directoryStartLocation=allocate();//actually the location of the fcb
            memcpy(buf+(size*pos),DE,size);
            LBAwrite(buf,1,6);//hardcoded
            fcb->buflen=0;
            fcb->FileLen=0;
            fcb->FirstDataBlk=allocate();

            LBAwrite(fcb,1,DE->directoryStartLocation);// this line was used to init a new directory but needs to be changed to establish new file lba's
            LBA = DE->directoryStartLocation;
            free(buf);//the scope of this if statement might need to be changed
            free(DE);
        }
        else if(strcmp(DE->name,"NULL")==0){//reached the end of directory and no empty space was found so must establish new block
                DE->directoryStartLocation=allocate();
                strcpy(DE->name,"CAP");
                memcpy(buf+(size*9),DE,size);
                LBAwrite(buf,1,LBA);
                initBlk(DE->directoryStartLocation);
                LBAread(buf,1,DE->directoryStartLocation);
                i=0;//did a new read on the newly inited block now looking for first empty DE which should be the first entry

        }
        else if(strcmp(DE->name,"CAP")==0){//reached the end of a directory block but load next block as directory spans mulitple blocks
            LBAread(buf,1,DE->directoryStartLocation);
            i = 0;
        }

    }
    return  LBA;//returns the LBA for the fcb of the newly opened file

}
int b_read (int fd, char * buffer, int count){//read from an LBA into buffer, fd is a file on our end we get from b_open

}

int b_write (int fd, char * buffer, int count){// we read from the char buffer count bytes into our own fd
    FCB* file;
    int size = sizeof(*file);
    file= malloc(size);
    dataBlk *curr;
    curr = malloc(sizeof(*curr));
    char * temp= malloc(504);
    LBAread(file,1,fd);//read fcb into ptr file
    LBAread(curr,1,file->FirstDataBlk);//reads in first data block pointed to by the fcb
    if(curr->ByteStored>=504){
        if(file->FileLen==0){//first write
            strcpy(curr->data,buffer);
            curr->ByteStored=count;
            file->FileLen=+count;
            LBAwrite(curr,1,file->FirstDataBlk);//double check, first write onto disk
        }
        else if(curr->ByteStored<504){
            if((504-curr->ByteStored)>count){
                strcpy(temp,curr->data);
                file->FileLen=+count;
                curr->ByteStored=+count;
                strcat(temp,buffer);//combines old data with new data read
                LBAwrite(curr,1,file->FirstDataBlk);
            }
            else if((504-curr->ByteStored)<count){//not enough space in the first data block so must establish a new data block
                int BytesLeft = 504-curr->ByteStored;
                strcpy(temp,curr->data);
                char *half1=malloc(BytesLeft);//splits the read into two buffers
                char * half2=malloc(count-BytesLeft);
                int tempLBA;
                memcpy(half1,buffer,BytesLeft);
                strcat(temp,half1);//check if this works this is assuming it doesnt concat at the end of the 504 bytes allocated
                curr->ByteStored+=BytesLeft;
                file->FileLen+=BytesLeft;
                tempLBA=curr->NextLBA;
                curr->NextLBA=allocate();
                LBAwrite(curr,1,file->FirstDataBlk);

                memset(temp,0,504);
                strcpy(curr->data,temp);
                curr->ByteStored=0;
                curr->NextLBA=0;
                LBAwrite(curr,1,tempLBA);
                LBAread(curr,1,tempLBA);
                memmove(curr->data,buffer+BytesLeft,count-BytesLeft);
                file->FileLen+=count-BytesLeft;
                curr->ByteStored+=count-BytesLeft;
                LBAwrite(curr,1,tempLBA);
            }

        }   
    }
    else{
        int next=curr->NextLBA;
        //LBAread(curr,1,);
    }
    
  
    
}

int b_seek (int fd, off_t offset, int whence){
//lseek(fd,offset,whence);
}
void b_close (int fd){//close the file on our end

}



//NOTE cp or copy file command in fsshell makes a duplicate file in our fs