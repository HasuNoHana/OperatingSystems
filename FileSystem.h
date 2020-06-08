#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_FILE_NR 16
#define MAX_FILE_NAME  14
#define BLOCK_DATA_SIZE 1020
#define BLOCK_SIZE 1024
#define VFSFILENAME "FileSystem"

typedef struct Block{
    char data[BLOCK_DATA_SIZE];
    unsigned int next_block;
} BLOCK;

typedef struct Super_block{
    unsigned int size, user_space, user_space_in_use, blocks_nr, first_INode;
} SUPERBLOCK;

typedef struct INode{
    char name[MAX_FILE_NAME];
    unsigned int size, first_block;
} INODE;

FILE *vfs;
SUPERBLOCK *super_block;
char inode_bitmap[MAX_FILE_NR];
char *blocks_bitmap;

/* Main functions of VFS */
int init_VFS(unsigned int size);
int delete_VFS(void);
int copy_from_VFS(char *filename );
int copy_to_VFS(char *filename);
int delete_file(char *filename);
int display_directory(void);
int display_disk_status(void);

/* Auxiliary functions of VFS*/
int save_vfs(void);
int load_vfs(void);
int getInodeOffset(int pos);
int getBlockOffset(int pos);
unsigned int getFreeSpace(void);
unsigned int getFileSize(FILE* file);
unsigned int getBlocksNumber(unsigned int size);
int findFile(char* filename);
int getFreeInode(void);
int getFreeBlock(void);
