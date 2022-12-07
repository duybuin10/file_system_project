/**************************************************************
* Class:  CSC-415
* Name: Alex Shirazi, Duy Nguyen, Vi Dao, Manuel Hernandez
* Student ID: 918794583,917131389,920353977,(unknown)
* TEAM-NAME:A-TEAM
* Project: Basic File System
*
* File: mfs.c
*
* Description:mfs.c is the main api to deal with the user interacting with the file system
*
**************************************************************/
#include "mfs.h"
#define blockSize 512
//char* defaultDirName;
//char* CurrentDir=malloc(1024);


int fs_mkdir(const char *pathname, mode_t mode){
    if(strcmp(pathname,"root")==0||strcmp(pathname,".")==0||strcmp(pathname,"..")==0||strcmp(pathname,"NULL")==0||strcmp(pathname,"CAP")==0||strcmp(pathname,"empty")==0){
        printf("Directory name Illegal\n");
        return 0;
    }
    fdDir* DE;
    int size,newBlk;
    size= sizeof(*DE);
    DE = malloc(size);
    char *buf = malloc(blockSize);
    int CurrentDirLBA =absolutePathChild(CurrentDir);
 
    // dirLBA();//Finds the LBA of the current directory

    memset(buf,0,blockSize);//sets every byte in buf to 0 makeing sure there is no garabage data
    LBAread(buf,1,CurrentDirLBA);//reads the first block of data of the current directory 

    //checks to see if dir already exists
    //loop through entries before adding directory to make sure theres no dupliates
    for(int i = 0;i<(blockSize/size);i++){
        memmove(DE,buf+(size*i),size);
        if(strcmp(DE->name,pathname)==0){
            printf("\n Directory already exists please choose another name!\n");
            return 0;
        }

        if(i == 9){//the last entry of every block
            if(strcmp(DE->name,"CAP") == 0){//cap is the name of every 10th directory entry meaning theres one more directory block to search
                i = 0;//sets i to zero to restart the for loop to search th enew block
                LBAread(buf,1,DE->directoryStartLocation);//loads tje next block of a directory is a directory spans more than one block
            }
        }
    }
   
    LBAread(buf,1,CurrentDirLBA);//Rereads the first block of data of the current directory 
    for(int i = 0; i <(blockSize/size) ;i++){
        memmove(DE,buf+(size*i),size);
       
        if(DE->type==0){
            DE->type=2;//PROBABLY NEED TO FILL IN  MORE FIELDS
        
            memset(DE->name,0,20);//clears out garbage data IMPORTANT!
            strcpy(DE->name,pathname);
            DE->directoryStartLocation=allocate();
            memcpy(buf+(size*i),DE,size);
            LBAwrite(buf,1,CurrentDirLBA);//reads the updated directory block onto the disk
            printf("Directory %s Sucessfully Created!\n",pathname);
        
            initChildDir(DE->directoryStartLocation,CurrentDirLBA);//double check LBA passing

            return 1;
        }
        else if( i == 9){
            if(strcmp(DE->name,"CAP") == 0){//cap is the name of every 10th directory entry meaning theres one more directory block to search
                i = 0;//sets i to zero to restart the for loop to search th enew block
                LBAread(buf,1,DE->directoryStartLocation);
            }
            else if(strcmp(DE->name,"NULL") == 0){//we have reached the end of the directory and theres no empty space so we add another block at the end
             
                newBlk=allocate();
                
                DE->directoryStartLocation=newBlk;//instead of being NULL cap now points to the new block that was created to extend the directory
               
                memset(DE->name,0,20);//clears out garbage data IMPORTANT!
                
                strcpy(DE->name,"CAP");
                
                memcpy(buf+(size*9),DE,size);
                 
                LBAwrite(buf,1,newBlk);//reads the updated directory block onto the disk pointing to newly extended directory block as the CAP
                 
                initBlk(newBlk);
              
                LBAread(buf,1,newBlk);
                
               
                DE->type=2;
                
                memset(DE->name,0,20);//clears out garbage data IMPORTANT!
               
                strcpy(DE->name,pathname);
               
                DE->directoryStartLocation=allocate();//holds LBA of the new child directory
            
                initChildDir(DE->directoryStartLocation,dirLBA());//i think this is wrong because it points to the last block of a dir since we searched till the end, need to keep track of first block in directory and pass instead
                
                memcpy(buf,DE,size);
               
                LBAwrite(buf,1,newBlk);
               
            }
        }
    }
   
    free(buf);
    free(DE);   
}//if FS IS FULL NEED TO CHECK IF ALLOCATE CANT FINE ANY MORE SPACE BUT IN CONTEXT OF THIS SCHOOL ASSIGNMENT THAT PROBABLY WONT BE THE CASE

