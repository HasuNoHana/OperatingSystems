#include "FileSystem.h"

int init_VFS(unsigned int size)
{
    unsigned int blocks_nr, fs_size;
    /* creating file system */
    vfs = fopen(VFSFILENAME,"wb");
    if(!vfs){
        printf("File system creation failed\n");
        return 1;
    }
    /* calculating number of blocks and size of file system*/
    blocks_nr = getBlocksNumber(size);
    fs_size = sizeof(SUPERBLOCK) + MAX_FILE_NR * sizeof(INODE) + blocks_nr * BLOCK_SIZE;

    /* defining superblock */
    super_block = (SUPERBLOCK*)malloc(sizeof(SUPERBLOCK));
    if(!super_block){
        printf("Allocation error while allocating memory for super block\n");
        fclose(vfs);
        return 2;
    }
    super_block->size = fs_size;
    super_block->blocks_nr = blocks_nr;
    super_block->user_space = BLOCK_DATA_SIZE*blocks_nr;
    super_block->user_space_in_use = 0;
    super_block->first_INode = MAX_FILE_NR;

    /* definition of bitmap of blocks */
    blocks_bitmap = (char*)calloc(blocks_nr, sizeof(char));
    if(!blocks_bitmap)
    {
        printf("Allocation error while allocating memory for super blocks_bitmap\n");
        fclose(vfs);
        free(super_block);
        return 2;
    }

    if(save_vfs())
    {
        printf("Error occured while saving file system");
        fclose(vfs);
        free(super_block);
        free(blocks_bitmap);
        return 3;
    }
    printf("Successfully created file system\n");
    return 0;

}

/* returns number of blocks in system file for given size */
unsigned int getBlocksNumber(unsigned int size) {
    return (size + BLOCK_SIZE - 1) / BLOCK_SIZE;
}

int display_disk_status(void)
{
    int i;
    printf("\nDISK STATUS\n");
    printf("Adress: 0 | SuperBlock:\n");
    printf("Total size:\t %d \n", super_block->size);
    printf("User space:\t %d \n", super_block->user_space);
    printf("Free:      \t %d \n", super_block->user_space - super_block->user_space_in_use );
    printf("In use:    \t %d \n", super_block->user_space_in_use);
    printf("Block size:\t %ld \n", sizeof(BLOCK));
    printf("Number of blocks: %d \n", super_block->blocks_nr);

    printf("\nAdress: %d | INode bitmap:\n", getInodeOffset(0));
    for(i = 0; i < MAX_FILE_NR; ++i)
        printf("%c", inode_bitmap[i] + '0');
    printf("\n");

    printf("\nAdress: %d | Block bitmap:\n", getBlockOffset(0));
    for(i = 0; i < super_block->blocks_nr; ++i)
        printf("%c", blocks_bitmap[i] + '0');
    printf("\n");

    return 0;
}

int display_directory(void)
{
    int i;
    INODE* inode = malloc(sizeof(INODE));
    if(!inode)
    {
        printf("Allocation error for INode\n");
        return 1;
    }

    printf("\nFiles\n");
    for(i = 0; i < MAX_FILE_NR; ++i)
    {
        if(inode_bitmap[i])
        {
            /* Sets the position indicator associated with the vfs to a new position. */
            fseek(vfs, getInodeOffset(i), SEEK_SET);
            if(fread(inode, sizeof(INODE), 1, vfs) != 1)
            {
                printf("Error occured while reading INode\n");
                free(inode);
                return 1;
            }
            printf("Name: \t%s\n",inode->name);
            printf("Size: \t%d\n",inode->size);
            printf("Block:\t%d\n\n",inode->first_block);
        }
    }
    free(inode);
    return 0;
}

int delete_VFS(void)
{
    if(remove(VFSFILENAME))
    {
        printf("Succesfully deleted file system\n");
        return 0;
    }
    return 1;
}