int fs_rmdir(const char *pathname){//NOTE MAKE SURE RMDIR DELETES AND DEALLOCATES ANY FILES IN THE DIRECTORY
if(strcmp(pathname,"root")==0||strcmp(pathname,".")==0||strcmp(pathname,"..")==0||strcmp(pathname,"NULL")==0||strcmp(pathname,"CAP")==0||strcmp(pathname,"empty")==0){
        printf("Directory Illegal to remove\n");
        return 0;
    }
    fdDir* dr;
    int size = sizeof(*dr);
    dr=malloc(size);
    char *buf = malloc(blockSize);
    memset(buf,0,blockSize);
    int CurrentDirLBA = childLBA(CurrentDir);//Finds the LBA of the current directory
  
    

    memset(buf,0,blockSize);
    LBAread(buf,1,CurrentDirLBA);//loads the first block of the directory

//this for loop is reponsible for removing direcrtory entry data
    for(int i = 0; i < 10; i++){
        memmove(dr,buf+(size*i),size);
        if(strcmp(dr->name,pathname)==0 && dr->type == 2){
            //sets fields to default
            dr->type = 0;
            strcpy(dr->name,"empty");
            deallocate(dr->directoryStartLocation);
            memcpy((buf+(sizeof(*dr)*i)),dr,size);  
            
             LBAwrite(buf,1,CurrentDirLBA);//writes to actual desired directory being deleted and sets all bytes to 0      dr->directoryStartLocation
            printf("\n directory successfully removed \n");
           // return 1;
        }
    }

  
       fdDir* DE;
  
    DE=malloc(size);
    
    dr=malloc(size);
    
    //this for loop is responsible for reformating block so existing dir entries always come before empty directories! IMPORTANT FOR LS -l
    for(int i=0;i<10;i++){
        
          memmove(dr,buf+(size*i),size);
        if(strcmp(dr->name,"empty")==0||strcmp(dr->name,"")==0||strlen(dr->name)==0&&strcmp(dr->name,"NULL")!=0&&strcmp(dr->name,"CAP")!=0&&dr->type==0){
            memmove(DE,buf+(size*(i+1)),size);
         if(strcmp(DE->name,"empty")!=0||strcmp(DE->name,"")!=0||strcmp(DE->name,"NULL")!=0||strcmp(DE->name,"CAP")!=0&&DE->type!=0){
            //moves empty placeholder 1 de position
              memcpy(buf+(size*(i+1)),dr,size);
                                                                    
              //cleans garbage data 
              memset(dr,0,48);
              memcpy(buf+(size*i),dr,size);
                //rearranges directories
              memcpy(buf+(size*i),DE,size);
         }
        }
      
    }
 
    LBAwrite(buf,1,CurrentDirLBA);//writes to actual desired directory being deleted and sets all bytes to 0      dr->directoryStartLocation
    
    free(buf);
    //printf("\n Directory does not exist \n");
    return 0;
}


fdDir * fs_opendir(const char *name){//Used for command ls to open a stream to dir and return a dir ptr

    cdBuf=malloc(blockSize);
    fdDir* DE;
    int size = sizeof(*DE);
    DE=malloc(size);

    int lba = childLBA(name);
     DE->dirEntryPosition=0;
     memset(DE->name,0,20);//clears out garbage data IMPORTANT!
    memset(cdBuf,0,blockSize);
    LBAread(cdBuf,1,lba);
   
    memmove(DE,cdBuf,size); //de-name =. 1st entry
   
    DE->dirEntryPosition=48;
    printf("opening directory\n"); 
    return DE;
    }
    struct fs_diriteminfo *fs_readdir(fdDir *dirp){//used in display file func in fsshell and that function is also call by ls. Really this function converts dir name to a fs_diriteminfo type

    struct fs_diriteminfo* DI;
    int size=sizeof(*dirp);
    DI=malloc(259);
    int temp=dirp->dirEntryPosition;


 if(strcmp(dirp->name,".")==0&&strlen(dirp->name)==1){
   

        strcpy(DI->d_name,dirp->name);
     memmove(dirp,cdBuf+temp,size); 
      dirp->dirEntryPosition=temp+48;
     
   
    return DI;
 
    }

    strcpy(DI->d_name,dirp->name);
  
    memmove(dirp,cdBuf+temp,size); 
    dirp->dirEntryPosition=temp+size;

   if(strcmp(DI->d_name,"NULL")==0||strcmp(DI->d_name,"empty")==0||strcmp(DI->d_name,"CAP")==0){
       return NULL;
   }

    return DI;
        //char arr[/*numer of entries in the directory*/];


}
int fs_closedir(fdDir *dirp){//used in display files assuming to close the strea



free(cdBuf);
}

char * fs_getcwd(char *buf, size_t size){//return current array

strcpy(buf,CurrentDir);

return buf;
}

int fs_setcwd(char *buf){//linux chdir buf==path passed in
    //the way our dir is setup the global array 
    //buf = path  ex root/dir1/dir2    dir2     /dir1/dir2
    char* newPath= malloc(sizeof(CurrentDir));
  
    int lba = childLBA(buf); //core dump here!
    
    if(lba==-1){
        printf("no such file or directory\n");
        return 1;
    }
 
    CurrentDir=convertToAbsolutePath(buf);
    
  
   return 0;
    
}
int fs_isFile(char * path){//return 1 if file, 0 otherwise
    //Probably needs to be changed but assuming path is the file name and were searching in the current level of the directory
    //also this is set up assuming a dir is one block long which wont be the case most of the time so need to set up logic for 
    //the end cap of the dir block that points to the next block
    //this state of incomplete logic is the case for both isdir and isfile
    fdDir* DE;
    int size;
    size= sizeof(*DE);
    char* buf = malloc(blockSize);
    DE = malloc(size);
    memset(buf,0,blockSize);//sets every byte in buf to 0 makeing sure there is no garabage data
    LBAread(buf,1,6);//hard coded change later
    for(int i=0; i < 10; i++){    //hard coded dir size
        memmove(DE,buf+(size*i),size);
        if(strcmp(DE->name,path)==0){
            if(DE->type == 0||DE->type==2){
                return 0;
            }
            else if(DE->type == 1){
                return 1;
            }
        }
    }
    
}
int fs_isDir(char * path){//return 1 if directory, 0 otherwise
    fdDir* DE;
    int size;
    size= sizeof(*DE);
    char* buf = malloc(blockSize);
    DE = malloc(size);
    memset(buf,0,blockSize);//sets every byte in buf to 0 makeing sure there is no garabage data
    LBAread(buf,1,6);//hard coded change later
    for(int i=0; i < 10; i++){//EQUALS IS ZERO
        memmove(DE,buf+(size*i),size);
        if(DE->name==path){
            if(strcmp(DE->name,path)==0 &&DE->type==2){
                return 1;
            }
            else if(DE->type == 1||DE->type==0){
                return 0;
            }
        }
    }
}



int fs_delete(char* filename){	//removes a file
    int CurrDir = dirLBA();
    fdDir* DE;
    int size,FileLBA;
    size= sizeof(*DE);
    char* buf = malloc(blockSize);
    DE = malloc(size);
    LBAread(buf,1,CurrDir);
  
    for(int i = 0; i<blockSize/size;i++){
        memcpy(DE,buf+(i*size),size);
        if(strcmp(DE->name,filename) == 0){
            DelFCB(DE->directoryStartLocation);
        }
        else if(strcmp(DE->name,"CAP") == 0){
            LBAread(buf,1,DE->directoryStartLocation);
            i = 0;
        }
        else if(strcmp(DE->name,NULL) == 0){
            printf("/n File does not exist in current directory/n");
            return 0;
        }
    }
    DelFCB(FileLBA);
}

int fs_stat(const char *path, struct fs_stat *buf){
    char* buf1=malloc(blockSize); // read whole block
    fdDir* DE;
    int size = sizeof(*DE);
    DE=malloc(size);
// //compare *path to all dir names in currentworking directory
// //if path ==(a child name)
// //set size = whatever size is read
// //set type to 1 if file 2 if direcotry
// //return 1 if sucess 0 if fail
    int lbaParent=6;
    int lba = childLBA(path);
     memset(DE->name,0,20);
    memset(buf1,0,blockSize);

    LBAread(buf1,1,lba);
   
   
    //LBAread(buf1,1,lbaParent);
    int lsBytes = 0;
    
     
             //set all data logic
        //modify later for file sizes included
    memset(DE,0,size);
    memset(buf1,0,blockSize);
    LBAread(buf1,1,lba);
      for (int i =0; i< (blockSize/size); i++){
          memmove(DE,buf1+(size*i),size);
              if(strcmp(path,"NULL")==0){
                  break;
              }
          if(strcmp(path,"empty")!=0||strcmp(path,"")!=0||strcmp(path,"CAP")!=0){
              lsBytes++;
         }
        break;
     }
  

 



int totalBytes = size*lsBytes;
buf->st_size=totalBytes;
     return 1;


}