int copy_to_VFS(char* filename)
{
    FILE* file;
    INODE* temp_inode;
    BLOCK* temp_block;
    unsigned int file_size;
    int inode_pos, block_pos1, block_pos2, file_size_blocks;
    int i,j;
    double d;

    printf("\nCopying %s\n",filename);
    if(findFile(filename) != -1)
    {
        printf("File already exists\n",filename);
        return 6;
    }
    /* find free INode */
    inode_pos = getFreeInode();
    if(inode_pos == MAX_FILE_NR)
    {
        printf("Reached maximal number of files\n");
        return 5;
    }
    if(strlen(filename) >= MAX_FILE_NAME)
    {
        printf("File name too long\n");
        return 1;
    }
    file = fopen(filename,"r+b");
    if(!file)
    {
        printf("Error occured while opening file\n", filename);
        return 2;
    }
    file_size = getFileSize(file);
    if(getFreeSpace() < file_size)
    {
        printf("No space left on disk\n");
        fclose(file);
        return 3;
    }
    temp_inode = (INODE*)malloc(sizeof(INODE));
    if(!temp_inode)
    {
        printf("Allocation error for INode\n");
        return 4;
    }
    temp_block = (BLOCK*)malloc(sizeof(BLOCK));
    if(!temp_block)
    {
        printf("Allocation error for block\n");
        return 5;
    }
    file_size_blocks = getBlocksNumber(file_size);

    /* definining INode */
    temp_inode->size=file_size;
    strcpy(temp_inode->name, filename);
    block_pos1 = getFreeBlock();
    temp_inode->first_block = block_pos1;

    fseek(vfs, getInodeOffset(inode_pos), SEEK_SET);
    if (fwrite(temp_inode, sizeof(INODE), 1, vfs) != 1)
    {
        printf("Error occured while writing to the INode\n");
        fclose(file);
        free(temp_block);
        free(temp_inode);
        free(super_block);
        free(blocks_bitmap);
        fclose(vfs);
        return 6;
    }
    free(temp_inode);

    /* finding empty blocks and saving file in them */
    for(i = 0; i < file_size_blocks; ++i)
    {
        if(fread(temp_block->data, BLOCK_DATA_SIZE, 1, file) != 1 && !feof(file))
        {
            printf("Error occured while reading the file\n", filename);
            fclose(file);
            free(temp_block);
            free(super_block);
            free(blocks_bitmap);
            fclose(vfs);
            return 7;
        }
        /* find empty slot and save part of the file */
        if(i + 1 < file_size_blocks)
        {
            block_pos2 = getFreeBlock();
            temp_block->next_block = block_pos2;
        }else temp_block->next_block = super_block->blocks_nr; /* blocks_nr symbolizes null */

        fseek(vfs, getBlockOffset(block_pos1), SEEK_SET);
        if (fwrite(temp_block, sizeof(BLOCK), 1, vfs) != 1)
        {
            printf("Error occured while writing to block number %d\n",i);
            fclose(file);
            free(temp_block);
            free(super_block);
            free(blocks_bitmap);
            fclose(vfs);
            return 8;
        }
        block_pos1 = block_pos2; /* keep previous block position */
        memset(temp_block->data, 0, BLOCK_DATA_SIZE);
    }

    free(temp_block);

    /* update user space */
    super_block->user_space_in_use += file_size_blocks * BLOCK_DATA_SIZE;
    save_vfs();
    printf("Succesfully copied file: %s\n",filename);
    fclose(file);
    return 0;
}

int copy_from_VFS(char* filename)
{
    char *new_filename;
    FILE* file;
    int block_pos, inode_pos, over_size, block_size,i;
    BLOCK* block;
    INODE* inode;
    printf("\nCopyting file %s outside of virtual disk\n",filename);
    inode_pos = findFile(filename);
    if(inode_pos == -1)
    {
        printf("File doesn't exist\n",filename);
        return 1;
    }
    new_filename = strcat(filename,"_2");
    file = fopen(new_filename, "w");
    if(!file)
    {
        printf("Error occured while creating the file\n");
        return 1;
    }
    inode =(INODE*)malloc(sizeof(INODE));
    if(!inode)
    {
        printf("Allocation error while allocating memory for INode\n");
        return 1;
    }
    block = (BLOCK*)malloc(sizeof(BLOCK));
    if(!block)
    {
        printf("Allocation error while allocating memory for block\n");
        free(inode);
        return 1;
    }
    fseek(vfs, getInodeOffset(inode_pos),SEEK_SET);
    if (fread(inode, sizeof(INODE),1,vfs)!=1)
    {
        printf("Error occured while reading INode\n");
        free(inode);
        free(block);
        return 1;
    }

    block_pos = inode->first_block;
    over_size = inode->size%BLOCK_DATA_SIZE;
    block_size = inode->size/BLOCK_DATA_SIZE;
    /* loop over blocks which contain copied file and append to file*/
    for(i = 0; i < block_size; ++i)
    {
        fseek(vfs, getBlockOffset(block_pos),SEEK_SET);
        if(fread(block,sizeof(BLOCK),1,vfs) != 1)
        {
            printf("Error occured while writing to block number %d\n", i);
            free(inode);
            free(block);
            fclose(file);
            remove(new_filename);
            free(super_block);
            free(blocks_bitmap);
            fclose(vfs);
            return 1;
        }
        if(fwrite(block->data, BLOCK_DATA_SIZE, 1, file) != 1)
        {
            printf("Error occured while writing to block number %d!!!---\n",i);
            free(inode);
            free(block);
            fclose(file);
            remove(new_filename);
            free(super_block);
            free(blocks_bitmap);
            fclose(vfs);
            return 1;
        }
        block_pos = block->next_block;
    }

    fseek(vfs,getBlockOffset(block_pos),SEEK_SET);
    if(fread(block,sizeof(BLOCK),1,vfs) != 1)
    {
        printf("Error occured while reading block number %d\n",i);
        free(inode);
        free(block);
        fclose(file);
        remove(new_filename);
        free(super_block);
        free(blocks_bitmap);
        fclose(vfs);
        return 1;
    }
    if(fwrite(block->data, over_size, 1, file) != 1)
    {
        printf("Error occured while writing to block number %d\n",i);
        free(inode);
        free(block);
        fclose(file);
        remove(new_filename);
        free(super_block);
        free(blocks_bitmap);
        fclose(vfs);
        return 1;
    }

    printf("Succesfully copied file %s to the physical disk\n",new_filename);
    return 0;
}

int delete_file(char* filename)
{
    int block_pos, inode_pos;
    BLOCK* block;
    INODE* inode;

    printf("Removing file %s\n",filename);
    inode =(INODE*)malloc(sizeof(INODE));
    if(!inode)
    {
        printf("Allocation error while allocating memory for INode\n");
        return 1;
    }
    block = (BLOCK*)malloc(sizeof(BLOCK));
    if(!block)
    {
        printf("Allocation error while allocating memory for Block\n");
        free(inode);
        return 1;
    }
    inode_pos = findFile(filename);
    if(inode_pos == -1)
    {
        printf("File %s doesn't exist\n",filename);
        free(inode);
        free(block);
        return 1;
    }
    fseek(vfs, getInodeOffset(inode_pos),SEEK_SET);
    if (fread(inode, sizeof(INODE),1,vfs)!=1)
    {
        printf("Allocation error while reading INode\n");
        free(inode);
        free(block);
        return 1;
    }

    /* updating INode and Blocks bitmaps */
    inode_bitmap[inode_pos] = 0;
    block_pos = inode->first_block;
    do{
        fseek(vfs,getBlockOffset(block_pos),SEEK_SET);
        if(fread(block,sizeof(BLOCK),1,vfs) != 1)
        {
            printf("Allocation error while reading Block\n");
            free(inode);
            free(block);
            return 1;
        }
        blocks_bitmap[block_pos] = 0;
        block_pos = block->next_block;
        super_block->user_space_in_use -= BLOCK_DATA_SIZE;
    }
    while(block_pos != super_block->blocks_nr);
    if(super_block->first_INode > inode_pos)
        super_block->first_INode = inode_pos;

    free(inode);
    free(block);
    save_vfs();
    printf("Succesfully removed file %s\n",filename);
    return 0;
}


int findFile(char* filename)
{
    int i;
    INODE* inode = malloc(sizeof(inode));
    for(i = 0; i < MAX_FILE_NR; ++i)
    {
        if(inode_bitmap[i] == 1)
        {
            fseek(vfs, getInodeOffset(i), SEEK_SET);
            if(fread(inode, sizeof(INODE), 1, vfs) != 1)
            {
                printf("Error occured while reading INode\n");
                free(inode);
                return -1;
            }
            if(strcmp(inode->name,filename) == 0)
            {
                free(inode);
                return i;
            }
        }
    }
    free(inode);
    return -1;
}