int mv_dir(char* filename, char* path){
     fdDir* source;
     fdDir* destination;
    
    int size = sizeof(*destination);
    
    destination=malloc(size);

//start of file search


source=malloc(size);
char* buf = malloc(blockSize);
memset(source,0,size);
memset(buf,0,blockSize);
//for locating the file directory entry
int CurrentDirLBA=childLBA(CurrentDir);

 LBAread(buf,1,CurrentDirLBA);
for(int i=0;i<10;i++){
    memmove(source,buf+(size*i),size);
     if(strcmp(source->name,"NULL")==0){
                printf("No Such Filename!\n");
                return 0;
            }
        
        if(strcmp(filename,source->name)==0&&source->type==1){

            
            //before deleting the file, we must check if the path to the new directory is VALID!!! 
            int dirValidator = childLBA(path);
            if(dirValidator!=0){
                fdDir* check = malloc(size);
                memset(buf,0,blockSize);
                LBAread(buf,1,dirValidator);
                for(int i=0;i<10;i++){
                     memmove(check,buf+(size*i),size);
                     if(strcmp(check->name,"empty")==0){
                     fs_rmFile(filename);
                     break;
                     }else if (strcmp(check->name,"NULL")==0){
                         printf("no space in Destination Directory\n");
                         return 0;
                     }
                }
            break;
            //deleting file!
        }
        }
      
    }

 memset(buf,0,blockSize);  
int destLba = childLBA(path);
LBAread(buf,1,destLba);
memset(destination,0,size);


for(int i=0;i<10;i++){
memmove(destination,buf+(size*i),size);

if(strcmp(destination->name,"empty")==0){

    memcpy(buf+(size*i),source,size);
    LBAwrite(buf,1,destLba);
    printf("move sucessful!\n");
    return 1;
}
}
//in the event that destination directory is full, we must put back the file entry in its original directory


return 0;

}


int fs_rmFile(const char *pathname){

    fdDir* dr;
    int size = sizeof(*dr);
    dr=malloc(size);
    char *buf = malloc(blockSize);
    memset(buf,0,blockSize);
    int CurrentDirLBA = childLBA(CurrentDir);//Finds the LBA of the current directory
    
    

    memset(buf,0,blockSize);
    LBAread(buf,1,CurrentDirLBA);//loads the first block of the directory

//this for loop is reponsible for removing file entry data
    for(int i = 0; i < 10; i++){
        memmove(dr,buf+(size*i),size);
        if(strcmp(dr->name,pathname)==0 && dr->type == 1){
            //sets fields to default
            memset(dr,0,size);
            dr->type = 0;
            strcpy(dr->name,"empty");
            memcpy((buf+(size*i)),dr,size);  
            
             LBAwrite(buf,1,CurrentDirLBA);
           
           // return 1;
        }
    }

  
       fdDir* DE;
  
    DE=malloc(size);
    
    dr=malloc(size);
    
    //this for loop is responsible for reformating block so existing dir entries always come before empty ones! IMPORTANT FOR LS -l
    for(int i=0;i<10;i++){
        
          memmove(dr,buf+(size*i),size);
        if(strcmp(dr->name,"empty")==0||strcmp(dr->name,"")==0||strlen(dr->name)==0&&strcmp(dr->name,"NULL")!=0&&strcmp(dr->name,"CAP")!=0&&dr->type==0){
            memmove(DE,buf+(size*(i+1)),size);
         if(strcmp(DE->name,"empty")!=0||strcmp(DE->name,"")!=0||strcmp(DE->name,"NULL")!=0||strcmp(DE->name,"CAP")!=0&&DE->type!=0){
        
            //moves empty placeholder 1 position
              memcpy(buf+(size*(i+1)),dr,size);                                                      
              //cleans garbage data 
              memset(dr,0,48);
              memcpy(buf+(size*i),dr,size);
                //rearranges directories
              memcpy(buf+(size*i),DE,size);
         }
        }
      
    }
 
    LBAwrite(buf,1,CurrentDirLBA);//writes to actual desired directory being deleted and sets all bytes to 0      dr->directoryStartLocation
    
    free(buf);
    //printf("\n Directory does not exist \n");
    return 0;
}