int getFreeInode(void)
{
    int i,j, ret;

    if(super_block->first_INode < MAX_FILE_NR)
    {
        ret = super_block->first_INode;
        i = ret;
    }
    else
    {
        for ( i = 0; i < MAX_FILE_NR && inode_bitmap[i] != 0; ++i);
        ret = i;
    }
    for ( j = i+1; j < MAX_FILE_NR && inode_bitmap[j] != 0; ++j);
    if(j>=MAX_FILE_NR) --j;
    super_block->first_INode = j;
    inode_bitmap[ret] = 1;
    return ret;

}
int getFreeBlock(void)
{
    int i;
    for( i = 0; i < super_block->blocks_nr && blocks_bitmap[i] != 0; ++i);
    blocks_bitmap[i]=1;
    return i;
}
int getInodeOffset(int pos)
{
    return sizeof(SUPERBLOCK)+super_block->blocks_nr +sizeof(inode_bitmap)+pos*sizeof(INODE);
}
int getBlockOffset(int pos)
{
    return sizeof(SUPERBLOCK)+super_block->blocks_nr+sizeof(inode_bitmap)+MAX_FILE_NR*sizeof(INODE)+pos*sizeof(BLOCK);
}
unsigned int getFreeSpace()
{
    return (super_block->user_space - super_block->user_space_in_use);
}
int save_vfs(void)
{
    rewind(vfs);
    if(fwrite(super_block, sizeof(SUPERBLOCK), 1, vfs) != 1)
    {
        printf("Error occured while writing to super block\n");
        return 1;
    }

    if (fwrite(inode_bitmap, MAX_FILE_NR, 1, vfs) != 1){
        printf("Error occured while writing to INode bitmap\n");
        return 1;
    }
    if (fwrite(blocks_bitmap, super_block->blocks_nr, 1, vfs) != 1){
        printf("Error occured while writing to blocks bitmap\n");
        return 1;
    }
    return 0;
}
int load_vfs(void)
{
    vfs = fopen(VFSFILENAME,"r+b");
    if(!vfs){
        printf("Error occured while opening file system\n");
        exit(1);
    }
    super_block = (SUPERBLOCK*)malloc(sizeof(SUPERBLOCK));
    if(!super_block){
        printf("Allocation error while allocating memory for super block\n");
        fclose(vfs);
        exit(2);
    }
    if(fread(super_block, sizeof(SUPERBLOCK), 1, vfs) != 1)
    {
        printf("Error occured while reading from super block\n");
        fclose(vfs);
        free(super_block);
        exit(3);
    }
    if(fread(inode_bitmap, MAX_FILE_NR, 1, vfs) != 1)
    {
        printf("Error occured while reading from INode bitmap\n");
        fclose(vfs);
        free(super_block);
        exit(3);
    }
    blocks_bitmap = (char*)calloc(super_block->blocks_nr, sizeof(char));
    if(!blocks_bitmap)
    {
        printf("Allocation error while allocating memory for blocks bitmap\n");
        fclose(vfs);
        free(super_block);
        exit(2);
    }
    if(fread(blocks_bitmap, super_block->blocks_nr, 1, vfs) != 1)
    {
        printf("Error occured while reading from blocks bitmap\n");
        fclose(vfs);
        free(super_block);
        free(blocks_bitmap);
        exit(3);
    }

    return 0;
}

unsigned int getFileSize(FILE* file)
{
    unsigned int size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    return size;
}


int main(int argc, char* argv[]) {
    if(argc < 2) {
        printf("Too little arguments\n");
        return -1;
    }
/*
	arguments that you can give to program:
		c <number> 		- create file system that have size <number>
		st <file name> 		- save file from minix on file system
		sf <file name> 		- save file to minix from file system
		r <file name> 		- remove file from file system
		l 			- list all files in file system
		m 			- show how mutch of memory is used
		d 			- delete file system
*/

    switch (argv[1][0])
    {
        case 'c':/*CREATE FILE SYSTEM*/
            if(argc<3) {
                printf("Too little arguments, give size");
                return -1;
            }
	    printf("creating new file system of size %d\n", atoi(argv[2]));
            if(init_VFS(atoi(argv[2]))) return -2;

            break;
        case 's':/*SAVE TO OR FROM FILE SYSTEM*/
            if(argc < 3)
            {
                printf("Invalid saving, it should look like this:\nsave file in file system: st filename\nsave file from file system: sf filename ");
                return -6;
            }
            if(argv[1][1] == 't')
            {
		printf("Saving file to file system\n");
                load_vfs();
                if(copy_to_VFS(argv[2])) return -4;
            }
            else if(argv[1][1] == 'f')
            {
		printf("Saving file from file system\n");
                load_vfs();
                if(copy_from_VFS(argv[2])) return -5;
            }
            else
            {
                printf("Invalid saving, it should look like this:\nsave file in file system: st filename\nsave file from file system: sf filename ");
                return -6;
            }
            break;
        case 'r':/*DELETE FILE*/
            if(argc < 3)
            {
                printf("Invalid saving, it should look like this:\nr filename");
                return -6;
            }
	    printf("deleting file from file system\n");
            load_vfs();
            if(delete_file(argv[2])) return -9;
            break;
	case 'l':/*LIST FILES IN FILE SYSTEM*/
	    printf("listing all files in file system\n");
            load_vfs();
            if(display_directory()) return -7;
            break;
        case 'm':/*SHOW DISC STATUS*/
	    printf("showing disc status\n");
            load_vfs();
            if(display_disk_status()) return -8;
            break;
	case 'd':/*DELETE FILE SYSTEM*/
	    printf("deleting the file system\n");
            if(delete_VFS()) return -3;
            break;
        default:
            printf("instruction unknown\n");
            break;
    }

    fclose(vfs);
    free(super_block);
    free(blocks_bitmap);
    return 0;
}
